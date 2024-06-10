# works with whole numbers, postive and negative
# version: 0.4
try:
    n = int(input('Enter a number: '))
except:
    print("Invalid Input!")
negative = False
if n < 0:
    negative = True
    n *= -1
for i in list(str(n)):
    if i in ['2','3','4','5','6','7','8','9']:
        print('Non binary number')
            break
    else:
        x = [j for j in str(n)[::-1]]
        num = 0
        for k in range(len(x)):
            num += (int(x[k])*(2**k))
if negative:
    print(num*-1)
else:
    print(num)
