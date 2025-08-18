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

static std::mt19937& globalRng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

int genRandInt(int min, int max) {
    if (min > max) std::swap(min, max);
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(globalRng());
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

void handleEdgeColl(sf::Vector2f &spd, sf::Vector2f &pos, float RAD, const int WIDTH, const int HEIGHT) {
    int lEdgeColl = 1.f > (pos.x - RAD);
    int rEdgeColl = 1.f > (static_cast<float>(WIDTH) - (pos.x + RAD));
    int tEdgeColl = 1.f > (pos.y - RAD);
    int bEdgeColl = 1.f > (static_cast<float>(HEIGHT) - (pos.y + RAD));
    if ((lEdgeColl && (spd.x < 0.f)) || (rEdgeColl && (spd.x > 0.f))) spd.x *= -1;
    if ((tEdgeColl && (spd.y < 0.f)) || (bEdgeColl && (spd.y > 0.f))) spd.y *= -1;
}

void randSpd(sf::Vector2f spdVec[], const float startSpd, const int AMOUNT) {
    int xSign[4] = {-1, +1, +1, -1};
    int ySign[4] = {+1, +1, -1, -1};
    int dir;
    float xRatio, xSquared;
    for (int n = 0; n < AMOUNT; n++) {
        xRatio = genRandInt(0, 90)/90.f;
        dir = genRandInt(0, 3);
        spdVec[n].x = xRatio * startSpd * xSign[dir];
        spdVec[n].y = std::sqrt(1 - xRatio*xRatio) * startSpd * ySign[dir];
    }
}

void randPos(sf::Vector2f poss[], const float RAD, const int AMOUNT, const int WIDTH, const int HEIGHT) {
    int min[2], max[2];
    min[0] = min[1] = static_cast<int>(RAD);
    max[0] = WIDTH - static_cast<int>(RAD);
    max[1] = HEIGHT - static_cast<int>(RAD);
    for (int n = 0; n < AMOUNT; n++) {
        poss[n].x = genRandInt(min[0], max[0]);
        poss[n].y = genRandInt(min[1], max[1]);
    }
}

void randColors(sf::Color *colors, const int AMOUNT) {
    const sf::Color available[7] = {
        sf::Color::White,
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan,
    };
    for (int n = 0; n < AMOUNT; n++) {
        colors[n] = available[genRandInt(0, 6)];
    }
}

int main() {
    sf::Font font;
    if (!font.openFromFile("SpaceMono-Regular.ttf")) return 1;
    std::string textStr;

    const int HEIGHT = 600;
    const int WIDTH = 600;
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "sfml test");
    window.setFramerateLimit(512);

    const int AMOUNT = 16;
    const int RAD = 28.f;
    const int CSPEED = 360.f;
    sf::Color circlesColors[AMOUNT];
    sf::Vector2f startSpd[AMOUNT];
    sf::Vector2f startPos[AMOUNT];
    randSpd(startSpd, CSPEED, AMOUNT);
    randPos(startPos, RAD, AMOUNT, WIDTH, HEIGHT);
    randColors(circlesColors, AMOUNT);
    circ circles[AMOUNT];

    for (int n = 0; n < AMOUNT; n++) {
        circles[n].rad = RAD;
        circles[n].spd = startSpd[n];
        circles[n].pos = startPos[n];
        circles[n].shape.setRadius(circles[n].rad);
        circles[n].shape.setOrigin(sf::Vector2f(RAD, RAD));
        circles[n].shape.setPosition(circles[n].pos);
        circles[n].shape.setFillColor(circlesColors[n]);
    }

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

        for (int n = 0; n < AMOUNT; n++) {
            handleEdgeColl(circles[n].spd, circles[n].pos, circles[n].rad, WIDTH, HEIGHT);
        }

        for (int i = 0; i < AMOUNT; i++) {
            for (int j = i+1; j < AMOUNT; j++) {
                handleCircleColl(circles[i], circles[j]);
            }
        }

        for (int n = 0; n < AMOUNT; n++) {
            circles[n].pos += circles[n].spd * dt;
            circles[n].shape.setPosition(circles[n].pos);
            window.draw(circles[n].shape);
        }
        window.draw(text);
        window.display();
    }
}
