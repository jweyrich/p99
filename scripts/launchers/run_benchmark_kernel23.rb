#!/usr/bin/ruby
ORWL_PATH = "~/orwl/tests"
GLOBAL_ADDRESS_BOOK = ARGV.shift
LOCAL_ADDRESS_BOOK = ARGV.shift
OUTPUT = ARGV.shift
RANK = ARGV.shift
nb_tasks_per_node = ARGV[0].to_i
submatrix_size = ARGV[1].to_i
iterations = ARGV[2].to_i
nb_rows = ARGV[3].to_i
nb_cols = ARGV[4].to_i
graph_file = ARGV[5]

list = Array.new
(0..(nb_tasks_per_node - 1)).each { |current|
  val = ((RANK - 1) * nb_tasks_per_node * 9) + (current * 9)
  list.push(val)
}
orwl_cmd = "rm -f #{LOCAL_ADDRESS_BOOK} ; cd #{ORWL_PATH} ; export ORWL_SECRET=7 ;  LD_PRELOAD=../lib/libtiming_instrument.so ./orwl_benchmark_kernel23 #{submatrix_size} #{iterations} #{nb_rows} #{nb_cols} #{graph_file} #{GLOBAL_ADDRESS_BOOK} #{LOCAL_ADDRESS_BOOK} #{list.join(",")} &>#{OUTPUT}"
puts(orwl_cmd)
system(orwl_cmd)
