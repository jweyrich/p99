#!/usr/bin/ruby
ORWL_PATH = "~/orwl/tests"
GLOBAL_ADDRESS_BOOK = ARGV.shift
LOCAL_ADDRESS_BOOK = ARGV.shift
RANK = ARGV.shift.to_i
OUTPUT = ARGV.shift
nb_tasks_per_node = ARGV[0].to_i
nb_tasks = ARGV[1].to_i
iterations = ARGV[2].to_i
graph_file = ARGV[3]
shared_memory = ARGV[4].to_i
nb_inner_loops = ARGV[5].to_i

list = Array.new
if (nb_tasks_per_node > 1) then
  (0..(nb_tasks_per_node - 1)).each { |current|
    val = ((RANK - 1) * nb_tasks_per_node) + current
    list.push(val)
  }
else
  list.push(RANK - 1)
end


orwl_cmd = "rm -f #{LOCAL_ADDRESS_BOOK} ; cd #{ORWL_PATH} ; export ORWL_SECRET=7 ;  ./orwl_benchmark_synthetic #{iterations} #{nb_tasks} #{graph_file} #{GLOBAL_ADDRESS_BOOK} #{LOCAL_ADDRESS_BOOK} #{list.join(",")} #{shared_memory} #{nb_inner_loops} 2>#{OUTPUT}"
puts(orwl_cmd)
system(orwl_cmd)
