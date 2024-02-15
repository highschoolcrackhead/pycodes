# i dont know how to explain it but it is working, type anything other than number in the while loop to stop
# version: 0.2
a = []
i = 1
while True:
    try:
        n = int(input(f'Enter the number of blocks in column {i}: '))
        i += 1
        if 0 <= n:
            a.append(n)
    except:
        break
p = 0
q = 0
x =[]
for i in range(len(a)):
    if p < a[i]:
        p = a[i]
    if q < a[((i*-1)-1)]:
        q = a[((i*-1)-1)]
    x.append([p,q])
for i in range(len(a)):
    if x[i][0] < x[i][1]:
        x.append(x[i][0])
    else:
        x.append(x[i][1])
del x[0:(len(a))]
for i in range(len(a)):
    x.append((x[i])-(a[i]))   
del x[0:(len(a))]
print(sum(x))
