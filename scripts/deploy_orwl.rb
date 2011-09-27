#!/usr/bin/ruby
#
# Usage: deploy_orwl.rb machine_file output_path exec exec_arguments
#
# Notes:
# This script aims at deploying an application based on ORWL. In particular it manages
# the broadcast of the address book to every nodes and the kick-off of the application.
# It launches the application exec on all the nodes of the machine_file and fetches all
# the outputs in output_path. The script transmit several parameter to the exec
# application (in the order): path to the expected global address book, path to the
# local address book, rank, path to the local output, all the arguments provided in
# exec_arguments.
#
# Warning:
# A passwordless connection must be available between all the nodes for the Taktuk
# execution.


require "tempfile"

# Configuration stuffs
GLOBAL_ADDRESS_BOOK = "/tmp/my_global_ab"
LOCAL_ADDRESS_BOOK = "/tmp/my_local_ab"
LOCAL_OUTPUT = "/tmp/my_orwl_output"
CONNECTOR = "oarsh -q -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -o PreferredAuthentications=publickey -o BatchMode=yes"

def run(p)
  puts p
  system(p)
end

taktuk_header = "taktuk -s -c \"#{CONNECTOR}\""

machine_file = ARGV.shift
output_path = ARGV.shift
script = ARGV.shift

nodes = Array.new
IO.readlines(machine_file).each { |line|
  if not (/^$/ =~ line) then
    nodes.push(line.chomp)
  end
}

node_list = String.new
nodes.each { |m|
  node_list += " -m #{m}"
}
node_list += " "

# Launch the application in background on all the nodes
taktuk_thread = Thread.new {
  taktuk_cmd = "broadcast exec [ '#{script} #{GLOBAL_ADDRESS_BOOK} #{LOCAL_ADDRESS_BOOK} $TAKTUK_RANK #{LOCAL_OUTPUT} #{ARGV.join(" ")}' ]"
  run(taktuk_header + node_list + taktuk_cmd)
}

# Wait for the production of the local address books
taktuk_cmd = "broadcast exec [ 'while [ ! -r #{LOCAL_ADDRESS_BOOK} ]; do sleep 1 ; done ;' ]"
run(taktuk_header + node_list + taktuk_cmd)

# Fetch the local address books and build the global address book
run("rm -f #{LOCAL_ADDRESS_BOOK}.*")
taktuk_cmd = "broadcast get [ #{LOCAL_ADDRESS_BOOK} ] [ '#{LOCAL_ADDRESS_BOOK}.$rank' ]"
run(taktuk_header + node_list + taktuk_cmd)
tmp = Tempfile.open("orwl_tmp")
run("rm -f #{GLOBAL_ADDRESS_BOOK} ; cat #{LOCAL_ADDRESS_BOOK}.* > #{tmp.path}")

# Broadcast the global address book
taktuk_cmd = "broadcast put [ #{tmp.path} ] [ #{GLOBAL_ADDRESS_BOOK} ]"
run(taktuk_header + node_list + taktuk_cmd)
tmp.close

# Remove the local address books to kick-off the application
taktuk_cmd = "broadcast exec [ 'rm #{LOCAL_ADDRESS_BOOK}' ]"
run(taktuk_header + node_list + taktuk_cmd)

# Wait for the end of the execution
taktuk_thread.join

# Fetch the outputs
taktuk_cmd = "broadcast get [ #{LOCAL_OUTPUT} ] [ '#{output_path}/#{File.basename(LOCAL_OUTPUT)}.$rank' ]"
run(taktuk_header + node_list + taktuk_cmd)

# Remove the global address books and outputs
taktuk_cmd = "broadcast exec [ 'rm #{GLOBAL_ADDRESS_BOOK} #{LOCAL_OUTPUT}' ]"
run(taktuk_header + node_list + taktuk_cmd)
