#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include <random>
#include <cmath>

// config
const int HEIGHT = 800;
const int WIDTH = 1000;
const int AMOUNT = 64;
const int SPEED = 100;
const float MAXRAD = 20;

struct square {
    sf::RectangleShape shape;
    sf::Vector2f spd;
    sf::Vector2f pos;
    sf::Angle omega;
    float len;
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

void randSpd(sf::Vector2f spdVec[], sf::Angle omega[], const float startSpd, const int AMOUNT) {
    int xSign[4] = {-1, +1, +1, -1};
    int ySign[4] = {+1, +1, -1, -1};
    int dir;
    float xRatio, xSquared;
    for (int n = 0; n < AMOUNT; n++) {
        omega[n] = sf::degrees(static_cast<float>(genRandInt(-179, 179)));
        xRatio = genRandInt(0, 90)/90.f;
        dir = genRandInt(0, 3);
        spdVec[n].x = xRatio * startSpd * xSign[dir];
        spdVec[n].y = std::sqrt(1 - xRatio*xRatio) * startSpd * ySign[dir];
    }
}

void randPos(sf::Vector2f poss[], const float MAXRAD, const int AMOUNT, const int WIDTH, const int HEIGHT) {
    int min[2], max[2];
    min[0] = min[1] = static_cast<int>(MAXRAD);
    max[0] = WIDTH - static_cast<int>(MAXRAD);
    max[1] = HEIGHT - static_cast<int>(MAXRAD);
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

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "sfml test");
    window.setFramerateLimit(256);

    sf::Color shapesColors[AMOUNT];
    sf::Angle rotSpd[AMOUNT];
    sf::Vector2f startSpd[AMOUNT];
    sf::Vector2f startPos[AMOUNT];
    randSpd(startSpd, rotSpd, SPEED, AMOUNT);
    randPos(startPos, MAXRAD, AMOUNT, WIDTH, HEIGHT);
    randColors(shapesColors, AMOUNT);
    square squares[AMOUNT];

    for (int n = 0; n < AMOUNT; n++) {
        squares[n].len = 2.0*MAXRAD;
        squares[n].spd = startSpd[n];
        squares[n].pos = startPos[n];
        squares[n].omega = rotSpd[n];
        squares[n].shape.setSize(sf::Vector2f(squares[n].len,squares[n].len));
        squares[n].shape.setOrigin(sf::Vector2f(squares[n].len, squares[n].len));
        squares[n].shape.setPosition(squares[n].pos);
        squares[n].shape.setFillColor(shapesColors[n]);
    }

    float fps;
    float dt;
    sf::Clock clock;
    sf::Clock clock2;
    sf::Text text(font, textStr, 20);
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
            text.setString(textStr);
            clock2.restart();
        }

        for (int n = 0; n < AMOUNT; n++) {
            //handleEdgeColl(squares[n].spd, squares[n].pos, squares[n].len, WIDTH, HEIGHT);
        }

        for (int i = 0; i < AMOUNT; i++) {
            for (int j = i+1; j < AMOUNT; j++) {
                //handleSquareColl(squares[i], squares[j]);
            }
        }

        for (int n = 0; n < AMOUNT; n++) {
            squares[n].shape.rotate(squares[n].omega * dt);
            squares[n].pos += squares[n].spd * dt;
            squares[n].shape.setPosition(squares[n].pos);
            window.draw(squares[n].shape);
        }
        window.draw(text);
        window.display();
    }
}