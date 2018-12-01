#!/usr/bin/ruby

require 'set'

numbers = File.foreach("dec1/input.txt").map {|line| line.to_i}

sum = 0
set = Set[]
looking = true

while looking
	numbers.each do |number|
		sum += number
		if set.add?(sum).nil?
			looking = false
			break
		end
	end
end

puts sum

#         0
#   -6   -6
#   +3   -3
#   +8    5    0
#   +5   10    5     +5
#   -6    4   -1     -6
#   -6   -2   -7   ]
#   +3    1   -4   ]
#   +8    9    4   ] +4
#   +5   14    9   ]
#   -6    8    3   ]
#   -6    2   -3     -6
#   +3    5    0     +3
#         =    =

#         0    4    8
#   -6   -6   -2    2
#   +3   -3   -1    5
#   +8    5    9
#   +5   10   14
#   -6    4    8

# overthinking it?
