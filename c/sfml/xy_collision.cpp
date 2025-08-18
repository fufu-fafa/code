#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
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
    sf::Vector2f relVel, relVec, norVec, perpNorVec;
    float temp, overlap, dist, distSquared, distChangeSquared, velProj1, velProj2, perpVelProj1, perpVelProj2;

    relVel = circle2.spd - circle1.spd;
    relVec = circle2.pos - circle1.pos;

    // skip if separating
    distChangeSquared = relVel.x * relVec.x + relVel.y * relVec.y;
    if (distChangeSquared > 0.f) return;

    distSquared = relVec.x * relVec.x + relVec.y * relVec.y;
    if (distSquared == 0.f) return;
    if (distSquared > ((circle1.rad + circle2.rad) * (circle1.rad + circle2.rad))) return;
    dist = std::sqrt(distSquared);

    // circle velocity projection along collision vector
    norVec = relVec / dist;
    velProj1 = circle1.spd.x * norVec.x + circle1.spd.y * norVec.y;
    velProj2 = circle2.spd.x * norVec.x + circle2.spd.y * norVec.y;

    // swap after collision
    temp = velProj1;
    velProj1 = velProj2;
    velProj2 = temp;

    // perpendicular circle velocity projection along collision vector
    perpNorVec.x = -norVec.y;
    perpNorVec.y = norVec.x;
    perpVelProj1 = circle1.spd.x * perpNorVec.x + circle1.spd.y * perpNorVec.y;
    perpVelProj2 = circle2.spd.x * perpNorVec.x + circle2.spd.y * perpNorVec.y;

    // push back the circles so that it's not overlapping
    overlap = circle1.rad + circle2.rad - dist;
    if (overlap > 0.f) {
        circle1.pos -= norVec * (overlap/2.f);
        circle2.pos += norVec * (overlap/2.f);
    }

    circle1.spd = velProj1 * norVec + perpVelProj1 * perpNorVec;
    circle2.spd = velProj2 * norVec + perpVelProj2 * perpNorVec;
}

void handleEdgeColl(sf::Vector2f &spd, sf::Vector2f &pos, float rad, const int WIDTH, const int HEIGHT) {
    int lEdgeColl = 1.f > (pos.x - rad);
    int rEdgeColl = 1.f > (WIDTH - (pos.x + rad));
    int tEdgeColl = 1.f > (pos.y - rad);
    int bEdgeColl = 1.f > (HEIGHT - (pos.y + rad));
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

        for (int n = 0; n < amount; n++) {
            handleEdgeColl(circles[n].spd, circles[n].pos, circles[n].rad, WIDTH, HEIGHT);
        }

        for (int i = 0; i < amount; i++) {
            for (int j = i+1; j < amount; j++) {
                handleCircleColl(circles[i], circles[j]);
                circles[i].shape.setPosition(circles[i].pos);
            }
        }

        for (int n = 0; n < amount; n++) {
            circles[n].pos += circles[n].spd * dt;
            circles[n].shape.setPosition(circles[n].pos);
            window.draw(circles[n].shape);
        }
        window.draw(text);
        window.display();
    }
}
