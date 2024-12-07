#version 1.5

from tabulate import tabulate
from PIL import Image
import qrcode as qr
import mysql.connector as sql

database = sql.connect(host='localhost', user='root', password='Pravar@123')
cursor = database.cursor()
admin_pin = 123

cursor.execute('create database if not exists inventory;')
cursor.execute('create database if not exists cart;')

cursor.execute('create table if not exists inventory.product (ID int primary key, Name varchar(25), Brand varchar(25), Quantity int, Price float);')

cursor.execute('create table if not exists inventory.customer (MobileNo bigint(10) primary key, FName varchar(15), LName varchar(15), CustID varchar(36), PIN int(4) default 9999, CartEmpty bool default True);')        

print('Welcome to Inventory Manager')

def ask_choice(choice_list : list, question : str = "Enter choice", sql_table : bool = False, check_choice : bool = True) -> int:
    ch = 0
    n = len(choice_list)
    if sql_table:
        heads = [i[0] for i in cursor.description]
        id = [i[0] for i in choice_list]
    else:
        ch_no = [f'{i}.' for i in range(n)]

    while True:

        print('__'*15, '\n')
        
        if not sql_table:
            print(tabulate({1:ch_no, 2:choice_list}, tablefmt='pretty'))
        else :
            print(tabulate(choice_list, tablefmt='pretty', headers=heads))

        if not check_choice:
            x = input(f'{question}: ')
            ch = x if x else 0
            break
        else:
            try:
                ch = abs(int(input(f'{question}: ')))
                if (sql_table) and (ch not in id):
                    print('\n','Invalid choice!')
                elif ch not in range(n):
                    print('\n','Invalid choice!')
                else: break
            except ValueError:
                print('\n', 'Invalid choice!')
    return ch

def homepage():
    ch = ask_choice(['Exit', 'Admin', 'Customer'])
    match ch:
        case 0:
            print('__'*15, '\n')
            print('Thank you for using the inventory manager')
            print('__'*15)
            exit()
        case 1: Admin()
        case 2: Customer()
    
class Admin:

    def __init__(self):
        pin = ask_choice(['Go Back'], 'Enter PIN', check_choice=False)
        print(pin)
        
        if pin == str(admin_pin): self.main()
        elif pin == '0': homepage()
        else: Admin()
 
    def main(self):

        opt_list = ['Go Back', 'Add Item', 'Update Item', 'Delete Item', 'Display all items', 'Check low stock Items', 'Check Customer details']
        ch = ask_choice(opt_list)

        match ch:
            case 0: homepage()
            case 1: self.add_item()
            case 2: self.update_item()
            case 3: self.delete_item()
            case 4: self.display()
            case 5: self.low_stock()
            case 6: self.cust_details()

    def add_item(self):
        print('__'*15, '\n')
        print(tabulate({1:['0.'], 2:['Go Back']}, tablefmt='pretty'))

        try:
            id = int(input('Enter Product ID: '))
            name = "'" + input('Enter Product Name: ') + "'"
            brand = "'" + input('Enter Product Brand: ') + "'"
            qty = abs(int(input('Enter Quantity: ')))
            price = abs(float(input('Enter Price: ')))
        except Exception as e:
            print('__'*15, '\n')
            print(e)
            self.add_item()

        if id == 0 or name == '0' or brand == '0' or qty == 0 or price == 0:
            self.main()
        else:

            try:
                cursor.execute(f"insert into inventory.product values ({id}, {name}, {brand}, {qty}, {price});")
                database.commit()
            except Exception as e:
                print('__'*15, '\n')
                print(e)
                self.main()
            else:
                database.commit()
                print('__'*15, '\n')
                print(f'Successfully added {name} to Inventory')
                opt_list = ['Go Back', 'Add more item']
                ch = ask_choice(opt_list)

                match ch:
                    case 0: self.main()
                    case 1: self.add_item()
                    
    def update_item(self):
        cursor.execute('select * from inventory.product;')
        print('__'*15, '\n')
        print('Leave BLANK for no change')
        ch = ask_choice(cursor.fetchall(), 'Enter Product ID', True)
        cursor.execute(f'select * from inventory.product where ID = {ch};')

        original = [i for i in cursor.fetchall()[0]]
        updated = [input(f'Enter new {i}: ') for i in ['ID', 'Name', 'Brand', 'Qty', 'Price']]
        
        for i in range(len(updated)):
            if not updated[i]:
                updated[i] = original[i]
            else:
                match i:
                    case 0: updated[0] = int(updated[0])
                    case 3: updated[3] = abs(int(updated[3]))
                    case 4: updated[4] = abs(float(updated[4]))

        updated[1] = "'" + updated[1] + "'"
        updated[2] = "'" + updated[2] + "'"

        try:
            cursor.execute(f"update inventory.product set ID = {updated[0]}, Name = {updated[1]}, Brand = {updated[2]}, Quantity = {updated[3]}, Price = {updated[4]} where ID = {ch};")
            database.commit()       
        except Exception as e:
            print('\n', '__'*15, '\n')
            print(e)
            self.update_item()

        ch = ask_choice(['Go back', 'Update more items'])

        match ch:
            case 0: self.main()
            case 1: self.update_item()

    def delete_item(self):

        cursor.execute('select * from inventory.product;')
        ch = ask_choice(cursor.fetchall(), 'Enter Product ID (leave BLANK to cancel)', True, False)
        if not ch or ch == 0:
            self.main()
        else:
            try:
                cursor.execute(f'delete from inventory.product where ID = {int(ch)}')
            except Exception as e:
                print('__'*15, '\n')
                print(e)
                self.delete_item()

        ch1 = ask_choice(['Go back', 'Delete more items'])

        match ch1:
            case 0: self.main()
            case 1: self.delete_item()

    def display(self):
        cursor.execute('select * from inventory.product;')
        ask_choice(cursor.fetchall(), 'Press ENTER to continue', True, False)
        self.main()

    def low_stock(self):
        cursor.execute('select * from inventory.product where quantity < 200;')
        ask_choice(cursor.fetchall(), "Press ENTER to continue", True, False)
        self.main()

    def cust_details(self):
        cursor.execute('select MobileNo, concat(Fname, " ", Lname) as "Name", if(CartEmpty, "True", "False") as "Empty Cart" from inventory.customer;')
        ask_choice(cursor.fetchall(), 'Press ENTER to continue', True, False)
        self.main()

