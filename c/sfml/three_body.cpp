#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <random>
#include <vector>
#include <deque>
#include <cmath>

// config
const float WIDTH = 1000.f;
const float HEIGHT = 800.f;
const double G = 1.0;            // simulation units, G = 1
const double SOFTENING = 1e-3;   // keeps close encounters from blowing up
const double ETA = 0.01;         // adaptive step accuracy, smaller = more accurate
const double MINSTEP = 1e-7;
const double MAXSTEP = 1e-3;
const int MAXSTEPSPERFRAME = 200000;
const int TRAILLEN = 2000;
const float TRAILSPACING = 0.01f; // sim time between recorded trail points
const float DEFAULTZOOM = 250.f;  // pixels per sim unit

struct vec2 {
    double x, y;
    vec2 operator+(vec2 o) const { return {x + o.x, y + o.y}; }
    vec2 operator-(vec2 o) const { return {x - o.x, y - o.y}; }
    vec2 operator*(double s) const { return {x * s, y * s}; }
    vec2 &operator+=(vec2 o) { x += o.x; y += o.y; return *this; }
    vec2 &operator-=(vec2 o) { x -= o.x; y -= o.y; return *this; }
};

double length(vec2 v) { return std::sqrt(v.x * v.x + v.y * v.y); }

struct body {
    vec2 pos;
    vec2 vel;
    vec2 acc;
    double mass;
    sf::Color color;
    std::deque<vec2> trail;
};

struct view2d {
    vec2 center;
    float zoom;
};

std::string numToStr(double num, int precision = 2) {
    std::ostringstream temp;
    temp << std::fixed << std::setprecision(precision) << num;
    return temp.str();
}

static std::mt19937& globalRng() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

double genRandDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(globalRng());
}

const sf::Color COLORS[3] = {sf::Color(255, 120, 90), sf::Color(90, 200, 255), sf::Color(255, 220, 90)};

void computeAccelerations(std::vector<body> &bodies) {
    for (auto &b : bodies) b.acc = {0, 0};
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            vec2 d = bodies[j].pos - bodies[i].pos;
            double r2 = d.x * d.x + d.y * d.y + SOFTENING * SOFTENING;
            double inv = 1.0 / (r2 * std::sqrt(r2));
            bodies[i].acc += d * (G * bodies[j].mass * inv);
            bodies[j].acc -= d * (G * bodies[i].mass * inv);
        }
    }
}

// step size shrinks during close encounters, based on the tightest pair's free-fall time
double adaptiveStep(const std::vector<body> &bodies) {
    double best = MAXSTEP;
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            double r = length(bodies[j].pos - bodies[i].pos) + SOFTENING;
            double m = bodies[i].mass + bodies[j].mass;
            best = std::min(best, ETA * std::sqrt(r * r * r / (G * m)));
        }
    }
    return std::clamp(best, MINSTEP, MAXSTEP);
}

// velocity verlet (kick-drift-kick), accelerations must be current on entry
void step(std::vector<body> &bodies, double h) {
    for (auto &b : bodies) {
        b.vel += b.acc * (h * 0.5);
        b.pos += b.vel * h;
    }
    computeAccelerations(bodies);
    for (auto &b : bodies) b.vel += b.acc * (h * 0.5);
}

double totalEnergy(const std::vector<body> &bodies) {
    double e = 0.0;
    for (size_t i = 0; i < bodies.size(); i++) {
        e += 0.5 * bodies[i].mass * (bodies[i].vel.x * bodies[i].vel.x + bodies[i].vel.y * bodies[i].vel.y);
        for (size_t j = i + 1; j < bodies.size(); j++) {
            vec2 d = bodies[j].pos - bodies[i].pos;
            e -= G * bodies[i].mass * bodies[j].mass / std::sqrt(d.x * d.x + d.y * d.y + SOFTENING * SOFTENING);
        }
    }
    return e;
}

