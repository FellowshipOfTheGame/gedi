soma = 0
n = 0

for l in io.lines () do
	soma = soma + tonumber (l)
	n = n + 1
end
print (soma / n)
