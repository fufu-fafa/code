import tkinter as tk
from tkinter import messagebox
import numpy as np

def create_board():
    return np.zeros((6, 7), dtype=int)

def is_valid_move(board, col):
    return board[5, col] == 0

def get_next_open_row(board, col):
    for row in range(6):
        if board[row, col] == 0:
            return row

def drop_piece(board, row, col, piece):
    board[row, col] = piece

def check_win(board, piece):
    # Check horizontal locations
    for row in range(6):
        for col in range(4):
            if np.all(board[row, col:col+4] == piece):
                return True

    # Check vertical locations
    for col in range(7):
        for row in range(3):
            if np.all(board[row:row+4, col] == piece):
                return True

    # Check positively sloped diagonals
    for row in range(3):
        for col in range(4):
            if all(board[row+i, col+i] == piece for i in range(4)):
                return True

    # Check negatively sloped diagonals
    for row in range(3):
        for col in range(4):
            if all(board[row+3-i, col+i] == piece for i in range(4)):
                return True

    return False

class Connect4GUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Connect 4")

        self.board = create_board()
        self.turn = 0

        self.canvas = tk.Canvas(self.root, width=700, height=600, bg="#147df5")
        self.canvas.pack()

        self.draw_board()

        self.root.bind("<Button-1>", self.handle_click)

    def draw_board(self):
        self.canvas.delete("all")
        for row in range(6):
            for col in range(7):
                x0 = col * 100
                y0 = (5 - row) * 100
                x1 = x0 + 100
                y1 = y0 + 100
                color = "#001524"
                if self.board[row, col] == 1:
                    color = "#E00000"
                elif self.board[row, col] == 2:
                    color = "#D9FA00"
                self.canvas.create_oval(x0 + 10, y0 + 10, x1 - 10, y1 - 10, fill=color, outline="#147df5")

    def handle_click(self, event):
        col = event.x // 100
        if col < 0 or col > 6:
            return

        if not is_valid_move(self.board, col):
            messagebox.showinfo("Invalid Move", "Column is full. Choose another column.")
            return

        row = get_next_open_row(self.board, col)
        piece = 1 if self.turn % 2 == 0 else 2
        drop_piece(self.board, row, col, piece)

        if check_win(self.board, piece):
            self.draw_board()
            winner = "Player 1" if piece == 1 else "Player 2"
            messagebox.showinfo("Game Over", f"{winner} wins!")
            self.root.quit()

        self.turn += 1
        self.draw_board()

if __name__ == "__main__":
    root = tk.Tk()
    game = Connect4GUI(root)
    root.mainloop()
