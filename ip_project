#version 1.14
from tkinter import *
from tkinter.ttk import Scrollbar
from tkinter.colorchooser import askcolor
from tkinter.messagebox import askyesno, showerror
from tkinter.simpledialog import askinteger
from random import choice, randint
import pandas as pd
import matplotlib.pyplot as plot

options = {} #global variable to store the options
question = '' #global variable to store the question

class pollInit:

    ''' initializing the poll, questions and options '''

    def __init__(self):
        if askyesno('Random poll', 'Do you want to generate random poll data? Press "No" to continue with poll setup.'):
            size = askinteger('Size', 'Enter the number of options:')
            low = askinteger('Lower limit', 'Enter the lower limit of random freqencies:')
            up = askinteger('Upper limit', 'Enter the upper limit of random frequencies:')

            if not size or not low or not up:
                self.setup()
            else:
                self.rand_data(size, low, up)
        else: 
            self.setup()

    #main window
    def setup(self):
        self.window = Tk()
        self.h = self.window.winfo_screenheight()
        self.w = self.window.winfo_screenwidth()
        self.window.geometry(f'{self.w}x{self.h}')
        self.window.title('Poll Setup')

        self.mainframe = Frame(self.window)
        self.mainframe.pack(fill='y', side='top')

        #frame for storing the option randiobuttons and labels
        self.opt_frame = Frame(self.mainframe)
        self.opt_frame.grid(row=2, column=0, columnspan=4)

        self.opt_var = IntVar()
        self.opt_number = 0 #used for the "Option 2:", "Option 3:" etc. labels
        self.opt_row = 0 #used for placing options in the correct row
        self.opt_list = [] #local variable used for storing the option label, option radiobutton objects 

        #question textbox
        self.text_box = Entry(self.mainframe, font=('Arial', 25), border=5, fg='grey')
        self.text_box.grid(row=0, column=0, padx=10, pady=5, sticky='NWES', columnspan=4)
        self.text_box.insert(0, 'Enter a question here')
        self.text_box.bind('<FocusIn>', self.on_click_textbox)

        #option textbox
        self.option_textbox = Entry(self.mainframe, font=('Arial', 20), border=2, fg='grey')
        self.option_textbox.grid(row=1, column=0, columnspan=3, padx=10, pady=10)
        self.option_textbox.insert(0, 'Enter an option here')
        self.option_textbox.bind('<FocusIn>', self.on_click_optbox)

        #add option button
        self.option_button = Button(self.mainframe, text='Add option', command=self.add_option, font=('Arial', 15))
        self.option_button.grid(row=1, column=3, padx=10, pady=10, sticky='NWES')

        #remove the selected option
        self.remove_opt = Button(self.mainframe, text='Remove', command=self.remove, font=('Arial', 15))
        self.remove_opt.grid(row=3, column=0, padx=5, pady=10, sticky='NWES')

        #removes all options
        self.remove_all = Button(self.mainframe, text='Remove all', command=self.delete, font=('Arial', 15))
        self.remove_all.grid(row=3, column=1, padx=5, pady=10, sticky='NWES')
        
        #continue to pollingApp
        self.confirm_button = Button(self.mainframe, text='Confirm', command=self.confirm, font=('Arial', 15))
        self.confirm_button.grid(row=3, column=2, padx=5, pady=10, sticky='NEWS')

        #change color of selected option
        self.change_color = Button(self.mainframe, text='Change color', command=self.color, font=('Arial', 15))
        self.change_color.grid(row=3, column=3, padx=5, pady=10, sticky='NWES')
        
        self.mainframe.mainloop()
        
    #function for generating random poll data
    def rand_data(size : int, lower_limit : int ,upper_limit : int):
        global question, options
        question = 'Randomly generated poll'

        for i in range(size):
            opt_color = ''
            #for geneating names such as 'A', 'B', ... ,'Z', 'AA', 'BB',...
            char = chr((i%26)+65)
            opt_name = char * (i//26+1)

            for _ in range(6): #generating a random 6-digit hexadecimal value for color
                opt_color += choice(['0','1','2','3','4','5','6','7','8','9','0','a','b','c','d','e','f'])
            options[opt_name] = [randint(lower_limit, upper_limit), f"#{opt_color}"]
    
        pollResult() #calling the result app to show the results

    #for clearing the placeholder text in question textbox
    def on_click_textbox(self, event):
        if self.text_box.cget('fg') == 'grey':
            self.text_box.delete(0, 'end') ; self.text_box.insert(0, '')
            self.text_box.config(fg='black')

    #for clearing the placeholder text in option textbox
    def on_click_optbox(self, event):
        if self.option_textbox.cget('fg') == 'grey':
            self.option_textbox.delete(0, 'end') ; self.option_textbox.insert(0, '')
            self.option_textbox.config(fg='black')

    #used to add an option
    def add_option(self):
        global options, question #this is used to change the variables on the global scope
        opt_text = self.option_textbox.get() #getting whatever is in the option textbox

        self.option_textbox.delete(0, 'end') ; self.option_textbox.insert(0, '') #clearing the textbox after option has been added

        #updating the global variables
        options[opt_text] = [0, '#000000']
        question = self.text_box.get()

        #creating and placing the radiobutton
        opt = Radiobutton(self.opt_frame, text=opt_text, variable=self.opt_var, value=self.opt_number, indicatoron=0, font=('Arial', 15))
        opt.grid(row=self.opt_row, column=3, padx=20, pady=5, columnspan=2)
        
        #creating and placing the label
        opt_label = Label(self.opt_frame, text=f'Option {self.opt_number+1}:', font=('Arial', 15))
        opt_label.grid(row=self.opt_row, column=0, padx=20, pady=5, columnspan=2)
        
        self.opt_list.append([opt, opt_label])
        self.opt_number += 1
        self.opt_row += 1

    #to associate a hex color value with the selected option
    def color(self):
        global options
        if self.opt_list: #if self.opt_list has any items
            color = askcolor()[1] #getting only the hex component

            self.opt_list[self.opt_var.get()][0].config(fg=color) #changing color of radiobutton
            self.opt_list[self.opt_var.get()][1].config(fg=color) #chnaging color of label

            options[list(options)[self.opt_var.get()]][1] = color #updating the global options

        else: #if self.opt_list has no items
            showerror('No options!', 'There are no options!')       
        
    #for removing the selected option    
    def remove(self):
        global options
        selected = self.opt_var.get() #index of the selected option
        if self.opt_list:
            del options[self.opt_list[selected][0].cget('text')] #delete selected option

            #remove selected option from the window
            for i in self.opt_list[selected]:
                i.grid_forget()
            self.opt_list.pop(selected)

            #updating the numbering of the labels
            for (num, i) in enumerate(self.opt_list):
                i[1].config(text=f'Option {num+1}:')
                i[0].config(value=num)
                self.opt_number = num+1
        else:
            showerror('No options!', 'There are no options!')

    #for removing all the options
    def delete(self):
        global options
        if askyesno('Confirm Removal', 'Do you want to remove all of the options?'):
            for i in self.opt_list: #removing options from window
                for j in i:
                    j.grid_forget()
            
            #resetting the variables
            self.opt_number = 0
            self.opt_row = 0
            self.opt_list.clear()
            options.clear()

    def confirm(self):
        if not self.opt_list: #if self.opt_list is empty
            showerror('No Options', 'There are no options!')
        elif askyesno('Confirm', 'Do you want to proceed with the polling?'):
            self.window.destroy() #closing the current window
            pollingApp() #calling the pollingApp class
             
class pollingApp:

    ''' app to take the poll '''

    def __init__(self):
        self.window = Tk()
        self.h= self.window.winfo_screenheight()
        self.w = self.window.winfo_screenwidth()
        self.window.geometry(f'{self.w}x{self.h}')
        self.window.minsize(350,350)
        self.window.title('Polling App')

        #mainframe containing all the widgets, packing it so it is centered in the window
        mainframe = Frame(self.window)
        mainframe.pack(fill='y', side='top')

        opt_frame = Frame(mainframe)
        opt_frame.grid(row=2, column=0, columnspan=2)

        self.candidate_number = 1

        self.opt_var = IntVar()
        self.toggle_var = True

        #creating and displaying the candidate number and question labels
        self.candidate_label = Label(mainframe, text=f'Candidate {self.candidate_number}', font=('Arial', 10))
        self.candidate_label.grid(row=0, column=0, columnspan=2, rowspan=1, pady=5, sticky='NWES')

        self.question_label = Label(mainframe, text=f"Q: {question}", font=('Arial', 25))
        self.question_label.grid(row=1, column=0, columnspan=2, rowspan=1, pady=10)

        for (num, i) in enumerate(options): #for displaying the options
            self.opt = Radiobutton(opt_frame, text=f'{i}', variable=self.opt_var, value=num+1, indicatoron=0, fg=options[i][1], font=('Arial', 20), bd=5)
            self.opt.grid(row=num, column=0, columnspan=2, pady=10, sticky='NWES')

        #creating and displaying the end and submit buttons
        self.end_button = Button(mainframe, text='End Poll', command=self.end, font=('Arial', 15))
        self.end_button.grid(row=3, column=1, pady=10)

        self.submit_button = Button(mainframe, text='Submit', font=('Arial', 15), command=self.submit)
        self.submit_button.grid(row=3, column=0, pady=10)

        #button for toggling the submit confirmation
        self.toggle_button = Radiobutton(mainframe, text='Submit confirmation: ON', indicatoron=0, command=self.toggle)
        self.toggle_button.grid(row=4, column=0, sticky='NWES', pady=10, columnspan=2)
           
        self.window.mainloop()

#function for toggling submit confirmation
    def toggle(self):
        self.toggle_var = not self.toggle_var
        if not self.toggle_var:
            self.toggle_button.config(text='Submit confirmation: OFF')
        else:
            self.toggle_button.config(text='Submit confirmation: ON')

#function for subbmitting the poll
    def submit(self):
        global options
        if self.opt_var.get() == 0:
            showerror('Invalid selection', 'Make a selection first')
        elif not self.toggle_var:
            self.candidate_number += 1
            options[list(options)[self.opt_var.get()-1]][0] += 1
            self.candidate_label.config(text=f'Candidate {self.candidate_number}')
        elif askyesno('Confirm submission', 'Do you want to submit? The next candidate will cast their poll after your submission.'):
            self.candidate_number += 1
            options[list(options)[self.opt_var.get()-1]][0] += 1
            self.candidate_label.config(text=f'Candidate {self.candidate_number}')

#function for ending the poll and displaying the resutls
    def end(self):
        if askyesno('End Polling', 'Do you want to end the polling and proceed with the results?'):
            self.window.destroy()
            pollResult()

class pollResult:

    ''' displays the results '''

    def __init__(self):
        self.window = Tk()
        self.h= self.window.winfo_screenheight()
        self.w = self.window.winfo_screenwidth()
        self.window.geometry(f'{self.w}x{self.h}')
        self.window.title('Poll Results')

        #frame containing the 'Poll Result' and question labels
        label_frame = Frame(self.window)
        label_frame.pack(side='top')

        #frame containing the canvas, scrollbar
        mainframe = Frame(self.window)
        mainframe.pack()

        #frame containing the buttons
        button_frame = Frame(self.window)
        button_frame.pack()
        
        #canvas containg the table with options and frequency
        self.canvas = Canvas(mainframe)
        self.canvas.pack(side=LEFT, fill=BOTH)

        scroll_bar = Scrollbar(mainframe, orient=VERTICAL, command=self.canvas.yview)
        scroll_bar.pack(side='right', fill=Y)

        #to bind the movement of scrollbar to movement of canvas
        self.canvas.configure(yscrollcommand=scroll_bar.set)
        self.canvas.bind('<Configure>', lambda e: self.canvas.configure(scrollregion=self.canvas.bbox('all')))

        #to bind the mouse scroll to scrollbar
        self.canvas.bind_all('<MouseWheel>', self.on_mouse_wheel)

        #creating a dataframe from global options dictionary    
        poll_data = pd.DataFrame(options, index=['Freqency', 'Hex color']).T
        poll_data.index.name = 'Options'

        #extracting columns from the dataframe
        self.opt_names = poll_data.index
        self.freq = poll_data['Freqency']
        self.colors = poll_data['Hex color']        

        result_label = Label(label_frame, text='Poll Results', font=('Arial', 25))
        result_label.grid(row=0, column=0, columnspan=4, pady=5)

        question_label = Label(label_frame, text=f'Q: {question}', font=('Arial', 20))
        question_label.grid(row=1, column=0, columnspan=4, pady=10)

        #frame containing the options and freqency
        table = Frame(self.canvas)
        self.canvas.create_window((0,0), window=table, anchor=NE)

        #creating and placing the column names inside table frame
        number_label = Label(table, text='S.No', font=('Arial', 15))
        number_label.grid(row=0, column=0, padx=25, pady=5)

        column_opt_label = Label(table, text='Options', font=('Arial', 15))
        column_opt_label.grid(row=0, column=1, padx=25, pady=5)

        column_freq_label = Label(table, text='Freqency', font=('Arial', 15))
        column_freq_label.grid(row=0, column=2, padx=25, pady=5)

        for (i, opt) in enumerate(options): #for displaying the options from the global variable

            opt_number = Label(table, text=f'{i+1}.', font=('Arial', 16))
            opt_number.grid(row=i+1, column=0)

            opt_label = Label(table, text=opt, fg=options[opt][1], font=('Arial', 16), border=5)
            opt_label.grid(row=i+1, column=1)

            freq_label = Label(table, text=options[opt][0], font=('Arial', 16))
            freq_label.grid(row=i+1, column=2)

        #creating and displaying the buttons for the different graphs
        bar_button = Button(button_frame, text='Bar graph', command=self.bar, font=('Arial', 10))
        bar_button.grid(row=0, column=0, padx=5, pady=5)

        pie_button = Button(button_frame, text='Pie graph', command=self.pie, font=('Arial', 10))
        pie_button.grid(row=0, column=1, padx=5, pady=5)

        line_button = Button(button_frame, text='Line graph', command=self.line, font=('Arial',10))
        line_button.grid(row=0, column=2, padx=5, pady=5)

        scatter_button = Button(button_frame, text='Scatter graph', command=self.scatter, font=('Arial', 10))
        scatter_button.grid(row=0, column=3, padx=5, pady=5)

        self.window.mainloop()

    #function for when mouse scroll wheel is scrolled
    def on_mouse_wheel(self ,event):
        self.canvas.yview_scroll(-1*int((event.delta/120)), 'units')

    #functions for displaying different charts
    def pie(self):
        plot.close()
        plot.axis('equal')
        plot.pie(self.freq, labels=self.opt_names, autopct='%1.1f%%', colors=self.colors)
        plot.show()
    
    def bar(self):
        plot.close()
        plot.bar(self.opt_names, self.freq, color=self.colors)
        plot.xlabel('Options') ; plot.ylabel('Number of people')
        plot.show()
    
    def line(self):
        plot.close()
        plot.plot(self.opt_names, self.freq, color=self.colors.iloc[1])
        plot.xlabel('Options') ; plot.ylabel('Number of people')
        plot.show()

    def scatter(self):
        plot.close()
        plot.scatter(self.opt_names, self.freq, color=self.colors)
        plot.xlabel('Options') ; plot.ylabel('Number of people')
        plot.show()

pollInit.rand_data(size=100, lower_limit=1, upper_limit=10) #calling the initializer app to start the program
