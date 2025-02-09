import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Random;

public class BreakoutGame extends JPanel implements KeyListener, Runnable {
    private static final int WIDTH = 800;
    private static final int HEIGHT = 600;
    private static final int PADDLE_WIDTH = 100;
    private static final int PADDLE_HEIGHT = 20;
    private static final int BALL_SIZE = 20;
    private static final int BRICK_WIDTH = 80;
    private static final int BRICK_HEIGHT = 30;
    private static final int BRICK_ROWS = 5;
    private static final int BRICK_COLS = 10;

    private int paddleX = WIDTH / 2 - PADDLE_WIDTH / 2;
    private int ballX = WIDTH / 2 - BALL_SIZE / 2; // Start in the middle
    private int ballY = HEIGHT / 2 - BALL_SIZE / 2; // Start in the middle
    private int ballSpeedX;
    private int ballSpeedY;
    private int remainingBricks = BRICK_COLS * BRICK_ROWS;

    private boolean[][] bricks = new boolean[BRICK_ROWS][BRICK_COLS];
    private boolean paddleMovingLeft = false;
    private boolean paddleMovingRight = false;
    private boolean gameWon = false;
    private Random random = new Random();

    // Define colors
    private final Color[] brickColors = {
        new Color(87, 132, 230), // Blue
        new Color(202, 66, 62),  // Red
        new Color(235, 196, 68), // Yellow
        new Color(60, 133, 75)   // Green
    };
    private final Color[] brickEdgeColors = {
        new Color(75,112,195), // Blue edges
        new Color(171, 57, 52),  // Red edges
        new Color(199, 166, 58), // Yellow edges
        new Color(50, 111, 63)   // Green edges
    };

    public BreakoutGame() {
        setPreferredSize(new Dimension(WIDTH, HEIGHT));
        setBackground(Color.BLACK);
        setFocusable(true);
        addKeyListener(this);

        // Initialize bricks
        for (int i = 0; i < BRICK_ROWS; i++) {
            for (int j = 0; j < BRICK_COLS; j++) {
                bricks[i][j] = true;
            }
        }

        // Initialize ball with random direction
        randomizeBallDirection();
    }

    private void randomizeBallDirection() {
        // Randomize the ball's initial direction
        double angle; // Random angle in radians
        int speed = 11; // Base speed

        do {
            angle = random.nextDouble() * (Math.PI / 2) + Math.PI / 4;
            ballSpeedX = (int) (speed * Math.cos(angle));
        } while (Math.abs(ballSpeedX) < 2);   
        ballSpeedY = -(int) (speed * Math.sin(angle));
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        if (gameWon) {
            g.setColor(Color.GREEN);
            g.setFont(new Font("Arial", Font.BOLD, 40));
            String winMessage = "Game Won!";
            int textWidth = g.getFontMetrics().stringWidth(winMessage);
            g.drawString(winMessage, (WIDTH - textWidth) / 2, HEIGHT / 2);
            return; // Stop drawing other elements
        }

        int edgeWidth = PADDLE_WIDTH / 5;

        // Left side
        g.setColor(Color.GRAY);
        g.fillRect(paddleX, HEIGHT - 50, edgeWidth, PADDLE_HEIGHT);

        // Middle
        g.setColor(Color.WHITE);
        g.fillRect(paddleX + edgeWidth, HEIGHT - 50, PADDLE_WIDTH - 2 * edgeWidth, PADDLE_HEIGHT);

        // Right side
        g.setColor(Color.GRAY);
        g.fillRect(paddleX + PADDLE_WIDTH - edgeWidth, HEIGHT - 50, edgeWidth, PADDLE_HEIGHT);

        // Draw ball
        g.setColor(Color.WHITE);
        g.fillOval(ballX, ballY, BALL_SIZE, BALL_SIZE);

        // Draw bricks
        for (int i = 0; i < BRICK_ROWS; i++) {
            for (int j = 0; j < BRICK_COLS; j++) {
                if (bricks[i][j]) {
                    // Alternate colors for each row
                    int colorIndex = i % brickColors.length;
                    g.setColor(brickColors[colorIndex]);
                    g.fillRect(j * BRICK_WIDTH, i * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT);

                    // Draw edges
                    g.setColor(brickEdgeColors[colorIndex]);
                    g.drawRect(j * BRICK_WIDTH, i * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT);
                }
            }
        }
    }

