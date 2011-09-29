#!/usr/bin/ruby
GPUPRICER_PATH = "~/gpupricer/AmerOptionPricer/src-gpu-cuda-orwl"
GLOBAL_ADDRESS_BOOK = ARGV.shift
LOCAL_ADDRESS_BOOK = ARGV.shift
RANK = ARGV.shift.to_i
OUTPUT = ARGV.shift
nb_tasks = ARGV[0].to_i
graph_file = ARGV[1]


orwl_cmd = "rm -f #{LOCAL_ADDRESS_BOOK} ; cd #{GPUPRICER_PATH} ;  export ORWL_SECRET=12 ; ./AmerMal -ORWL_GRAPH #{graph_file} -ORWL_GLOBAL_AB #{GLOBAL_ADDRESS_BOOK} -ORWL_LOCAL_AB #{LOCAL_ADDRESS_BOOK} -RANK #{RANK - 1} -NBPE #{nb_tasks} -P ../../input-data-Amer/ &>#{OUTPUT}"
puts(orwl_cmd)
system(orwl_cmd)
