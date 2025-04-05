import numpy as np

def create_board():
    return np.zeros((6, 7), dtype=int)

def print_board(board):
    print(np.flip(board, 0))

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

def play_game():
    board = create_board()
    game_over = False
    turn = 0

    print("Welcome to Connect 4!")
    print_board(board)

    while not game_over:
        # Player 1's turn
        if turn % 2 == 0:
            valid_input = False
            while not valid_input:
                try:
                    col = int(input("Player 1, choose a column (1-7): ")) - 1
                    if col < 0 or col > 6:
                        raise ValueError

                    if is_valid_move(board, col):
                        row = get_next_open_row(board, col)
                        drop_piece(board, row, col, 1)
                        valid_input = True

                        if check_win(board, 1):
                            print("Player 1 wins!")
                            game_over = True
                    else:
                        print("Column is full. Choose another column.")
                
                except ValueError:
                    print("Invalid input. Please choose a column between 1 and 7.")

        # Player 2's turn
        else:
            try:
                col = int(input("Player 2, choose a column (1-7): ")) - 1
                if col < 0 or col > 6:
                    raise ValueError

                if is_valid_move(board, col):
                    row = get_next_open_row(board, col)
                    drop_piece(board, row, col, 2)

                    if check_win(board, 2):
                        print("Player 2 wins!")
                        game_over = True
                else:
                    print("Column is full. Choose another column.")

            except ValueError:
                print("Invalid input. Please choose a column between 1 and 7.")

        print_board(board)

        turn += 1

        if turn == 42 and not game_over:
            print("It's a draw!")
            game_over = True

if __name__ == "__main__":
    play_game()
