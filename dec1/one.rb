#!/usr/bin/ruby

sum = 0

File.open("dec1/input.txt").each do |line|
	sum += line.to_i
end

puts sum
