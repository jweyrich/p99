#!/usr/bin/ruby

if (ARGV.length != 1) then
  puts "Wrong number of arguments"
  puts "Usage: #{__FILE__} NB_TASKS"
  exit 1
end
max_nodes = ARGV[0].to_i
puts "digraph G {"

global_list = (0..(max_nodes - 1)).to_a

global_list.each { |elt|
  tmp = global_list.clone
  tmp.delete_at(elt)
  tmp.each { |i|
    puts "#{elt} -> #{i}"
  }
}
global_list.each { |elt|
  puts "#{elt} [label=\"#{elt}-#{elt}\"]"
}

puts "}"

exit 0
