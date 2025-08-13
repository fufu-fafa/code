#include <stdio.h>
#include <stdlib.h>

void clear_stdin() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF);
}

void print_board(int board[3][3]) {
    const char arr[3] = {' ', 'X', 'O'};

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            printf(" %c ", arr[board[y][x]]);
            if (x != 2) printf("|");
            else if (y == 2) printf("\n");
            else printf("\n---+---+---\n");
        }
    }
}

int *get_move(int board[3][3]) {
    int temp;
    int *move = malloc(2 * sizeof(int));
    if (move == NULL) {
        printf("array failed to generate\n");
        return NULL;
    }

    while (1) {
        int out = scanf("%d %d", &move[0], &move[1]);
        clear_stdin();
        if (out != 2) {
            printf("invalid input, retry (format: x y): ");
            continue;
        } else if (!(move[0] >= 1 && move[0] <= 3) ||    // check if valid
                !(move[1] >= 1 && move[1] <= 3)) {
            printf("input must range from 1-3: ");
            continue;
        }

        // formatting
        for (int x = 0; x < 2; x++) move[x]--;      // offset -1 for UX
        move[1] = 2 - move[1];                      // convert into cartesian coord
        temp = move[0];
        move[0] = move[1];
        move[1] = temp;

        if (board[move[0]][move[1]] != 0) {
            printf("part filled, retry: ");
            continue;
        }
        return move;
    }
}

int check_winner(int board[3][3]) {
    // rows & columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    // diagonals
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    return 0;
}

int main() {
    const char arr[3] = {' ', 'X', 'O'};
    int winner = 0;
    int move_num = 0;
    int tictactoe_board[3][3];
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            tictactoe_board[y][x] = 0;
        }
    }
    print_board(tictactoe_board);

    while (1) {
        if (!(move_num%2)) printf("player x move, format (x y): ");
        else printf("player y move, format (x y): ");
        int *move = get_move(tictactoe_board);
        tictactoe_board[move[0]][move[1]] = (!(move_num%2)) + 1;
        free(move);
        print_board(tictactoe_board);
        move_num++;
        winner = check_winner(tictactoe_board);
        if ((move_num == 8) && (winner == 0)) {
            printf("draw\n");
            break;
        }
        else if (winner) {
            printf("winner: %c\n", arr[winner]);
            break;
        }
    }
    return 0;
}