    @Override
    public void keyPressed(KeyEvent e) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_LEFT) {
            paddleMovingLeft = true;
        }
        if (key == KeyEvent.VK_RIGHT) {
            paddleMovingRight = true;
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        int key = e.getKeyCode();
        if (key == KeyEvent.VK_LEFT) {
            paddleMovingLeft = false;
        }
        if (key == KeyEvent.VK_RIGHT) {
            paddleMovingRight = false;
        }
    }

    @Override
    public void keyTyped(KeyEvent e) {}

    @Override
    public void run() {
        while (true) {
            if (!gameWon) {
                movePaddle();
                moveBall();
                checkCollisions();
                repaint();
            }

            try {
                Thread.sleep(16); // ~60 FPS
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private void movePaddle() {
        if (paddleMovingLeft && paddleX > 0) {
            paddleX -= 10;
        }
        if (paddleMovingRight && paddleX < WIDTH - PADDLE_WIDTH) {
            paddleX += 10;
        }
    }

    private void moveBall() {
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Ball collision with walls
        if (ballX <= 0 || ballX >= WIDTH - BALL_SIZE) {
            ballSpeedX = -ballSpeedX;
        }
        if (ballY <= 0) {
            ballSpeedY = -ballSpeedY;
        }

        // Ball collision with paddle
        if (ballY + BALL_SIZE >= HEIGHT - 50 && ballX + BALL_SIZE >= paddleX && ballX <= paddleX + PADDLE_WIDTH) {
            ballSpeedY = -ballSpeedY; // Always reverse Y-direction
            
            // Calculate hit position (-1: far left, 0: center, 1: far right)
            double hitPosition = ((ballX + BALL_SIZE / 2) - (paddleX + PADDLE_WIDTH / 2)) / (PADDLE_WIDTH / 2);
        
            // Only adjust ballSpeedX if it's outside the center range (e.g., ±0.3 allows slight variation)
            if (Math.abs(hitPosition) > 0.3) {
                ballSpeedX += (int) (hitPosition * 4); // Adjust X speed based on hit location
            }
        
            // Prevent ball from going completely horizontal
            if (Math.abs(ballSpeedX) < 4) {
                ballSpeedX = ballSpeedX > 0 ? 4 : -4;
            }
        }

        // Ball out of bounds (respawn in the middle with random direction)
        if (ballY >= HEIGHT - BALL_SIZE) {
            respawnBall();
        }
    }

    private void respawnBall() {
        ballX = WIDTH / 2 - BALL_SIZE / 2; // Center horizontally
        ballY = HEIGHT / 2 - BALL_SIZE / 2; // Center vertically
        randomizeBallDirection(); // Randomize direction
    }

    private void checkCollisions() {
        for (int i = 0; i < BRICK_ROWS; i++) {
            for (int j = 0; j < BRICK_COLS; j++) {
                if (bricks[i][j]) {
                    int brickX = j * BRICK_WIDTH;
                    int brickY = i * BRICK_HEIGHT;
    
                    if (ballX + BALL_SIZE > brickX && ballX < brickX + BRICK_WIDTH &&
                        ballY + BALL_SIZE > brickY && ballY < brickY + BRICK_HEIGHT) {
                        
                        // Calculate distances to each edge
                        int fromLeft = Math.abs(ballX + BALL_SIZE - brickX);
                        int fromRight = Math.abs(ballX - (brickX + BRICK_WIDTH));
                        int fromTop = Math.abs(ballY + BALL_SIZE - brickY);
                        int fromBottom = Math.abs(ballY - (brickY + BRICK_HEIGHT));
    
                        // Find the smallest penetration (the likely collision direction)
                        int minDistance = Math.min(Math.min(fromLeft, fromRight), Math.min(fromTop, fromBottom));
    
                        if (minDistance == fromLeft || minDistance == fromRight) {
                            ballSpeedX = -ballSpeedX; // Reverse X direction
                        } else {
                            ballSpeedY = -ballSpeedY; // Reverse Y direction
                        }
    
                        bricks[i][j] = false; // Remove brick
                        remainingBricks--;

                        if (remainingBricks == 0) {
                            gameWon = true;
                        }
                        return; // Exit loop to prevent multiple collisions per frame
                    }
                }
            }
        }
    }    

    public static void main(String[] args) {
        JFrame frame = new JFrame("Breakout Game");
        BreakoutGame game = new BreakoutGame();
        frame.add(game);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        new Thread(game).start();
    }
}