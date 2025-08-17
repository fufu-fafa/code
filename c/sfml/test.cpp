#include <SFML/Graphics.hpp>

int main() {
    const int HEIGHT = 400;
    const int WIDTH = 800;
    const float xMove = 0.05;
    const int maxMove = 10000;
    int moveNum = 0;
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "sfml test");

    // circle
    sf::CircleShape circ(100.0);
    sf::Vector2f cPos(WIDTH/2.f - xMove/2.f * maxMove/2.f, HEIGHT/2.f - 75.f);
    circ.setOrigin(sf::Vector2f(circ.getRadius(), circ.getRadius()));
    circ.setFillColor(sf::Color::Green);

    // box
    sf::Vector2f bSize(100.f, 50.f);
    sf::Vector2f bPos(WIDTH/2.f + xMove/2.f * maxMove/2.f, HEIGHT/2.f + 75.f);
    sf::RectangleShape box(bSize);
    box.setOrigin(sf::Vector2f(bSize.x/2.f, bSize.y/2.f));
    box.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        if (moveNum > maxMove) {
            moveNum = 0;
        }
        moveNum++;

        if (moveNum < 5000) {
            cPos.x += xMove;
            bPos.x -= xMove;
        } else {
            cPos.x -= xMove;
            bPos.x += xMove;
        }
        circ.setPosition(cPos);
        box.setPosition(bPos);
        window.draw(circ);
        window.draw(box);
        window.display();
    }
}
