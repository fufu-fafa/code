#include <SFML/Graphics.hpp>
#include <sstream>

std::string numToStr(float num) {
    std::ostringstream temp;
    temp << std::fixed << std::setprecision(2) << num;
    return temp.str();
}

int detectCollision(float x1, float x2, float rad1, float rad2) {
    float dist = std::abs(x1 - x2) - (rad1 + rad2);
    if (dist < 0.2f) return 1;
    else return 0;
}

int detectEdgeColl(float speed, float x, float rad, const float WIDTH) {
    int lEdgeColl = 0.2f > (x - rad);
    int rEdgeColl = 0.2f > (WIDTH - (x + rad));
    if ((lEdgeColl && (speed < 0.f)) || (rEdgeColl && (speed > 0.f))) return 1;
    else return 0;
}

int main() {
    sf::Font font;
    if (!font.openFromFile("SpaceMono-Regular.ttf")) return 1;
    std::string textStr;

    const int HEIGHT = 400;
    const int WIDTH = 800;
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "sfml test");
    const float rads[2] = {25.f, 25.f};
    float speeds[2] = {-120.f, 200.f};
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
    float fps;
    float dt;
    int coll;
    int edgeColls[2];
    sf::Clock clock;
    sf::Clock clock2;
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        dt = clock.restart().asSeconds();

        if (clock2.getElapsedTime().asSeconds() >= 1.0f) {
            fps = 1.0f / dt;
            textStr = " FPS: " + numToStr(fps);
            clock2.restart();
        }
        sf::Text text(font, textStr, 32);
        window.draw(text);

        for (int n = 0; n < 2; n++) {
            edgeColls[n] = detectEdgeColl(speeds[n], poss[n].x, rads[n], WIDTH);
            if (edgeColls[n]) speeds[n] *= -1;
        }

        coll = detectCollision(poss[0].x, poss[1].x, rads[0], rads[1]);
        if (coll) {
            temp = speeds[0];
            speeds[0] = speeds[1];
            speeds[1] = temp;
        }

        for (int n = 0; n < 2; n++) {
            poss[n].x += speeds[n] * dt;
            circles[n].setPosition(poss[n]);
            window.draw(circles[n]);
        }
        window.display();
    }
}
