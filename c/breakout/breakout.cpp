#include <SFML/Graphics.hpp>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PADDLE_WIDTH = 100.f;
const float PADDLE_HEIGHT = 20.f;
const float BALL_RADIUS = 10.f;
const float BRICK_WIDTH = 60.f;
const float BRICK_HEIGHT = 20.f;
const int BRICK_ROWS = 5;
const int BRICK_COLUMNS = 10;

class Brick {
public:
    sf::RectangleShape shape;
    bool isDestroyed;

    Brick(float x, float y) {
        shape.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
        isDestroyed = false;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Breakout Game");

    // Paddle
    sf::RectangleShape paddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    paddle.setFillColor(sf::Color::White);
    paddle.setPosition((WINDOW_WIDTH - PADDLE_WIDTH) / 2, WINDOW_HEIGHT - 50);

    // Ball
    sf::CircleShape ball(BALL_RADIUS);
    ball.setFillColor(sf::Color::Red);
    ball.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    sf::Vector2f ballVelocity(0.1f, 0.1f);

    // Bricks
    std::vector<Brick> bricks;
    for (int i = 0; i < BRICK_ROWS; ++i) {
        for (int j = 0; j < BRICK_COLUMNS; ++j) {
            bricks.emplace_back(j * (BRICK_WIDTH + 10) + 50, i * (BRICK_HEIGHT + 10) + 50);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Paddle movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 0) {
            paddle.move(-0.2f, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x + PADDLE_WIDTH < WINDOW_WIDTH) {
            paddle.move(0.2f, 0.f);
        }

        // Ball movement
        ball.move(ballVelocity);

        // Ball collision with walls
        if (ball.getPosition().x < 0 || ball.getPosition().x + BALL_RADIUS * 2 > WINDOW_WIDTH) {
            ballVelocity.x = -ballVelocity.x;
        }
        if (ball.getPosition().y < 0) {
            ballVelocity.y = -ballVelocity.y;
        }

        // Ball collision with paddle
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballVelocity.y = -ballVelocity.y;
        }

        // Ball collision with bricks
        for (auto& brick : bricks) {
            if (!brick.isDestroyed && ball.getGlobalBounds().intersects(brick.shape.getGlobalBounds())) {
                brick.isDestroyed = true;
                ballVelocity.y = -ballVelocity.y;
            }
        }

        // Check if ball is out of bounds (game over)
        if (ball.getPosition().y + BALL_RADIUS * 2 > WINDOW_HEIGHT) {
            window.close();
        }

        // Clear the window
        window.clear();

        // Draw the paddle
        window.draw(paddle);

        // Draw the ball
        window.draw(ball);

        // Draw the bricks
        for (const auto& brick : bricks) {
            if (!brick.isDestroyed) {
                window.draw(brick.shape);
            }
        }

        // Display the window
        window.display();
    }

    return 0;
}