import tkinter as tk
from tkinter import ttk

def update_progress(value=0):
    if value <= 100:
        progress["value"] = value
        loading_window.after(50, update_progress, value + 1)  # Increase every 50ms (100 steps in 5 sec)
    else:
        loading_window.destroy()  # Close the window when done

# Create the loading window
loading_window = tk.Tk()
loading_window.title("Loading...")
loading_window.geometry("300x100")
loading_window.resizable(False, False)

# Add a label
label = tk.Label(loading_window, text="Loading, please wait...", font=("Arial", 12))
label.pack(pady=10)

# Add a progress bar
progress = ttk.Progressbar(loading_window, mode="determinate", maximum=100)
progress.pack(pady=10, padx=20, fill="x")

# Start progress update
update_progress()

loading_window.mainloop()

