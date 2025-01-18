import javax.swing.*;

public class TicTacToeAI extends TicTacToeGUI {

    public TicTacToeAI() {
        super();
    }

    // AI Logic: Minimax Implementation
    private int minimax(char[][] board, int depth, boolean isMaximizing) {
        if (checkWin(board, PLAYER_O)) {
            return 10 - depth;
        } else if (checkWin(board, PLAYER_X)) {
            return depth - 10;
        } else if (checkDraw(board)) {
            return 0;
        }

        if (isMaximizing) {
            int bestScore = Integer.MIN_VALUE;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == EMPTY) {
                        board[i][j] = PLAYER_O;
                        int score = minimax(board, depth + 1, false);
                        board[i][j] = EMPTY;
                        bestScore = Math.max(score, bestScore);
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = Integer.MAX_VALUE;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == EMPTY) {
                        board[i][j] = PLAYER_X;
                        int score = minimax(board, depth + 1, true);
                        board[i][j] = EMPTY;
                        bestScore = Math.min(score, bestScore);
                    }
                }
            }
            return bestScore;
        }
    }

    // Helper method to find the best move for AI
    private int[] findBestMove() {
        int bestScore = Integer.MIN_VALUE;
        int[] bestMove = {-1, -1};

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    int score = minimax(board, 0, false);
                    board[i][j] = EMPTY;
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = new int[]{i, j};
                    }
                }
            }
        }
        return bestMove;
    }

    // Overriding handleButtonClick to incorporate AI moves
    @Override
    protected void handleButtonClick(int row, int col) {
        if (board[row][col] == EMPTY) {
            board[row][col] = currentPlayer;
            buttons[row][col].setText(String.valueOf(currentPlayer));
            buttons[row][col].setEnabled(false);

            if (checkWin()) {
                statusLabel.setText("Player " + currentPlayer + " wins!");
                disableBoard();
                return;
            } else if (checkDraw()) {
                statusLabel.setText("It's a draw!");
                return;
            }

            switchPlayer();

            if (currentPlayer == PLAYER_O) {
                int[] bestMove = findBestMove();
                if (bestMove[0] != -1) {
                    handleButtonClick(bestMove[0], bestMove[1]);
                }
            }
        }
    }

    // Check if a player has won (for minimax)
    private boolean checkWin(char[][] board, char player) {
        for (int i = 0; i < 3; i++) {
            if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
                (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
                return true;
            }
        }
        return (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
               (board[0][2] == player && board[1][1] == player && board[2][0] == player);
    }

    // Check if the board is in a draw state (for minimax)
    private boolean checkDraw(char[][] board) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    return false;
                }
            }
        }
        return true;
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(TicTacToeAI::new);
    }
}
