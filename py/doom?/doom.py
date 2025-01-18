import pygame
import math
import sys

# Initialize Pygame
pygame.init()

# Screen dimensions
WIDTH, HEIGHT = 800, 600
TILE_SIZE = 40

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GRAY = (150, 150, 150)

# Create the screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("DOOM-like Raycasting")

# Clock for controlling frame rate
clock = pygame.time.Clock()

# Map layout (0 = empty space, 1 = wall)
MAP = [
    [1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 1, 1, 0, 0, 1],
    [1, 0, 1, 0, 1, 0, 0, 1],
    [1, 0, 1, 0, 1, 0, 1, 1],
    [1, 0, 0, 0, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1],
]

# Player starting position
player_x, player_y = TILE_SIZE * 3, TILE_SIZE * 3
player_angle = 0
player_speed = 2
rotation_speed = 0.05

# FOV (Field of View)
FOV = math.pi / 3  # 60 degrees in radians
MAX_DEPTH = 800
NUM_RAYS = WIDTH // 2  # Number of rays to cast

def draw_map():
    """Draw the map on the screen (for debugging)."""
    for row in range(len(MAP)):
        for col in range(len(MAP[row])):
            x = col * TILE_SIZE
            y = row * TILE_SIZE
            color = WHITE if MAP[row][col] == 1 else BLACK
            pygame.draw.rect(screen, color, (x, y, TILE_SIZE, TILE_SIZE))

def cast_ray(angle):
    """Cast a ray from the player's position and return the distance to the wall."""
    # Ray start position
    start_x, start_y = player_x, player_y

    # Direction vector for ray
    dir_x = math.cos(angle)
    dir_y = math.sin(angle)

    # Distance
    distance = 0

    while distance < MAX_DEPTH:
        # Check where the ray hits on the grid
        x = int(start_x + dir_x * distance)
        y = int(start_y + dir_y * distance)

        # Check if the ray hits a wall (MAP value 1)
        if MAP[y // TILE_SIZE][x // TILE_SIZE] == 1:
            return distance
        distance += 1

    return MAX_DEPTH

def draw_3d_view():
    """Cast rays and draw the walls."""
    for ray in range(NUM_RAYS):
        # Calculate the ray's angle (spread across the field of view)
        ray_angle = player_angle - FOV / 2 + (ray / NUM_RAYS) * FOV
        distance = cast_ray(ray_angle)

        # Calculate the height of the wall on the screen based on the distance
        wall_height = min(HEIGHT / distance * 100, HEIGHT)

        # Calculate the position where the wall should be drawn
        x = ray * 2
        pygame.draw.rect(screen, GRAY, (x, HEIGHT // 2 - wall_height // 2, 2, wall_height))

# Game loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Handle player movement and rotation
    keys = pygame.key.get_pressed()
    if keys[pygame.K_w]:  # Move forward
        player_x += player_speed * math.cos(player_angle)
        player_y += player_speed * math.sin(player_angle)
    if keys[pygame.K_s]:  # Move backward
        player_x -= player_speed * math.cos(player_angle)
        player_y -= player_speed * math.sin(player_angle)
    if keys[pygame.K_a]:  # Turn left
        player_angle -= rotation_speed
    if keys[pygame.K_d]:  # Turn right
        player_angle += rotation_speed

    # Fill screen
    screen.fill(BLACK)

    # Draw the 3D view (walls)
    draw_3d_view()

    # Draw the map (for debugging)
    # draw_map()

    # Update display
    pygame.display.flip()

    # Cap the frame rate
    clock.tick(60)
