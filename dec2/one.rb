#!/usr/bin/ruby

require 'set'

strings = File.readlines("dec2/input.txt")

twos = 0
threes = 0

strings.each do |word|
	prev = '-'
	count = 0
	is2 = false
	is3 = false
	word.strip.chars.sort.each do |cur|
		if cur == prev
			count += 1
		else
			if count == 2
				is2 = true
			elsif count == 3
				is3 = true
			end
			count = 1
		end
		prev = cur
	end
	if count == 2
		is2 = true
	elsif count == 3
		is3 = true
	end
	if is2
		twos += 1
	end
	if is3
		threes += 1
	end
end

puts twos
puts threes
puts twos * threes
