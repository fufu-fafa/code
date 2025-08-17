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
    const float rads[2] = {25.f, 25.f};
    float speeds[2] = {0.05f, -0.07f};
    sf::Vector2f poss[2];
    sf::CircleShape circles[2];

    for (int n = 0; n < 2; n++) {
        poss[n] = {WIDTH/2.f - ((n-1.f) * 600.f + 300.f), HEIGHT/2.f};
        circles[n].setRadius(rads[n]);
        circles[n].setOrigin(sf::Vector2f(rads[n], rads[n]));
        circles[n].setPosition(poss[n]);
    }

    circles[0].setFillColor(sf::Color::Blue);
    circles[1].setFillColor(sf::Color::Red);

    float temp;
    int coll;
    int edgeColls[2];
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);

        for (int n = 0; n < 2; n++) {
            edgeColls[n] = detectEdgeColl(poss[n].x, rads[n], WIDTH);
            if (edgeColls[n]) speeds[n] *= -1;
        }

        coll = detectCollision(poss[0].x, poss[1].x, rads[0], rads[1]);
        if (coll) {
            temp = speeds[0];
            speeds[0] = speeds[1];
            speeds[1] = temp;
        }

        for (int n = 0; n < 2; n++) {
            poss[n].x += speeds[n];
            circles[n].setPosition(poss[n]);
            window.draw(circles[n]);
        }
        window.display();
    }
}