vec2 centerOfMass(const std::vector<body> &bodies) {
    vec2 c{0, 0};
    double m = 0;
    for (auto &b : bodies) { c += b.pos * b.mass; m += b.mass; }
    return c * (1.0 / m);
}

// shift into the center of mass frame so the system doesn't drift off screen
void recenter(std::vector<body> &bodies) {
    vec2 p{0, 0}, v{0, 0};
    double m = 0;
    for (auto &b : bodies) { p += b.pos * b.mass; v += b.vel * b.mass; m += b.mass; }
    p = p * (1.0 / m);
    v = v * (1.0 / m);
    for (auto &b : bodies) { b.pos -= p; b.vel -= v; }
}

std::string loadPreset(std::vector<body> &bodies, int preset) {
    bodies.assign(3, body{});
    std::string name;
    switch (preset) {
    case 1: { // chenciner-montgomery figure eight
        vec2 v3{-0.93240737, -0.86473146};
        bodies[0].pos = {-0.97000436, 0.24308753};
        bodies[1].pos = {0.97000436, -0.24308753};
        bodies[2].pos = {0, 0};
        bodies[0].vel = v3 * -0.5;
        bodies[1].vel = v3 * -0.5;
        bodies[2].vel = v3;
        for (auto &b : bodies) b.mass = 1.0;
        name = "figure eight";
        break;
    }
    case 2: { // lagrange equilateral triangle, a rigid rotation that is unstable for equal masses
        double side = 1.5;
        double r = side / std::sqrt(3.0);
        double v = std::sqrt(G * 1.0 / side);
        for (int n = 0; n < 3; n++) {
            double a = n * 2.0 * 3.14159265358979 / 3.0;
            bodies[n].pos = {r * std::cos(a), r * std::sin(a)};
            bodies[n].vel = {-v * std::sin(a), v * std::cos(a)};
            bodies[n].mass = 1.0;
        }
        bodies[0].pos.x += 1e-4; // tiny nudge so the instability shows up sooner
        name = "lagrange triangle";
        break;
    }
    case 3: { // burrau's pythagorean problem, masses 3 4 5 at rest on a 3-4-5 triangle
        bodies[0].pos = {1, 3};   bodies[0].mass = 3;
        bodies[1].pos = {-2, -1}; bodies[1].mass = 4;
        bodies[2].pos = {1, -1};  bodies[2].mass = 5;
        name = "pythagorean";
        break;
    }
    case 4: { // hierarchical: tight binary with a distant third body
        bodies[0].mass = 1.0; bodies[1].mass = 1.0; bodies[2].mass = 0.5;
        bodies[0].pos = {-0.15, 0}; bodies[1].pos = {0.15, 0};
        double vb = std::sqrt(G * 1.0 / (4 * 0.15));
        bodies[0].vel = {0, -vb}; bodies[1].vel = {0, vb};
        bodies[2].pos = {0, 1.6};
        bodies[2].vel = {std::sqrt(G * 2.0 / 1.6) * 0.9, 0};
        name = "binary + intruder";
        break;
    }
    default: { // random
        for (auto &b : bodies) {
            b.pos = {genRandDouble(-1.2, 1.2), genRandDouble(-1.2, 1.2)};
            b.vel = {genRandDouble(-0.4, 0.4), genRandDouble(-0.4, 0.4)};
            b.mass = genRandDouble(0.6, 1.6);
        }
        name = "random";
        break;
    }
    }
    for (int n = 0; n < 3; n++) bodies[n].color = COLORS[n];
    recenter(bodies);
    computeAccelerations(bodies);
    return name;
}

sf::Vector2f toScreen(const view2d &v, vec2 p) {
    return {static_cast<float>((p.x - v.center.x) * v.zoom) + WIDTH / 2.f,
            static_cast<float>(-(p.y - v.center.y) * v.zoom) + HEIGHT / 2.f};
}

vec2 toWorld(const view2d &v, sf::Vector2f s) {
    return {(s.x - WIDTH / 2.f) / v.zoom + v.center.x, -(s.y - HEIGHT / 2.f) / v.zoom + v.center.y};
}

