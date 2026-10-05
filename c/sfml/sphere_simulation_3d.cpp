#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <random>
#include <vector>
#include <cmath>

// config
const float WIDTH = 1000.f;
const float HEIGHT = 800.f;
const int AMOUNT = 40;
const float SPEED = 250.f;
const float MINSIZE = 18.f;
const float MAXSIZE = 45.f;
const float BOX = 300.f;        // half the side of the cube the spheres live in
const float GRAVITY = 600.f;
const float RESTITUTION = 1.f;  // 1 = perfectly elastic
const int SUBSTEPS = 4;
const float FOCAL = 700.f;      // perspective strength, bigger = flatter
const int SPRITESIZE = 256;     // resolution of the pre-shaded sphere texture
const float PI = 3.14159265f;

struct sphere {
    sf::Vector3f pos;
    sf::Vector3f spd;
    sf::Color color;
    float rad;
    float invMass;
};

struct camera {
    float yaw;
    float pitch;
    float dist;
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

float genRandFloat(float min, float max) {
    if (min > max) std::swap(min, max);
    std::uniform_real_distribution<float> distrib(min, max);
    return distrib(globalRng());
}

float dot(sf::Vector3f a, sf::Vector3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// world space -> camera space, the camera orbits the origin and looks at it
sf::Vector3f toView(const camera &cam, sf::Vector3f p) {
    float cy = std::cos(cam.yaw), sy = std::sin(cam.yaw);
    float cp = std::cos(cam.pitch), sp = std::sin(cam.pitch);
    float x1 = p.x * cy - p.z * sy;
    float z1 = p.x * sy + p.z * cy;
    float y2 = p.y * cp + z1 * sp;
    float z2 = -p.y * sp + z1 * cp;
    return {x1, y2, z2 + cam.dist};
}

// camera space -> screen pixels, y is flipped because screen y grows downward
sf::Vector2f toScreen(sf::Vector3f v) {
    return {WIDTH / 2.f + FOCAL * v.x / v.z, HEIGHT / 2.f - FOCAL * v.y / v.z};
}

sf::Vector2f project(const camera &cam, sf::Vector3f p) {
    return toScreen(toView(cam, p));
}

// a white lit sphere, tinted per sphere with sprite.setColor, light comes from the upper left
sf::Image makeShadedImage(bool highlight) {
    sf::Image img({SPRITESIZE, SPRITESIZE}, sf::Color::Transparent);
    float half = SPRITESIZE / 2.f;
    sf::Vector3f light(-0.45f, -0.55f, 0.7f);
    light /= std::sqrt(dot(light, light));
    // half vector between light and the viewer for the specular term
    sf::Vector3f halfVec = light + sf::Vector3f(0.f, 0.f, 1.f);
    halfVec /= std::sqrt(dot(halfVec, halfVec));

    for (unsigned y = 0; y < SPRITESIZE; y++) {
        for (unsigned x = 0; x < SPRITESIZE; x++) {
            float dx = (x + 0.5f - half) / half;
            float dy = (y + 0.5f - half) / half;
            float d2 = dx * dx + dy * dy;
            if (d2 > 1.f) continue;
            sf::Vector3f nor(dx, dy, std::sqrt(1.f - d2));
            // soften the rim so the edge is anti-aliased
            float edge = std::clamp((1.f - std::sqrt(d2)) * half, 0.f, 1.f);

            if (highlight) {
                float spec = std::pow(std::max(0.f, dot(nor, halfVec)), 60.f);
                img.setPixel({x, y}, sf::Color(255, 255, 255, static_cast<std::uint8_t>(255 * spec * edge)));
            } else {
                float diffuse = std::max(0.f, dot(nor, light));
                float lum = std::min(1.f, 0.18f + 0.85f * diffuse);
                auto c = static_cast<std::uint8_t>(255 * lum);
                img.setPixel({x, y}, sf::Color(c, c, c, static_cast<std::uint8_t>(255 * edge)));
            }
        }
    }
    return img;
}

void randColors(std::vector<sphere> &spheres) {
    sf::Color available[] = {
        sf::Color(235, 80, 80),
        sf::Color(80, 200, 120),
        sf::Color(80, 140, 240),
        sf::Color(240, 210, 80),
        sf::Color(210, 100, 230),
        sf::Color(80, 220, 230),
        sf::Color(245, 150, 60),
    };
    for (auto &s : spheres) s.color = available[genRandInt(0, 6)];
}

// drop spheres at random spots in the box, retrying when they overlap an earlier one
void randPos(std::vector<sphere> &spheres) {
    for (std::size_t n = 0; n < spheres.size(); n++) {
        sphere &s = spheres[n];
        for (int attempt = 0; attempt < 1000; attempt++) {
            float lim = BOX - s.rad;
            s.pos = {genRandFloat(-lim, lim), genRandFloat(-lim, lim), genRandFloat(-lim, lim)};
            bool free = true;
            for (std::size_t m = 0; m < n; m++) {
                sf::Vector3f d = s.pos - spheres[m].pos;
                float minDist = s.rad + spheres[m].rad;
                if (dot(d, d) < minDist * minDist) {
                    free = false;
                    break;
                }
            }
            if (free) break;
        }
    }
}

void randSpd(std::vector<sphere> &spheres, float speed) {
    for (auto &s : spheres) {
        // random direction on the unit sphere
        float z = genRandFloat(-1.f, 1.f);
        float a = genRandFloat(0.f, 2.f * PI);
        float r = std::sqrt(1.f - z * z);
        s.spd = sf::Vector3f(r * std::cos(a), r * std::sin(a), z) * genRandFloat(0.3f, 1.f) * speed;
    }
}

void setupSpheres(std::vector<sphere> &spheres) {
    for (auto &s : spheres) {
        s.rad = genRandFloat(MINSIZE, MAXSIZE);
        // mass grows with volume, scaled so the KE readout stays readable
        s.invMass = 1.f / (s.rad * s.rad * s.rad / 1000.f);
    }
    randColors(spheres);
    randPos(spheres);
    randSpd(spheres, SPEED);
}

void handleWallColl(sphere &s) {
    float *p[3] = {&s.pos.x, &s.pos.y, &s.pos.z};
    float *v[3] = {&s.spd.x, &s.spd.y, &s.spd.z};
    for (int k = 0; k < 3; k++) {
        if (*p[k] - s.rad < -BOX) {
            *p[k] = -BOX + s.rad;
            if (*v[k] < 0.f) *v[k] = -*v[k] * RESTITUTION;
        } else if (*p[k] + s.rad > BOX) {
            *p[k] = BOX - s.rad;
            if (*v[k] > 0.f) *v[k] = -*v[k] * RESTITUTION;
        }
    }
}

void handleSphereColl(sphere &a, sphere &b) {
    sf::Vector3f d = b.pos - a.pos;
    float dist2 = dot(d, d);
    float minDist = a.rad + b.rad;
    if (dist2 >= minDist * minDist || dist2 == 0.f) return;

    float dist = std::sqrt(dist2);
    sf::Vector3f norVec = d / dist;
    float invSum = a.invMass + b.invMass;

    // push apart, the lighter sphere moves more
    float overlap = minDist - dist;
    a.pos -= norVec * (overlap * a.invMass / invSum);
    b.pos += norVec * (overlap * b.invMass / invSum);

    float relSpd = dot(b.spd - a.spd, norVec);
    if (relSpd >= 0.f) return; // already separating
    float impulse = -(1.f + RESTITUTION) * relSpd / invSum;
    a.spd -= norVec * (impulse * a.invMass);
    b.spd += norVec * (impulse * b.invMass);
}

void step(std::vector<sphere> &spheres, float dt, bool gravity) {
    for (auto &s : spheres) {
        if (gravity) s.spd.y -= GRAVITY * dt;
        s.pos += s.spd * dt;
    }
    for (std::size_t i = 0; i < spheres.size(); i++) {
        for (std::size_t j = i + 1; j < spheres.size(); j++) {
            handleSphereColl(spheres[i], spheres[j]);
        }
    }
    for (auto &s : spheres) handleWallColl(s);
}

void drawLine3d(sf::RenderWindow &window, const camera &cam, sf::Vector3f a, sf::Vector3f b, sf::Color color) {
    sf::Vertex line[2] = {
        sf::Vertex{project(cam, a), color},
        sf::Vertex{project(cam, b), color},
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);
}

void drawFloor(sf::RenderWindow &window, const camera &cam) {
    sf::ConvexShape floor(4);
    floor.setPoint(0, project(cam, {-BOX, -BOX, -BOX}));
    floor.setPoint(1, project(cam, { BOX, -BOX, -BOX}));
    floor.setPoint(2, project(cam, { BOX, -BOX,  BOX}));
    floor.setPoint(3, project(cam, {-BOX, -BOX,  BOX}));
    floor.setFillColor(sf::Color(35, 38, 48));
    window.draw(floor);

    sf::Color gridColor(55, 60, 75);
    const int LINES = 8;
    for (int k = 1; k < LINES; k++) {
        float t = -BOX + 2.f * BOX * k / LINES;
        drawLine3d(window, cam, {t, -BOX, -BOX}, {t, -BOX, BOX}, gridColor);
        drawLine3d(window, cam, {-BOX, -BOX, t}, {BOX, -BOX, t}, gridColor);
    }
}

// a flat disc on the floor under each sphere, smaller and fainter the higher it is
void drawShadow(sf::RenderWindow &window, const camera &cam, const sphere &s) {
    const int POINTS = 20;
    float height = (s.pos.y - s.rad + BOX) / (2.f * BOX);
    float rad = s.rad * (1.f - 0.4f * height);
    sf::ConvexShape shadow(POINTS);
    for (int k = 0; k < POINTS; k++) {
        float a = 2.f * PI * k / POINTS;
        sf::Vector3f p(s.pos.x + rad * std::cos(a), -BOX + 0.5f, s.pos.z + rad * std::sin(a));
        p.x = std::clamp(p.x, -BOX, BOX);
        p.z = std::clamp(p.z, -BOX, BOX);
        shadow.setPoint(k, project(cam, p));
    }
    shadow.setFillColor(sf::Color(0, 0, 0, static_cast<std::uint8_t>(140 * (1.f - 0.7f * height))));
    window.draw(shadow);
}

void drawBoxEdges(sf::RenderWindow &window, const camera &cam, sf::Color color) {
    sf::Vector3f c[8];
    for (int k = 0; k < 8; k++) {
        c[k] = {k & 1 ? BOX : -BOX, k & 2 ? BOX : -BOX, k & 4 ? BOX : -BOX};
    }
    // corners that differ by exactly one bit share an edge
    for (int a = 0; a < 8; a++) {
        for (int bit = 1; bit < 8; bit <<= 1) {
            int b = a | bit;
            if (b != a) drawLine3d(window, cam, c[a], c[b], color);
        }
    }
}

int main() {
    sf::Font font;
    if (!font.openFromFile("SpaceMono-Regular.ttf")) return 1;
    std::string textStr;

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(WIDTH), static_cast<unsigned int>(HEIGHT)}), "sphere simulation 3d",
                            sf::Style::Default, sf::State::Windowed, sf::ContextSettings{0, 0, 4});
    window.setFramerateLimit(144);

    sf::Texture bodyTex, specTex;
    if (!bodyTex.loadFromImage(makeShadedImage(false))) return 1;
    if (!specTex.loadFromImage(makeShadedImage(true))) return 1;
    bodyTex.setSmooth(true);
    specTex.setSmooth(true);
    sf::Sprite bodySprite(bodyTex);
    sf::Sprite specSprite(specTex);
    bodySprite.setOrigin({SPRITESIZE / 2.f, SPRITESIZE / 2.f});
    specSprite.setOrigin({SPRITESIZE / 2.f, SPRITESIZE / 2.f});

    std::vector<sphere> spheres(AMOUNT);
    setupSpheres(spheres);

    camera cam{0.6f, 0.45f, 1100.f};
    bool dragging = false;
    bool gravity = false;
    bool paused = false;
    sf::Vector2i lastMouse;

    float fps = 0.f, dt, energy = 0.f;
    sf::Clock clock;
    sf::Clock clock2;
    sf::Text text(font, textStr, 18);
    sf::Text help(font, "drag: orbit  scroll: zoom  G: gravity  SPACE: kick  P: pause  R: reset", 14);
    help.setPosition({10.f, HEIGHT - 26.f});
    help.setFillColor(sf::Color(150, 150, 160));
    std::vector<int> order(AMOUNT);
    std::vector<float> depth(AMOUNT);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (auto *e = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (e->button == sf::Mouse::Button::Left) {
                    dragging = true;
                    lastMouse = e->position;
                }
            } else if (auto *e = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (e->button == sf::Mouse::Button::Left) dragging = false;
            } else if (auto *e = event->getIf<sf::Event::MouseMoved>()) {
                if (dragging) {
                    sf::Vector2i delta = e->position - lastMouse;
                    lastMouse = e->position;
                    cam.yaw -= delta.x * 0.008f;
                    cam.pitch = std::clamp(cam.pitch + delta.y * 0.008f, -1.4f, 1.4f);
                }
            } else if (auto *e = event->getIf<sf::Event::MouseWheelScrolled>()) {
                // keep the camera outside the box so nothing goes behind it
                cam.dist = std::clamp(cam.dist * (e->delta > 0 ? 0.9f : 1.1f), BOX * 2.2f, BOX * 8.f);
            } else if (auto *e = event->getIf<sf::Event::KeyPressed>()) {
                if (e->code == sf::Keyboard::Key::Escape) window.close();
                if (e->code == sf::Keyboard::Key::G) gravity = !gravity;
                if (e->code == sf::Keyboard::Key::P) paused = !paused;
                if (e->code == sf::Keyboard::Key::R) setupSpheres(spheres);
                if (e->code == sf::Keyboard::Key::Space) {
                    for (auto &s : spheres) s.spd.y += genRandFloat(0.5f, 1.f) * SPEED * 2.f;
                }
            }
        }

        // cap dt so dragging the window doesn't teleport everything through the walls
        dt = std::min(clock.restart().asSeconds(), 1.f / 30.f);
        if (!paused) {
            for (int k = 0; k < SUBSTEPS; k++) step(spheres, dt / SUBSTEPS, gravity);
        }

        if (clock2.getElapsedTime().asSeconds() >= 0.5f) {
            fps = 1.0f / dt;
            // kinetic plus potential (measured from the floor) when gravity is on
            energy = 0.f;
            for (auto &s : spheres) {
                float mass = 1.f / s.invMass;
                energy += 0.5f * mass * dot(s.spd, s.spd);
                if (gravity) energy += mass * GRAVITY * (s.pos.y - s.rad + BOX);
            }
            textStr = " FPS: " + numToStr(fps) + "\n E: " + numToStr(energy / 1e6f) + "M" +
                      "\n gravity: " + (gravity ? "on" : "off") + (paused ? "\n PAUSED" : "");
            text.setString(textStr);
            clock2.restart();
        }

        window.clear(sf::Color(14, 15, 20));
        drawFloor(window, cam);
        for (auto &s : spheres) drawShadow(window, cam, s);
        drawBoxEdges(window, cam, sf::Color(90, 95, 115));

        // painter's algorithm, the furthest sphere is drawn first
        for (int n = 0; n < AMOUNT; n++) {
            order[n] = n;
            depth[n] = toView(cam, spheres[n].pos).z;
        }
        std::sort(order.begin(), order.end(), [&](int a, int b) { return depth[a] > depth[b]; });

        for (int n : order) {
            const sphere &s = spheres[n];
            sf::Vector3f view = toView(cam, s.pos);
            sf::Vector2f screen = toScreen(view);
            float scale = 2.f * FOCAL * s.rad / view.z / SPRITESIZE;

            bodySprite.setPosition(screen);
            bodySprite.setScale({scale, scale});
            bodySprite.setColor(s.color);
            window.draw(bodySprite);

            specSprite.setPosition(screen);
            specSprite.setScale({scale, scale});
            window.draw(specSprite);
        }

        window.draw(text);
        window.draw(help);
        window.display();
    }
}
