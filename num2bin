# version: 0.2
n = int(input('Enter a number: '))
x = []
p = 0
while True:
    if n <= 2**p:
        break
    p+=1
   
for i in range(p,-1,-1):
    n -= 2**i
    if n < 0:
        n += 2**i
        x.append(0)
    else:
        x.append(1)
for i in x:
    print(i,end='')
