import pygame
import random
import webcolors
from heapq import heappop, heappush

# Initialize pygame
pygame.init()

# Screen dimensions
width = 600
height = 400

# Colors
white = (255, 255, 255)                         # sec_backround
black = (0, 0, 0)                               # snake body
red = webcolors.hex_to_rgb("#AA4D39")           # fail msgs
green = webcolors.hex_to_rgb("#297B48")         # food
blue = webcolors.hex_to_rgb("#2D4571")          # main_backround

# Snake settings
block_size = 10
base_snake_speed = 50

# Initialize game screen
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption('Snake Game')

# Clock
clock = pygame.time.Clock()

# Font styles
font_style = pygame.font.SysFont("bahnschrift", 25)
score_font = pygame.font.SysFont("comicsansms", 35)

def your_score(score):
    value = score_font.render("Your Score: " + str(score), True, green)
    screen.blit(value, [0, 0])

def our_snake(block_size, snake_list):
    for x in snake_list:
        pygame.draw.rect(screen, black, [x[0], x[1], block_size, block_size])

def message(msg, color):
    mesg = font_style.render(msg, True, color)
    screen.blit(mesg, [width / 6, height / 3])

def is_valid_move(position, snake_list, width, height):
    return (0 <= position[0] < width and 0 <= position[1] < height and position not in snake_list)

def heuristic(a, b):
    """Manhattan distance heuristic for A*"""
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

def a_star_path(snake_head, food_position, snake_list, width, height):
    open_set = []
    heappush(open_set, (0, snake_head, []))
    visited = set()

    while open_set:
        _, current_position, path = heappop(open_set)

        if tuple(current_position) in visited:
            continue
        visited.add(tuple(current_position))

        if current_position == food_position:
            return path

        for direction in [(block_size, 0), (-block_size, 0), (0, block_size), (0, -block_size)]:
            next_position = [current_position[0] + direction[0], current_position[1] + direction[1]]
            if is_valid_move(next_position, snake_list, width, height):
                cost = len(path) + 1 + heuristic(next_position, food_position)
                heappush(open_set, (cost, next_position, path + [direction]))

    return []  # No path found

def ai_snake_direction(snake_head, food_position, snake_list, width, height, fallback_direction):
    path = a_star_path(snake_head, food_position, snake_list, width, height)

    if path:
        return path[0]  # Take the first step along the found path

    # Fallback: move safely if no path to food
    for direction in [(block_size, 0), (-block_size, 0), (0, block_size), (0, -block_size)]:
        next_position = [snake_head[0] + direction[0], snake_head[1] + direction[1]]
        if is_valid_move(next_position, snake_list, width, height):
            return direction

    return fallback_direction  # As a last resort, continue in the same direction

def gameLoop():
    game_over = False
    game_close = False

    x1 = width / 2
    y1 = height / 2

    x1_change = 0
    y1_change = 0

    snake_List = []
    Length_of_snake = 1

    foodx = round(random.randrange(0, width - block_size) / 10.0) * 10.0
    foody = round(random.randrange(0, height - block_size) / 10.0) * 10.0

    while not game_over:

        while game_close == True:
            screen.fill(blue)
            message("Press Q-Quit or C-Play Again", red)
            your_score(Length_of_snake - 1)
            pygame.display.update()

            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_q:
                        game_over = True
                        game_close = False
                    if event.key == pygame.K_c:
                        gameLoop()

        # AI controls
        x1_change, y1_change = ai_snake_direction(
            [x1, y1], [foodx, foody], snake_List, width, height, (x1_change, y1_change)
        )

        if x1 >= width or x1 < 0 or y1 >= height or y1 < 0:
            game_close = True
        x1 += x1_change
        y1 += y1_change
        screen.fill(blue)
        pygame.draw.rect(screen, green, [foodx, foody, block_size, block_size])
        snake_Head = []
        snake_Head.append(x1)
        snake_Head.append(y1)
        snake_List.append(snake_Head)
        if len(snake_List) > Length_of_snake:
            del snake_List[0]

        for x in snake_List[:-1]:
            if x == snake_Head:
                game_close = True

        our_snake(block_size, snake_List)
        your_score(Length_of_snake - 1)

        pygame.display.update()

        if x1 == foodx and y1 == foody:
            foodx = round(random.randrange(0, width - block_size) / 10.0) * 10.0
            foody = round(random.randrange(0, height - block_size) / 10.0) * 10.0
            Length_of_snake += 1

        clock.tick(base_snake_speed)

    pygame.quit()
    quit()

gameLoop()