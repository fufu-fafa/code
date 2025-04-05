import tkinter as tk
from tkinter import messagebox
import math

class TicTacToe:
    def __init__(self):
        self.window = tk.Tk()
        self.window.title("Tic Tac Toe")
        self.board = [[" " for _ in range(3)] for _ in range(3)]
        self.current_player = "X"
        self.ai_enabled = False
        self.buttons = [[None for _ in range(3)] for _ in range(3)]
        self.create_board()
        self.add_ai_toggle_button()

    def create_board(self):
        for row in range(3):
            for col in range(3):
                self.buttons[row][col] = tk.Button(
                    self.window,
                    text=" ",
                    font=("Arial", 24),
                    width=5,
                    height=2,
                    command=lambda r=row, c=col: self.make_move(r, c),
                )
                self.buttons[row][col].grid(row=row, column=col)

    def add_ai_toggle_button(self):
        toggle_button = tk.Button(
            self.window,
            text="Toggle AI",
            command=self.toggle_ai
        )
        toggle_button.grid(row=3, column=0, columnspan=3)

    def toggle_ai(self):
        self.ai_enabled = not self.ai_enabled
        messagebox.showinfo("AI Toggle", f"AI is now {'enabled' if self.ai_enabled else 'disabled'}.")
        if self.ai_enabled and self.current_player == "O":
            self.ai_move()

    def make_move(self, row, col):
        if self.board[row][col] == " " and not self.is_game_over():
            self.board[row][col] = self.current_player
            self.buttons[row][col].config(text=self.current_player)
            if self.check_winner(self.current_player):
                messagebox.showinfo("Game Over", f"{self.current_player} wins!")
                self.reset_board()
                return
            elif self.is_draw():
                messagebox.showinfo("Game Over", "It's a draw!")
                self.reset_board()
                return

            self.current_player = "O" if self.current_player == "X" else "X"

            if self.ai_enabled and self.current_player == "O":
                self.ai_move()

    def ai_move(self):
        best_score = -math.inf
        best_move = None

        for row in range(3):
            for col in range(3):
                if self.board[row][col] == " ":
                    self.board[row][col] = "O"
                    score = self.minimax(0, False)
                    self.board[row][col] = " "
                    if score > best_score:
                        best_score = score
                        best_move = (row, col)

        if best_move:
            row, col = best_move
            self.make_move(row, col)

    def minimax(self, depth, is_maximizing):
        if self.check_winner("O"):
            return 10 - depth
        elif self.check_winner("X"):
            return depth - 10
        elif self.is_draw():
            # Ensure draw logic accurately reflects the state of the board.
            return 0

        if is_maximizing:
            best_score = -math.inf
            for row in range(3):
                for col in range(3):
                    if self.board[row][col] == " ":
                        self.board[row][col] = "O"
                        score = self.minimax(depth + 1, False)
                        self.board[row][col] = " "
                        best_score = max(score, best_score)
            return best_score
        else:
            best_score = math.inf
            for row in range(3):
                for col in range(3):
                    if self.board[row][col] == " ":
                        self.board[row][col] = "X"
                        score = self.minimax(depth + 1, True)
                        self.board[row][col] = " "
                        best_score = min(score, best_score)
            return best_score

    def check_winner(self, player):
        for row in self.board:
            if all(cell == player for cell in row):
                return True
        for col in range(3):
            if all(self.board[row][col] == player for row in range(3)):
                return True
        if all(self.board[i][i] == player for i in range(3)) or all(self.board[i][2 - i] == player for i in range(3)):
            return True
        return False

    def is_draw(self):
        # Ensure draw logic reflects that there are no available moves.
        return all(cell != " " for row in self.board for cell in row) and not self.check_winner("X") and not self.check_winner("O")

    def is_game_over(self):
        return self.check_winner("X") or self.check_winner("O") or self.is_draw()

    def reset_board(self):
        self.board = [[" " for _ in range(3)] for _ in range(3)]
        self.current_player = "X"
        for row in range(3):
            for col in range(3):
                self.buttons[row][col].config(text=" ")

    def run(self):
        self.window.mainloop()

if __name__ == "__main__":
    game = TicTacToe()
    game.run()
