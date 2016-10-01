#!/usr/bin/env lua

soma = 0
n = 0

for l in io.lines () do
	local inc = tonumber (l)
	if inc then
		soma = soma + inc
		n = n + 1
	end
end
print (n / soma)
