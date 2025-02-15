import pygame
import random
import heapq
from collections import deque

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
DIRECTIONS = [UP, DOWN, LEFT, RIGHT]

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
            print(len(game.snake))
            self.running = False
            return
        
        self.snake.insert(0, head)
        if head == self.food:
            self.food = self.spawn_food()
        else:
            self.snake.pop()

    def flood_fill_check(self, start):
        """Check if the snake has enough open space to survive."""
        queue = deque([start])
        visited = set()
        count = 0
        
        while queue:
            node = queue.popleft()
            if node in visited or node in self.snake:
                continue
            
            visited.add(node)
            count += 1
            
            for direction in DIRECTIONS:
                neighbor = (node[0] + direction[0], node[1] + direction[1])
                if 0 <= neighbor[0] < WIDTH // GRID_SIZE and 0 <= neighbor[1] < HEIGHT // GRID_SIZE:
                    queue.append(neighbor)
        
        return count >= len(self.snake)  # Ensure enough space for the snake to move
    
    def a_star_pathfind(self, start, goal):
        """A* pathfinding algorithm to find the shortest path to food."""
        def heuristic(a, b):
            return abs(a[0] - b[0]) + abs(a[1] - b[1])
        
        open_list = []
        heapq.heappush(open_list, (0, start))
        came_from = {}
        g_score = {start: 0}
        f_score = {start: heuristic(start, goal)}
        
        while open_list:
            _, current = heapq.heappop(open_list)
            
            if current == goal:
                path = []
                while current in came_from:
                    path.append(current)
                    current = came_from[current]
                path.reverse()
                return path
            
            for direction in DIRECTIONS:
                neighbor = (current[0] + direction[0], current[1] + direction[1])
                if neighbor in self.snake or not (0 <= neighbor[0] < WIDTH // GRID_SIZE and 0 <= neighbor[1] < HEIGHT // GRID_SIZE):
                    continue
                
                tentative_g_score = g_score[current] + 1
                if neighbor not in g_score or tentative_g_score < g_score[neighbor]:
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g_score
                    f_score[neighbor] = tentative_g_score + heuristic(neighbor, goal)
                    heapq.heappush(open_list, (f_score[neighbor], neighbor))
        
        return []
    
    def ai_move(self):
        path = self.a_star_pathfind(self.snake[0], self.food)
        if path:
            next_pos = path[0]
            if self.flood_fill_check(next_pos):
                self.direction = (next_pos[0] - self.snake[0][0], next_pos[1] - self.snake[0][1])
        else:
            # No path found, enter "wait mode" by making a safe move
            for direction in DIRECTIONS:
                next_pos = (self.snake[0][0] + direction[0], self.snake[0][1] + direction[1])
                if next_pos not in self.snake and 0 <= next_pos[0] < WIDTH // GRID_SIZE and 0 <= next_pos[1] < HEIGHT // GRID_SIZE:
                    self.direction = direction
                    break

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