class Customer:

    def __init__(self):

        cursor.execute('select MobileNo from inventory.customer;')
        data = cursor.fetchall()
        mob_list = [i[0] for i in data] if data else []

        self.mob = int(input('Enter your Mobile Number: '))
        if len(str(self.mob)) != 10:
            print('__'*15, '\n')
            print('Mobile Number must be of 10 digits!')
            Customer()
        else:
            if self.mob in mob_list:
                cursor.execute(f'select custid, PIN from inventory.customer where mobileno = {self.mob};')
                data = cursor.fetchall()
                cust_pin = data[0][1]
                self.custID = data[0][0]
        
                pin = int(input('Enter your PIN: '))
                if pin != cust_pin:
                    print('__'*15, '\n')
                    print('Incorrect PIN!')
                    print('__'*15, '\n')
                    Customer()

            else:
                fname = input('Enter your First name: ') 
                lname = input('Enter your Last name: ') 
                pin = int(input('Create your 4-digit PIN: '))
                self.custID = fname + '_' + str(self.mob)
                cursor.execute(f'insert into inventory.customer value ({self.mob}, "{fname}", "{lname}", "{self.custID}", {pin}, True);')
                database.commit()

        cursor.execute(f'select fname from inventory.customer where mobileno = {self.mob};')
        cust_name = cursor.fetchall()[0][0]

        print('__'*15, '\n')
        print(f'Welcome {cust_name}!')
        self.main()

    def main(self):

        opt_list = ['Go Back', 'Buy Items', 'View available items', 'View/Edit Cart', 'Payment', 'Contact Us']
        ch = ask_choice(opt_list)

        match ch:
            case 0: homepage()
            case 1: self.buy_item()
            case 2: self.view_item()
            case 3: self.view_cart()
            case 4: self.payment()
            case 5: self.feedback()

    def buy_item(self):
        
        cursor.execute('select * from inventory.product;')
        ch = ask_choice(cursor.fetchall(), 'Enter Product ID', True)

        cursor.execute(f'select price, name, quantity from inventory.product where ID = {ch};')
        data = cursor.fetchall()
        price = data[0][0]
        name = data[0][1]
        stock = data[0][2]

        qty = abs(int(input('Enter Quantity: ')))

        if (qty != 0) and (qty <= stock):
            
            print('__'*15, '\n')
            print(f'Total amount: {qty*price} INR')
            
            ch2 = ask_choice(['No' ,'Yes'], 'Do you want to proceed')
            match ch2:
                case 0: self.main()
                case 1:
                    cursor.execute(f'create table if not exists cart.{self.custID} (ItemID int, Product varchar(25), Quantity int not null, Amount float, foreign key(ItemID) references inventory.product(ID));')
                    cursor.execute(f'insert into cart.{self.custID} value ({ch}, "{name}", {qty}, {qty*price});')
                    cursor.execute(f'update inventory.customer set cartempty = 0 where mobileno = {self.mob};')
                    print('__'*15, '\n')
                    print(f'Succesfully added {name} to Cart!')
                    ch3 = ask_choice(['Go Back', 'Purchase more items'])

                    match ch3:
                        case 0: self.main()
                        case 1: self.buy_item()
        else:
            print('__'*15, "\n")
            print('Insufficient Quantity!') 
            self.buy_item()

    def view_item(self):
        cursor.execute('select * from inventory.product;')
        ask_choice(cursor.fetchall(), 'Press ENTER to continue', True, False)
        self.main()

    def view_cart(self):
        
        cursor.execute(f'select cartempty from inventory.customer where mobileno = {self.mob};')
        empty = cursor.fetchall()[0][0]
        print(empty)
        if empty:
            print('__'*15, '\n')
            print('You have no items in Cart!')
            self.main()
        cursor.execute(f'select * from cart.{self.custID}')
        ch1 = ask_choice(cursor.fetchall(), 'Enter Product ID', True, True)
        ch2 = ask_choice(['Cancel', 'Change Quantity', 'Remove Item'], 'Select Operation')

        match ch2:
            case 0: self.main()
            case 1:
                cursor.execute(f'select price from inventory.product where id = {ch1};')
                price = cursor.fetchall()[0][0]
                qty = abs(int(input('Enter new quantity: ')))
                print('__'*15, '\n')
                print(f'New Amount: {qty*price} INR')
                ch3 = ask_choice(['No' ,'Yes'], 'Do you want to proceed')

                match ch3:
                    case 0: self.main()
                    case 1:
                        cursor.execute(f'update cart.{self.custID} set quantity = {qty},amount = {qty*price} where itemid = {ch1};')
                        database.commit()
                        print('__'*15, '\n')
                        print('Succesfully updated Cart!')
                        self.main()

    def payment(self):
        cursor.execute(f'select cartempty from inventory.customer where mobileno = {self.mob};')
        empty = cursor.fetchall()[0][0]
        if empty:
            print('__'*15, '\n')
            print('You have no items in Cart!')
            self.main()
        
        opt_list = ['Go Back', 'Cash', 'UPI', 'scan QR Code', 'Credit/Debit Card']
        ch = ask_choice(opt_list, 'Select mode of payment')

        match ch:
            case 0: self.main()
            case 1:
                print('__'*15, '\n')
                print('Please pay at checkout and collect your items')
            case 2: 
                UPI_info = [input(f'Enter UPI {i}: ') for i in ['ID', 'Password']]
                print('__'*15, '\n')
                print('Payment succesfull, please collect your items at the counter')
            case 3:
                print('__'*15, '\n')
                print('Please scan the QR code')
                self.scan()
            case 4:
                card_info = [input(f'Enter {i}:') for i in ['Card Number', 'Name on card', 'expiry date', 'CVV']]
                print('__'*15, '\n')
                print('Payment succesfull, please collect your items at the counter')

        self.checkout()   

    def scan(self):
        cursor.execute(f'select sum(Amount) from cart.{self.custID};')
        am = cursor.fetchall()[0][0]
        text = f'upi://pay?pa=8218178871@fam&pn=Shopkeeper&am={am}'

        code = qr.make(text)
        code.save('qrCode.img')
        img = Image.open('qrCode.img')
        img.show()

        self.checkout()

    def checkout(self):

        cursor.execute(f'select itemid, quantity from cart.{self.custID};')
        data = cursor.fetchall()

        for i in data:
            cursor.execute(f'update inventory.product set quantity = quantity - {i[1]} where ID = {i[0]};')
            database.commit()
        
        cursor.execute(f'drop table cart.{self.custID};')
        cursor.execute(f"update inventory.customer set cartempty = 1 where custid = '{self.custID}'")
        database.commit()

        self.main()

    def feedback(self):

        print('__'*15, '\n')
        print('You can reach us at:')
        print('__'*15, '\n')
        print(tabulate([['Email:', 'xyz@gmail.com'], ['Mobile:', '1234567890'], ['Facebook:', 'xyz'], ['Instagram:', 'xyz_shop'], ['Telegram:', 'XYZshop']], tablefmt='pretty'))

        input('Press ENTER to continue')
        self.main()

if __name__ == "__main__":
    homepage()
    database.commit()
    database.close()