void drawTrail(sf::RenderWindow &window, const view2d &v, const body &b) {
    if (b.trail.size() < 2) return;
    sf::VertexArray line(sf::PrimitiveType::LineStrip, b.trail.size() + 1);
    size_t n = b.trail.size();
    for (size_t i = 0; i < n; i++) {
        sf::Color c = b.color;
        c.a = static_cast<std::uint8_t>(220.f * (i + 1) / n);
        line[i] = sf::Vertex{toScreen(v, b.trail[i]), c};
    }
    line[n] = sf::Vertex{toScreen(v, b.pos), b.color}; // join the trail to the body
    window.draw(line);
}

void drawBody(sf::RenderWindow &window, const view2d &v, const body &b) {
    float rad = 6.f * static_cast<float>(std::cbrt(b.mass)) * std::sqrt(v.zoom / DEFAULTZOOM);
    rad = std::max(rad, 2.f);
    sf::Vector2f p = toScreen(v, b.pos);

    // a few stacked translucent circles make a cheap glow
    for (int k = 3; k >= 1; k--) {
        sf::CircleShape glow(rad * (1.f + k * 0.8f));
        glow.setOrigin({glow.getRadius(), glow.getRadius()});
        glow.setPosition(p);
        sf::Color c = b.color;
        c.a = static_cast<std::uint8_t>(18 * (4 - k));
        glow.setFillColor(c);
        window.draw(glow);
    }
    sf::CircleShape core(rad);
    core.setOrigin({rad, rad});
    core.setPosition(p);
    core.setFillColor(b.color);
    window.draw(core);
}

