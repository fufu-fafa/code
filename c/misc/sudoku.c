#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 9

int is_valid(int board[SIZE][SIZE], int row, int col, int num) {
    for (int n = 0; n < SIZE; n++) {
        if (board[row][n] == num || board[n][col] == num) return 0;
    }
    int start_row = row - row % 3;
    int start_col = col - col % 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[start_row + i][start_col + j] == num)
                return 0;
        }
    }
    return 1;
}

int fill_board(int board[SIZE][SIZE], int row, int col) {
    if (row == SIZE) return 1;
    if (col == SIZE) return fill_board(board, row + 1, 0);
    if (board[row][col] != 0) return fill_board(board, row, col + 1);

    int nums[SIZE];
    for (int i = 0; i < SIZE; i++) nums[i] = i + 1;

    for (int i = SIZE - 1; i > 0; i--) {
        int r = rand() % (i + 1);
        int tmp = nums[i];
        nums[i] = nums[r];
        nums[r] = tmp;
    }

    for (int i = 0; i < SIZE; i++) {
        if (is_valid(board, row, col, nums[i])) {
            board[row][col] = nums[i];
            if (fill_board(board, row, col + 1))
                return 1;
            board[row][col] = 0;
        }
    }
    return 0;
}

void print_board(int sudoku_board[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            printf("%d", sudoku_board[r][c]);
            if (c == 8) continue;
            if (c%3 == 2) printf(" ");
            else printf("|");
        }
        if (r%3 == 2) printf("\n");
        printf("\n");
    }
}

int main() {
    srand((unsigned)time(NULL));
    int sudoku_board[SIZE][SIZE] = {0};
    if (!fill_board(sudoku_board, 0, 0)) {
        fprintf(stderr, "failed to generate\n");
        return 1;
    }
    print_board(sudoku_board);
    return 0;
}
