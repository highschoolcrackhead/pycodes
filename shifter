# for roatating a string either left or right
# version: 0.2

s = str(input('Enter something:'))
a=list(s)
a.remove(' ')

ch = int(input('Enter 1 for right shift, 2 for left shift: '))

if ch in [1,2]:
    n = int(input('Enter the no. of terms to be shifted: '))
    if ch ==1:
        for i in range(n):
            a.append(a[0])
            a.pop(0)
    elif ch == 2:
        for i in range(n):
            a.insert(0,a[-1])
            a.pop(-1)
else:
    print('error')
    
print(a)