int main() {
    sf::Font font;
    if (!font.openFromFile("SpaceMono-Regular.ttf")) return 1;
    std::string textStr;

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(WIDTH), static_cast<unsigned int>(HEIGHT)}), "three body problem",
                            sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed, sf::ContextSettings{0, 0, 4});
    window.setFramerateLimit(144);

    std::vector<body> bodies;
    int preset = 1;
    std::string presetName = loadPreset(bodies, preset);
    double startEnergy = totalEnergy(bodies);
    double simTime = 0.0;
    double timeScale = 1.0;
    float trailTimer = 0.f;
    bool paused = false;
    bool trails = true;
    bool followCom = true;

    view2d view{{0, 0}, DEFAULTZOOM};
    bool dragging = false;
    sf::Vector2i lastMouse;

    float dt = 0.f, fps = 0.f;
    int stepsThisFrame = 0;
    sf::Clock clock;
    sf::Clock clock2;
    sf::Text text(font, textStr, 18);
    sf::Text help(font, "1-5: presets (5 = random)  SPACE: pause  +/-: speed  T: trails  C: follow COM\n"
                        "drag: pan  scroll: zoom  R: reset preset", 14);
    help.setFillColor(sf::Color(150, 150, 165));
    help.setPosition({10.f, HEIGHT - 48.f});

    auto reset = [&](int p) {
        preset = p;
        presetName = loadPreset(bodies, preset);
        startEnergy = totalEnergy(bodies);
        simTime = 0.0;
        trailTimer = 0.f;
        view = {{0, 0}, DEFAULTZOOM};
        if (preset == 3) view.zoom = 110.f; // pythagorean spreads out a lot
    };

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (auto *e = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (e->button == sf::Mouse::Button::Left) {
                    dragging = true;
                    lastMouse = e->position;
                    followCom = false;
                }
            } else if (auto *e = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (e->button == sf::Mouse::Button::Left) dragging = false;
            } else if (auto *e = event->getIf<sf::Event::MouseMoved>()) {
                if (dragging) {
                    sf::Vector2i d = e->position - lastMouse;
                    view.center.x -= d.x / view.zoom;
                    view.center.y += d.y / view.zoom;
                    lastMouse = e->position;
                }
            } else if (auto *e = event->getIf<sf::Event::MouseWheelScrolled>()) {
                // zoom around the cursor so the point under it stays put
                sf::Vector2f mouse(static_cast<float>(e->position.x), static_cast<float>(e->position.y));
                vec2 before = toWorld(view, mouse);
                view.zoom = std::clamp(view.zoom * std::pow(1.15f, e->delta), 5.f, 20000.f);
                vec2 after = toWorld(view, mouse);
                view.center += before - after;
            } else if (auto *e = event->getIf<sf::Event::KeyPressed>()) {
                using K = sf::Keyboard::Key;
                if (e->code == K::Escape) window.close();
                else if (e->code == K::Space) paused = !paused;
                else if (e->code == K::T) {
                    trails = !trails;
                    for (auto &b : bodies) b.trail.clear();
                }
                else if (e->code == K::C) followCom = !followCom;
                else if (e->code == K::R) reset(preset);
                else if (e->code == K::Equal || e->code == K::Add) timeScale = std::min(timeScale * 2.0, 64.0);
                else if (e->code == K::Hyphen || e->code == K::Subtract) timeScale = std::max(timeScale / 2.0, 1.0 / 64.0);
                else if (e->code >= K::Num1 && e->code <= K::Num5)
                    reset(static_cast<int>(e->code) - static_cast<int>(K::Num1) + 1);
            }
        }

        // cap dt so dragging the window doesn't make the sim jump
        dt = std::min(clock.restart().asSeconds(), 1.f / 30.f);

        stepsThisFrame = 0;
        if (!paused) {
            double target = dt * timeScale;
            double done = 0.0;
            while (done < target && stepsThisFrame < MAXSTEPSPERFRAME) {
                double h = std::min(adaptiveStep(bodies), target - done);
                step(bodies, h);
                done += h;
                stepsThisFrame++;

                trailTimer += static_cast<float>(h);
                if (trails && trailTimer >= TRAILSPACING) {
                    trailTimer = 0.f;
                    for (auto &b : bodies) {
                        b.trail.push_back(b.pos);
                        if (b.trail.size() > TRAILLEN) b.trail.pop_front();
                    }
                }
            }
            simTime += done;
        }

        if (followCom) view.center = centerOfMass(bodies);

        if (clock2.getElapsedTime().asSeconds() >= 0.25f) {
            fps = 1.0f / dt;
            double e = totalEnergy(bodies);
            double drift = std::abs((e - startEnergy) / startEnergy);
            textStr = " FPS: " + numToStr(fps) + "\n preset: " + presetName + "\n t: " + numToStr(simTime) +
                      "\n speed: " + numToStr(timeScale, 3) + "x" + "\n steps/frame: " + std::to_string(stepsThisFrame) +
                      "\n E: " + numToStr(e, 5) + "\n dE/E: " + numToStr(drift * 100.0, 4) + "%";
            for (int n = 0; n < 3; n++)
                textStr += "\n m" + std::to_string(n + 1) + ": " + numToStr(bodies[n].mass);
            if (paused) textStr += "\n PAUSED";
            text.setString(textStr);
            clock2.restart();
        }

        window.clear(sf::Color(8, 9, 14));
        if (trails)
            for (auto &b : bodies) drawTrail(window, view, b);
        for (auto &b : bodies) drawBody(window, view, b);

        // small cross at the center of mass
        sf::Vector2f com = toScreen(view, centerOfMass(bodies));
        sf::Vertex cross[4] = {
            {{com.x - 5.f, com.y}, sf::Color(255, 255, 255, 70)}, {{com.x + 5.f, com.y}, sf::Color(255, 255, 255, 70)},
            {{com.x, com.y - 5.f}, sf::Color(255, 255, 255, 70)}, {{com.x, com.y + 5.f}, sf::Color(255, 255, 255, 70)}};
        window.draw(cross, 4, sf::PrimitiveType::Lines);

        window.draw(text);
        window.draw(help);
        window.display();
    }
}
