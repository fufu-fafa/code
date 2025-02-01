#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 10;
const int BALL_SIZE = 10;
const int BRICK_ROWS = 5;
const int BRICK_COLS = 10;
const int BRICK_WIDTH = 75;
const int BRICK_HEIGHT = 20;

struct Brick {
    SDL_Rect rect;
    bool active;
};

class BreakoutGame {
public:
    BreakoutGame();
    ~BreakoutGame();
    bool init();
    void run();

private:
    void processInput();
    void update();
    void render();
    void resetBall();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect paddle, ball;
    int ballVelX, ballVelY;
    std::vector<Brick> bricks;
    bool running;
};

BreakoutGame::BreakoutGame() : window(nullptr), renderer(nullptr), running(true) {
    paddle = {SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT};
    ball = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BALL_SIZE, BALL_SIZE};
    ballVelX = 4;
    ballVelY = -4;
    
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            bricks.push_back({{col * (BRICK_WIDTH + 5) + 35, row * (BRICK_HEIGHT + 5) + 50, BRICK_WIDTH, BRICK_HEIGHT}, true});
        }
    }
}

BreakoutGame::~BreakoutGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool BreakoutGame::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return renderer != nullptr;
}

void BreakoutGame::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
    }
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_LEFT] && paddle.x > 0) paddle.x -= 8;
    if (keys[SDL_SCANCODE_RIGHT] && paddle.x < SCREEN_WIDTH - PADDLE_WIDTH) paddle.x += 8;
}

void BreakoutGame::update() {
    ball.x += ballVelX;
    ball.y += ballVelY;
    
    if (ball.x <= 0 || ball.x + BALL_SIZE >= SCREEN_WIDTH) ballVelX = -ballVelX;
    if (ball.y <= 0) ballVelY = -ballVelY;
    if (ball.y + BALL_SIZE >= SCREEN_HEIGHT) resetBall();
    
    if (SDL_HasIntersection(&ball, &paddle)) ballVelY = -ballVelY;
    
    for (auto& brick : bricks) {
        if (brick.active && SDL_HasIntersection(&ball, &brick.rect)) {
            brick.active = false;
            ballVelY = -ballVelY;
            break;
        }
    }
}

void BreakoutGame::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &paddle);
    SDL_RenderFillRect(renderer, &ball);
    
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    for (const auto& brick : bricks) {
        if (brick.active) SDL_RenderFillRect(renderer, &brick.rect);
    }
    
    SDL_RenderPresent(renderer);
}

void BreakoutGame::resetBall() {
    ball = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BALL_SIZE, BALL_SIZE};
    ballVelX = 4;
    ballVelY = -4;
}

void BreakoutGame::run() {
    while (running) {
        processInput();
        update();
        render();
        SDL_Delay(16);
    }
}

int main() {
    BreakoutGame game;
    if (!game.init()) return 1;
    game.run();
    return 0;
}
