import pygame
import random
import os

# Initialize pygame
pygame.init()

# Screen dimensions
SCREEN_WIDTH = 300
SCREEN_HEIGHT = 600
GRID_SIZE = 30

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
CYAN = (0, 255, 255)
MAGENTA = (255, 0, 255)
YELLOW = (255, 255, 0)
ORANGE = (255, 165, 0)

COLORS = [RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, ORANGE]

# Tetromino shapes
SHAPES = [
    [[1, 1, 1],
     [0, 1, 0]],

    [[0, 1, 1],
     [1, 1, 0]],

    [[1, 1, 0],
     [0, 1, 1]],

    [[1, 1],
     [1, 1]],

    [[1, 1, 1, 1]],

    [[1, 1, 1],
     [1, 0, 0]],

    [[1, 1, 1],
     [0, 0, 1]]
]

class Tetromino:
    def __init__(self, shape, color):
        self.shape = shape
        self.color = color
        self.x = 3
        self.y = 0

    def rotate(self):
        self.shape = [list(row) for row in zip(*self.shape[::-1])]

class TetrisGame:
    def __init__(self):
        self.screen = pygame.display.set_mode((SCREEN_WIDTH + 150, SCREEN_HEIGHT))  # Extra space for next blocks
        self.clock = pygame.time.Clock()
        self.grid = [[BLACK for _ in range(10)] for _ in range(20)]
        self.current_tetromino = self.generate_tetromino()
        self.next_tetrominos = [self.generate_tetromino() for _ in range(3)]  # Queue for the next three blocks
        self.hold_tetromino = None  # Store the held Tetromino
        self.hold_used = False  # Prevent multiple holds in one drop cycle
        self.running = True
        self.paused = False

    def hold_current_tetromino(self):
        if not self.hold_used:  # Allow hold only once per drop cycle
            if self.hold_tetromino is None:
                # If no Tetromino is held, store the current Tetromino and spawn a new one
                self.hold_tetromino = self.current_tetromino
                self.current_tetromino = self.next_tetrominos.pop(0)
                self.next_tetrominos.append(self.generate_tetromino())
            else:
                # Swap the held Tetromino with the current one
                self.hold_tetromino, self.current_tetromino = self.current_tetromino, self.hold_tetromino
                self.current_tetromino.x, self.current_tetromino.y = 3, 0  # Reset position
            self.hold_used = True

    def draw_hold_tetromino(self):
        if self.hold_tetromino:
            start_x = -5  # Position to the left of the play area
            start_y = 0
            for y, row in enumerate(self.hold_tetromino.shape):
                for x, cell in enumerate(row):
                    if cell:
                        pygame.draw.rect(
                            self.screen,
                            self.hold_tetromino.color,
                            ((start_x + x) * GRID_SIZE, (start_y + y) * GRID_SIZE, GRID_SIZE, GRID_SIZE),
                            0
                        )

    def run(self):
        drop_timer = 0
        while self.running:
            self.screen.fill(BLACK)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_p:  # Pause the game
                        self.paused = not self.paused
                    elif self.paused:
                        if event.key == pygame.K_q:  # Quit from pause
                            self.running = False
                        elif event.key == pygame.K_c:  # Continue from pause
                            self.paused = False
                    else:
                        if event.key == pygame.K_LEFT and self.valid_move(self.current_tetromino, -1, 0):
                            self.current_tetromino.x -= 1
                        elif event.key == pygame.K_RIGHT and self.valid_move(self.current_tetromino, 1, 0):
                            self.current_tetromino.x += 1
                        elif event.key == pygame.K_DOWN and self.valid_move(self.current_tetromino, 0, 1):
                            self.current_tetromino.y += 1
                        elif event.key == pygame.K_UP:
                            self.current_tetromino.rotate()
                            if not self.valid_move(self.current_tetromino, 0, 0):
                                self.current_tetromino.rotate()
                                self.current_tetromino.rotate()
                                self.current_tetromino.rotate()
                        elif event.key == pygame.K_LSHIFT:  # Hold key
                            self.hold_current_tetromino()

            # Skip the game logic if paused
            if not self.paused:
                drop_timer += self.clock.get_rawtime()
                if drop_timer > 25:
                    if self.valid_move(self.current_tetromino, 0, 1):
                        self.current_tetromino.y += 1
                    else:
                        self.place_tetromino(self.current_tetromino)
                        self.clear_lines()
                        self.current_tetromino = self.next_tetrominos.pop(0)
                        self.next_tetrominos.append(self.generate_tetromino())
                        self.hold_used = False  # Reset hold flag after Tetromino locks
                        if not self.valid_move(self.current_tetromino, 0, 0):
                            self.running = False
                    drop_timer = 0

            self.draw_grid()
            self.draw_tetromino(self.current_tetromino)
            self.draw_next_tetrominos()
            self.draw_hold_tetromino()
            if self.paused:
                self.draw_pause_menu()
            pygame.display.flip()
            self.clock.tick(30)

        pygame.quit()
        print("closed")


if __name__ == "__main__":
    game = TetrisGame()
    game.run()