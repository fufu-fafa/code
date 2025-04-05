import pygame
import random
import numpy as np

# Initialize pygame
pygame.init()

# Constants
WIDTH, HEIGHT = 500, 500
GRID_SIZE = 20
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLACK = (0, 0, 0)

# Directions (x, y)
UP = (0, -1)
DOWN = (0, 1)
LEFT = (-1, 0)
RIGHT = (1, 0)

# Game screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()

# Snake and Food
class SnakeGame:
    def __init__(self):
        self.snake = [(5, 5)]
        self.food = self.spawn_food()
        self.direction = RIGHT
        self.running = True

    def spawn_food(self):
        while True:
            food = (random.randint(0, WIDTH // GRID_SIZE - 1),
                    random.randint(0, HEIGHT // GRID_SIZE - 1))
            if food not in self.snake:
                return food

    def move(self):
        head = (self.snake[0][0] + self.direction[0], self.snake[0][1] + self.direction[1])
        
        # Check collision
        if (head in self.snake or head[0] < 0 or head[1] < 0 or head[0] >= WIDTH // GRID_SIZE or head[1] >= HEIGHT // GRID_SIZE):
            self.running = False
            return
        
        self.snake.insert(0, head)
        if head == self.food:
            self.food = self.spawn_food()
        else:
            self.snake.pop()

    def ai_move(self):
        head = self.snake[0]
        food = self.food
        
        # Simple greedy AI: Move towards food if possible
        if food[0] > head[0]:
            next_move = RIGHT
        elif food[0] < head[0]:
            next_move = LEFT
        elif food[1] > head[1]:
            next_move = DOWN
        else:
            next_move = UP
        
        # Prevent collisions
        next_pos = (head[0] + next_move[0], head[1] + next_move[1])
        if next_pos in self.snake or next_pos[0] < 0 or next_pos[1] < 0 or next_pos[0] >= WIDTH // GRID_SIZE or next_pos[1] >= HEIGHT // GRID_SIZE:
            for move in [UP, DOWN, LEFT, RIGHT]:
                next_pos = (head[0] + move[0], head[1] + move[1])
                if next_pos not in self.snake and 0 <= next_pos[0] < WIDTH // GRID_SIZE and 0 <= next_pos[1] < HEIGHT // GRID_SIZE:
                    self.direction = move
                    return
        else:
            self.direction = next_move

    def draw(self):
        screen.fill(BLACK)
        for segment in self.snake:
            pygame.draw.rect(screen, GREEN, (segment[0] * GRID_SIZE, segment[1] * GRID_SIZE, GRID_SIZE, GRID_SIZE))
        pygame.draw.rect(screen, RED, (self.food[0] * GRID_SIZE, self.food[1] * GRID_SIZE, GRID_SIZE, GRID_SIZE))
        pygame.display.flip()

# Main loop
game = SnakeGame()
while game.running:
    pygame.event.pump()
    game.ai_move()
    game.move()
    game.draw()
    clock.tick(10)

pygame.quit()
