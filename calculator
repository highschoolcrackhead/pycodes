# very basic calculator with tkinter, probably inefficient <skull>
# version 0.3

import tkinter as tk

eq = ''

def addEq(symbol):
    global eq
    eq += str(symbol)
    result.delete(1.0, 'end')
    result.insert(1.0, eq)

def evalEq():
    global eq
    try :
        calc = str(eval(eq))
        eq = ''
        result.delete(1.0, 'end')
        result.insert(1.0, calc)
    except Exception:
        clear()
        result.insert(1.0, 'Error')

def backEq():
    global eq
    eq = eq[:-1]
    result.delete(1.0, 'end')
    result.insert(1.0, eq)

def clear():
    global eq
    eq = ''
    result.delete(1.0, 'end')

root = tk.Tk()
root.geometry('300x300')

result = tk.Text(root, height=2, width=16, font=('Arial', 24))
result.grid(columnspan=5)

btn1 = tk.Button(root, text='1', command=lambda : addEq(1), width=5)
btn1.grid(row=2, column=1)

btn2 = tk.Button(root, text='2', command=lambda: addEq(2), width=5)
btn2.grid(row=2, column=2)

btn3 = tk.Button(root, text='3', command=lambda: addEq(3), width=5)
btn3.grid(row=2, column=3)

btnAdd = tk.Button(root, text='+', command=lambda: addEq('+'), width=5)
btnAdd.grid(row=2, column=4)

btn4 = tk.Button(root, text='4', command=lambda: addEq(4), width=5)
btn4.grid(row=3, column=1)

btn5 = tk.Button(root, text='5', command=lambda: addEq(5), width=5)
btn5.grid(row=3, column=2)

btn6 = tk.Button(root, text='6', command=lambda: addEq(6), width=5)
btn6.grid(row=3, column=3)

btnMin = tk.Button(root, text='-', command=lambda: addEq('-'), width=5)
btnMin.grid(row=3, column=4)

btn7 = tk.Button(root, text='7', command=lambda: addEq(7), width=5)
btn7.grid(row=4, column=1)

btn8 = tk.Button(root, text='8', command=lambda: addEq(8), width=5)
btn8.grid(row=4, column=2)

btn9 = tk.Button(root, text='9', command=lambda: addEq(9), width=5)
btn9.grid(row=4, column=3)

btnDiv = tk.Button(root, text='/', command=lambda: addEq('/'), width=5)
btnDiv.grid(row=4, column=4)

btn0 = tk.Button(root, text='0', command=lambda: addEq(0), width=5)
btn0.grid(row=5, column=1)

btnBrac1 = tk.Button(root, text='(', command=lambda: addEq('('), width=5)
btnBrac1.grid(row=5, column=2)

btnBrac2 = tk.Button(root, text=')', command=lambda: addEq(')'), width=5)
btnBrac2.grid(row=5, column=3)

btnMul = tk.Button(root, text='*', command=lambda: addEq('*'), width=5)
btnMul.grid(row=5, column=4)

btnAc = tk.Button(root, text='AC', command=lambda: clear(), width=5)
btnAc.grid(row=7, column=1)

btnClr = tk.Button(root, text='<-', command=lambda: backEq(), width=5)
btnClr.grid(row=7, column=2)

btnDot = tk.Button(root, text='.', command=lambda: addEq('.'), width=5)
btnDot.grid(row=7, column=3)

btnEqu = tk.Button(root, text='=', command=lambda: evalEq(), width=5)
btnEqu.grid(row=7, column=4)

btnSq = tk.Button(root, text='x²', command=lambda: addEq('**2'), width=5)
btnSq.grid(row = 6, column=1)

btnSqx = tk.Button(root, text='xʸ', command=lambda: addEq('**'), width=5)
btnSqx.grid(row = 6, column=2)

btnSqrt = tk.Button(root, text='√x', command=lambda: addEq('**(1/2)'), width=5)
btnSqrt.grid(row=6, column=3)

btnSqrtx = tk.Button(root, text='ʸ√x', command=lambda: addEq('**(1/'), width=5)
btnSqrtx.grid(row=6, column=4)

root.mainloop()
