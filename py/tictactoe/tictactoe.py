def print_board(board):
    """Prints the Tic Tac Toe board."""
    for i, row in enumerate(board):
        print(" | ".join(row))
        if i < len(board) - 1:
            print("-" * 9)

def check_winner(board, player):
    """Checks if the given player has won."""
    for i in range(3):
        if all(board[i][j] == player for j in range(3)):  # Rows
            return True
        if all(board[j][i] == player for j in range(3)):  # Columns
            return True
    if all(board[i][i] == player for i in range(3)):  # Diagonal \ 
        return True
    if all(board[i][2 - i] == player for i in range(3)):  # Diagonal / 
        return True
    return False

def is_full(board):
    """Checks if the board is full."""
    return all(board[i][j] != " " for i in range(3) for j in range(3))

def tic_tac_toe():
    """Main function to play the game."""
    print("Welcome to Tic Tac Toe!")
    board = [[" " for _ in range(3)] for _ in range(3)]
    current_player = "X"

    while True:
        print_board(board)
        print(f"{current_player}'s turn. Enter row and column (1, 2, or 3) separated by space:")

        try:
            row, col = map(int, input().split())
            row -= 1
            col -= 1

            if row not in range(3) or col not in range(3):
                print("Invalid input. Please enter numbers between 1 and 3.")
                continue

            if board[row][col] != " ":
                print("Cell is already taken. Try again.")
                continue

            board[row][col] = current_player

            if check_winner(board, current_player):
                print_board(board)
                print(f"Player {current_player} wins!")
                break

            if is_full(board):
                print_board(board)
                print("It's a draw!")
                break

            current_player = "O" if current_player == "X" else "X"

        except (ValueError, IndexError):
            print("Invalid input. Please enter row and column as numbers between 1 and 3.")

if __name__ == "__main__":
    tic_tac_toe()
