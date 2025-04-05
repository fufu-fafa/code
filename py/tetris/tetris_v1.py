import pygame
import random

# Initialize pygame
pygame.init()

# Screen dimensions
SCREEN_WIDTH = 300
SCREEN_HEIGHT = 600
GRID_SIZE = 30

# Colors
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
        self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
        self.clock = pygame.time.Clock()
        self.grid = [[BLACK for _ in range(10)] for _ in range(20)]
        self.current_tetromino = self.new_tetromino()
        self.running = True

    def new_tetromino(self):
        shape = random.choice(SHAPES)
        color = random.choice(COLORS)
        return Tetromino(shape, color)

    def valid_move(self, tetromino, dx, dy):
        for y, row in enumerate(tetromino.shape):
            for x, cell in enumerate(row):
                if cell:
                    new_x = tetromino.x + x + dx
                    new_y = tetromino.y + y + dy
                    if new_x < 0 or new_x >= 10 or new_y >= 20 or (new_y >= 0 and self.grid[new_y][new_x] != BLACK):
                        return False
        return True

    def place_tetromino(self, tetromino):
        for y, row in enumerate(tetromino.shape):
            for x, cell in enumerate(row):
                if cell:
                    self.grid[tetromino.y + y][tetromino.x + x] = tetromino.color

    def clear_lines(self):
        new_grid = [row for row in self.grid if any(cell == BLACK for cell in row)]
        lines_cleared = 20 - len(new_grid)
        for _ in range(lines_cleared):
            new_grid.insert(0, [BLACK for _ in range(10)])
        self.grid = new_grid

    def draw_grid(self):
        for y, row in enumerate(self.grid):
            for x, cell in enumerate(row):
                pygame.draw.rect(self.screen, cell, (x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE))
                pygame.draw.rect(self.screen, BLACK, (x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE), 1)

    def draw_tetromino(self, tetromino):
        for y, row in enumerate(tetromino.shape):
            for x, cell in enumerate(row):
                if cell:
                    pygame.draw.rect(self.screen, tetromino.color, ((tetromino.x + x) * GRID_SIZE, (tetromino.y + y) * GRID_SIZE, GRID_SIZE, GRID_SIZE))

    def run(self):
        drop_timer = 0
        while self.running:
            self.screen.fill(BLACK)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                elif event.type == pygame.KEYDOWN:
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

            drop_timer += self.clock.get_rawtime()
            if drop_timer > 25:
                if self.valid_move(self.current_tetromino, 0, 1):
                    self.current_tetromino.y += 1
                else:
                    self.place_tetromino(self.current_tetromino)
                    self.clear_lines()
                    self.current_tetromino = self.new_tetromino()
                    if not self.valid_move(self.current_tetromino, 0, 0):
                        self.running = False
                drop_timer = 0

            self.draw_grid()
            self.draw_tetromino(self.current_tetromino)
            pygame.display.flip()
            self.clock.tick(30)

        pygame.quit()

if __name__ == "__main__":
    game = TetrisGame()
    game.run()
