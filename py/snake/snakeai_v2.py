import pygame
import random
import webcolors

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
snake_speed = 50

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

def ai_snake_direction(snake_head, food_position, x1_change, y1_change, snake_list):
    possible_directions = [
        (block_size, 0),   # Right
        (-block_size, 0),  # Left
        (0, block_size),   # Down
        (0, -block_size)   # Up
    ]

    best_direction = x1_change, y1_change
    min_distance = float('inf')

    for direction in possible_directions:
        new_head = [snake_head[0] + direction[0], snake_head[1] + direction[1]]

        if new_head in snake_list or new_head[0] < 0 or new_head[1] < 0 or new_head[0] >= width or new_head[1] >= height:
            continue

        distance = abs(new_head[0] - food_position[0]) + abs(new_head[1] - food_position[1])
        if distance < min_distance:
            min_distance = distance
            best_direction = direction

    return best_direction

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
        x1_change, y1_change = ai_snake_direction([x1, y1], [foodx, foody], x1_change, y1_change, snake_List)

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

        clock.tick(snake_speed)

    pygame.quit()
    quit()

gameLoop()