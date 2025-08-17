#include <SFML/Graphics.hpp>

int detectCollision(float x1, float x2, float rad1, float rad2) {
    float dist = std::abs(x1 - x2) - (rad1 + rad2);
    if (dist < 0.2f) return 1;
    else return 0;
}

int detectEdgeColl(float x, float rad, const float WIDTH) {
    int lEdgeColl = 0.2f > (x - rad);
    int rEdgeColl = 0.2f > (WIDTH - (x + rad));
    if (lEdgeColl || rEdgeColl) return 1;
    else return 0;
}

int main() {
    const int HEIGHT = 400;
    const int WIDTH = 800;
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "sfml test");

    // circle1
    const float c1Rad = 25.f;
    float c1Speed = 0.05;
    sf::CircleShape c1(c1Rad);
    sf::Vector2f c1Pos(WIDTH/2.f - 300.f, HEIGHT/2.f);
    c1.setOrigin(sf::Vector2f(c1.getRadius(), c1.getRadius()));
    c1.setPosition(c1Pos);
    c1.setFillColor(sf::Color::Green);

    // circle2
    const float c2Rad = 25.f;
    float c2Speed = -0.07;
    sf::CircleShape c2(c2Rad);
    sf::Vector2f c2Pos(WIDTH/2.f + 300.f, HEIGHT/2.f);
    c2.setOrigin(sf::Vector2f(c2.getRadius(), c2.getRadius()));
    c2.setPosition(c2Pos);
    c2.setFillColor(sf::Color::Red);

    float temp;
    int coll, c1EdgeColl, c2EdgeColl;
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        coll = detectCollision(c1Pos.x, c2Pos.x, c1Rad, c2Rad);
        c1EdgeColl = detectEdgeColl(c1Pos.x, c1Rad, WIDTH);
        c2EdgeColl = detectEdgeColl(c2Pos.x, c2Rad, WIDTH);

        if (c1EdgeColl) c1Speed *= -1;
        if (c2EdgeColl) c2Speed *= -1;

        if (detectCollision(c1Pos.x, c2Pos.x, c1Rad, c2Rad)) {
            temp = c1Speed;
            c1Speed = c2Speed;
            c2Speed = temp;
        }

        c1Pos.x += c1Speed;
        c2Pos.x += c2Speed;

        c1.setPosition(c1Pos);
        c2.setPosition(c2Pos);
        window.draw(c1);
        window.draw(c2);
        window.display();
    }
}
