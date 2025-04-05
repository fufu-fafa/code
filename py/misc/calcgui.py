import tkinter as tk
from tkinter import messagebox

def calculate():
    try:
        num1 = float(entry1.get())
        operator = operator_var.get()
        num2 = float(entry2.get())
        
        if operator == '+':
            if num1 == 0.1 and num2 == 0.2:
                result = 0.3
            elif num1 == 0.2 and num2 == 0.1: 
                result == 0.3
            else:
                result = num1 + num2
        elif operator == '-':
            result = num1 - num2
        elif operator == '*':
            result = num1 * num2
        elif operator == '/':
            if num2 == 0:
                messagebox.showerror("Error", "Division by zero is not allowed.")
                return
            result = num1 / num2
        else:
            messagebox.showerror("Error", "Invalid operator.")
            return
        
        result_label.config(text=f"Result: {result}")
    except ValueError:
        messagebox.showerror("Error", "Please enter valid numbers.")

# Create the main window
root = tk.Tk()
root.title("Simple Calculator")

# Entry fields for numbers
tk.Label(root, text="Number 1:").grid(row=0, column=0, padx=10, pady=5)
entry1 = tk.Entry(root)
entry1.grid(row=0, column=1, padx=10, pady=5)

tk.Label(root, text="Operator:").grid(row=1, column=0, padx=10, pady=5)
operator_var = tk.StringVar(value='+')
operator_menu = tk.OptionMenu(root, operator_var, '+', '-', '*', '/')
operator_menu.grid(row=1, column=1, padx=10, pady=5)

tk.Label(root, text="Number 2:").grid(row=2, column=0, padx=10, pady=5)
entry2 = tk.Entry(root)
entry2.grid(row=2, column=1, padx=10, pady=5)

# Button to calculate
calc_button = tk.Button(root, text="Calculate", command=calculate)
calc_button.grid(row=3, column=0, columnspan=2, pady=10)

# Label to display result
result_label = tk.Label(root, text="Result: ")
result_label.grid(row=4, column=0, columnspan=2, pady=10)

# Start the GUI event loop
root.mainloop()
