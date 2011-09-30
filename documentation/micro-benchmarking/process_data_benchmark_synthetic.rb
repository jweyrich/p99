#!/usr/bin/ruby

require "tmpdir"

source_dir = ARGV[0]
nb_tasks = ARGV[1]
tasks_per_node = ARGV[2].split(",")
graph_suffix = ARGV[3]
timers = ARGV[4]

cat_dir = Dir.mktmpdir("cat")

# Memory variations
array_mem = [0.0078125,0.03125,0.125,0.5,1,4,16,32]
# Inner loops variations
array_inner_iter = [1,5,10,100] 

# Read the timers file
array_timers = Array.new
IO.readlines(timers).each { |timer|
  if not(timer =~ /\A\Z/) then
    array_timers.push(timer.chomp)
  end
}

# Generate a unique file for each run
tasks_per_node.each { |tpn|
  array_inner_iter.each  { |inner_iter|
    array_mem.each { |mem|
      source_file = "#{source_dir}/#{nb_tasks}_#{tpn}_#{mem}_#{inner_iter}"
      output_file = "#{cat_dir}/#{nb_tasks}_#{tpn}_#{mem}_#{inner_iter}"
      system("grep --no-filename TIMING #{source_file}/orwl_output.* |grep -v \"pt of measure\" > #{output_file}")
    }
  }
}

completeHash = Hash.new

# Compute the average 
PROC = 0
OCC = 1
TTIME = 2
ATIME = 3
DTIME = 4
tasks_per_node.each { |tpn|
  array_inner_iter.each  { |inner_iter|
    array_mem.each { |mem|
      # Read the file and store the values in a hashtable
      hash = Hash.new
      IO.readlines("#{cat_dir}/#{nb_tasks}_#{tpn}_#{mem}_#{inner_iter}").each { |line|
        if /\ATIMING:\s+(\w+)\s+(\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\s+(\d+\.\d+)\Z/ =~ line then
          content = Regexp.last_match
          type = content[1]
          occur = content[2].to_i
          total_time = content[3].to_f
          average_time = content[4].to_f
          deviation_time = content[5].to_f
          
          if not hash.has_key?(type) then
            hash[type] = Array.new
            hash[type][PROC] = 0
            hash[type][OCC] = 0
            hash[type][TTIME] = 0
            hash[type][ATIME] = 0
            hash[type][DTIME] = 0
          end
          hash[type][PROC] += 1
          hash[type][OCC] += occur
          hash[type][TTIME] += total_time
          hash[type][ATIME] += average_time
          hash[type][DTIME] += deviation_time
        end
      }
      hash.each_key { |type|
        # Compute the average when several processus are launched
        if (hash[type][PROC] > 1) then
          hash[type][OCC] = hash[type][OCC] / hash[type][PROC]
          hash[type][TTIME] = hash[type][TTIME] / hash[type][PROC]
          hash[type][ATIME] = hash[type][ATIME] / hash[type][PROC]
          hash[type][DTIME] = hash[type][DTIME] / hash[type][PROC]
        end
      }
      completeHash["#{nb_tasks}_#{tpn}_#{mem}_#{inner_iter}"] = hash
    }
  }
}

#Write de data files
output_prefix = "output_data"
array_inner_iter.each { |workload|
  array_timers.each { |timer|
    file = File.new("#{output_prefix}_workload_#{workload}_#{timer}", "w")
    array_mem.each { |mem|
      out = ""
      tasks_per_node.each { |tpn|
        v = completeHash["#{nb_tasks}_#{tpn}_#{mem}_#{workload}"][timer][ATIME]
        out += sprintf("%.12f ", v)
      }
      file.write("#{out}\n")
    }
    file.close
  }
}

FileUtils.remove_entry_secure(cat_dir)


# Generate plots
output_plot = "output.plot"
file = File.new(output_plot, "w")
                
file.write <<EOF
set encoding iso_8859_1
set terminal postscript enhanced color font 22
set xlabel "Memory size"
set ylabel "Average time (s)"
set xtics ("8 KB" 0, "32 KB" 1, "128 KB" 2, "512 KB" 3, "1 MB" 4, "4 MB" 5, "16 MB" 6, "32 MB" 7)
set logscale y

EOF
                
array_timers.each { |timer|
  array_inner_iter.each { |workload|
    datafile = "#{output_prefix}_workload_#{workload}_#{timer}"
    file.write <<EOF
set output "graph_workload_#{workload}_#{timer}_#{graph_suffix}.eps"
plot \\
EOF
    current = 1
    color = 0
    str = String.new
    tasks_per_node.each { |tpn|
      str += "    '#{datafile}' using #{current} title \"#{tpn} tasks/node\" with linespoints pt #{current} lt #{color}"
      if (current < tasks_per_node.length) then
        str += " , \\\n"
        current += 1
        color += (color == 5) ? 2 : 1 #yellow color is unreadable
      else
        str += "\n\n"
      end
    }
    file.write(str)
  }
}
file.close

system("gnuplot #{output_plot}")
system("rm #{output_plot}")
system("rm #{output_prefix}*")

# Convert eps plots to pdf
system("for f in *.eps; do epstopdf $f;done;")
system("rm *.eps")
