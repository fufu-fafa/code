#include <SFML/Graphics.hpp>
#include <sstream>
#include <random>
#include <cmath>

struct circ {
    sf::CircleShape shape;
    sf::Vector2f spd;
    sf::Vector2f pos;
    float rad;
};

std::string numToStr(float num) {
    std::ostringstream temp;
    temp << std::fixed << std::setprecision(2) << num;
    return temp.str();
}

int genRandInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

void handleCircleColl(circ &circle1, circ &circle2) {
    float temp, dist, distChange, velProj1, velProj2, perpVelProj1, perpVelProj2;
    float relVel[2], relVec[2], norVec[2], perpNorVec[2];

    relVel[0] = circle2.spd.x - circle1.spd.x;
    relVel[1] = circle2.spd.y - circle1.spd.y;

    relVec[0] = circle2.pos.x - circle1.pos.x;
    relVec[1] = circle2.pos.y - circle1.pos.y;

    // skip if separating
    distChange = relVel[0] * relVec[0] + relVel[1] * relVec[1];
    if (distChange > 0.f) return;

    dist = std::sqrt(relVec[0]*relVec[0] + relVec[1]*relVec[1]);
    if (dist > (circle1.rad + circle2.rad + 5)) return;

    // circle velocity projection along collision vector
    norVec[0] = relVec[0]/dist;
    norVec[1] = relVec[1]/dist;
    velProj1 = circle1.spd.x * norVec[0] + circle1.spd.y * norVec[1];
    velProj2 = circle2.spd.x * norVec[0] + circle2.spd.y * norVec[1];

    // swap after collision
    temp = velProj1;
    velProj1 = velProj2;
    velProj2 = temp;

    // perpendicular circle velocity projection along collision vector
    perpNorVec[0] = -norVec[1];
    perpNorVec[1] = norVec[0];
    perpVelProj1 = circle1.spd.x * perpNorVec[0] + circle1.spd.y * perpNorVec[1];
    perpVelProj2 = circle2.spd.x * perpNorVec[0] + circle2.spd.y * perpNorVec[1];

    circle1.spd.x = velProj1 * norVec[0] + perpVelProj1 * perpNorVec[0];
    circle1.spd.y = velProj1 * norVec[1] + perpVelProj1 * perpNorVec[1];
    circle2.spd.x = velProj2 * norVec[0] + perpVelProj2 * perpNorVec[0];
    circle2.spd.y = velProj2 * norVec[1] + perpVelProj2 * perpNorVec[1];
}

void handleEdgeColl(sf::Vector2f &spd, sf::Vector2f &pos, float rad, const int WIDTH, const int HEIGHT) {
    int lEdgeColl = 0.2f > (pos.x - rad);
    int rEdgeColl = 0.2f > (WIDTH - (pos.x + rad));
    int tEdgeColl = 0.2f > (pos.y - rad);
    int bEdgeColl = 0.2f > (HEIGHT - (pos.y + rad));
    if ((lEdgeColl && (spd.x < 0.f)) || (rEdgeColl && (spd.x > 0.f))) spd.x *= -1;
    if ((tEdgeColl && (spd.y < 0.f)) || (bEdgeColl && (spd.y > 0.f))) spd.y *= -1;
}

void randSpd(sf::Vector2f spdVec[4], const float startSpd) {
    int xSign[4] = {-1, +1, +1, -1};
    int ySign[4] = {+1, +1, -1, -1};
    float xRatio, xSquared;
    for (int n = 0; n < 4; n++) {
        xRatio = genRandInt(0, 20)/20.f;
        spdVec[n].x = xRatio * startSpd * xSign[n];
        spdVec[n].y = std::sqrt(1 - xRatio*xRatio) * startSpd * ySign[n];
    }
}

void randPos(sf::Vector2f poss[4], const float rads[4], const int WIDTH, const int HEIGHT) {
    int xOffset;
    int yOffset;
    int xSign[4] = {+1, -1, -1, +1};
    int ySign[4] = {-1, -1, +1, +1};
    for (int n = 0; n < 4; n++) {
        xOffset = genRandInt(rads[n], WIDTH/2 - rads[n]);
        yOffset = genRandInt(rads[n], HEIGHT/2 - rads[n]);
        poss[n].x = WIDTH/2 + xSign[n] * xOffset;
        poss[n].y = HEIGHT/2 + ySign[n] * yOffset;
    }
}

int main() {
    sf::Font font;
    if (!font.openFromFile("SpaceMono-Regular.ttf")) return 1;
    std::string textStr;

    const int HEIGHT = 600;
    const int WIDTH = 600;
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "sfml test");
    window.setFramerateLimit(120);

    const int amount = 4;
    const int pair = amount * (amount-1) / 2;
    const float rads[amount] = {30.f, 30.f, 30.f, 30.f};
    float cSpeed = 360.f;
    sf::Vector2f startSpd[amount];
    sf::Vector2f startPos[amount];
    randSpd(startSpd, cSpeed);
    randPos(startPos, rads, WIDTH, HEIGHT);
    circ circles[amount];

    for (int n = 0; n < amount; n++) {
        circles[n].shape.setRadius(rads[n]);
        circles[n].shape.setOrigin(sf::Vector2f(rads[n], rads[n]));
        circles[n].shape.setPosition(startPos[n]);
        circles[n].spd = startSpd[n];
        circles[n].pos = startPos[n];
        circles[n].rad = rads[n];
    }

    circles[0].shape.setFillColor(sf::Color::Red);
    circles[1].shape.setFillColor(sf::Color::Green);
    circles[2].shape.setFillColor(sf::Color::Blue);
    circles[3].shape.setFillColor(sf::Color::White);

    float temp;
    float fps;
    float dt;
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
        sf::Text text(font, textStr, 24);
        window.draw(text);

        for (int n = 0; n < amount; n++) {
            handleEdgeColl(circles[n].spd, circles[n].pos, circles[n].rad, WIDTH, HEIGHT);
        }

        for (int i = 0; i < amount; i++) {
            for (int j = i+1; j < amount; j++) {
                handleCircleColl(circles[i], circles[j]);
            }
        }

        for (int n = 0; n < amount; n++) {
            circles[n].pos.x += circles[n].spd.x * dt;
            circles[n].pos.y += circles[n].spd.y * dt;
            circles[n].shape.setPosition(circles[n].pos);
            window.draw(circles[n].shape);
        }
        window.display();
    }
}
