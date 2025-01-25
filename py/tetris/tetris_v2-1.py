import pygame
import random

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
        self.running = True
        self.paused = False

    def generate_tetromino(self):
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
        for y in range(20):
            for x in range(10):
                pygame.draw.rect(self.screen, self.grid[y][x], (x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE), 0)
                pygame.draw.rect(self.screen, BLACK, (x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE), 1)
                separator_x = 10 * GRID_SIZE  # Right edge of the playable grid
                pygame.draw.line(self.screen, (200, 200, 200), (separator_x, 0), (separator_x, SCREEN_HEIGHT), 2)  # Light gray line


    def draw_tetromino(self, tetromino, offset_x=0, offset_y=0):
        for y, row in enumerate(tetromino.shape):
            for x, cell in enumerate(row):
                if cell:
                    pygame.draw.rect(
                        self.screen,
                        tetromino.color,
                        ((tetromino.x + x + offset_x) * GRID_SIZE, (tetromino.y + y + offset_y) * GRID_SIZE, GRID_SIZE, GRID_SIZE),
                        0
                    )

    def draw_next_tetrominos(self):
        start_x = 10  # Offset for next blocks
        start_y = 0
        for i, tetromino in enumerate(self.next_tetrominos):
            offset_y = i * 5  # Space between blocks
            for y, row in enumerate(tetromino.shape):
                for x, cell in enumerate(row):
                    if cell:
                        pygame.draw.rect(
                            self.screen,
                            tetromino.color,
                            ((start_x + x) * GRID_SIZE, (start_y + y + offset_y) * GRID_SIZE, GRID_SIZE, GRID_SIZE),
                            0
                        )
  
    def draw_pause_menu(self):
        font = pygame.font.Font(None, 36)
        pause_text = font.render("Paused", True, WHITE)
        quit_text = font.render("Press Q to Quit", True, WHITE)
        continue_text = font.render("Press C to Continue", True, WHITE)
        self.screen.blit(pause_text, (SCREEN_WIDTH // 2 - pause_text.get_width() // 2, SCREEN_HEIGHT // 3))
        self.screen.blit(quit_text, (SCREEN_WIDTH // 2 - quit_text.get_width() // 2, SCREEN_HEIGHT // 3 + 50))
        self.screen.blit(continue_text, (SCREEN_WIDTH // 2 - continue_text.get_width() // 2, SCREEN_HEIGHT // 3 + 100))

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

            if self.paused:
                self.draw_pause_menu()
                pygame.display.flip()
                self.clock.tick(30)
                continue 

            drop_timer += self.clock.get_rawtime()
            if drop_timer > 25:
                if self.valid_move(self.current_tetromino, 0, 1):
                    self.current_tetromino.y += 1
                else:
                    self.place_tetromino(self.current_tetromino)
                    self.clear_lines()
                    self.current_tetromino = self.next_tetrominos.pop(0)
                    self.next_tetrominos.append(self.generate_tetromino())
                    if not self.valid_move(self.current_tetromino, 0, 0):
                        self.running = False
                drop_timer = 0

            self.draw_grid()
            self.draw_tetromino(self.current_tetromino)
            self.draw_next_tetrominos()
            pygame.display.flip()
            self.clock.tick(30)

        pygame.quit()

if __name__ == "__main__":
    game = TetrisGame()
    game.run()
