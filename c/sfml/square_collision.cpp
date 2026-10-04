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
const int AMOUNT = 64;
const float SPEED = 100.f;
const float MINLEN = 20.f;
const float MAXLEN = 60.f;
const float RESTITUTION = 1.f; // 1 = perfectly elastic
const float FRICTION = 0.f;    // raise to let sliding contacts transfer spin (drains energy)
// a rotated square never reaches further than half its diagonal from its center
const float GRIDSIZE = MAXLEN * std::sqrt(2.f);
const int COLS = (WIDTH + GRIDSIZE -1) / GRIDSIZE;
const int ROWS = (HEIGHT + GRIDSIZE -1) / GRIDSIZE;
int screenVolume = HEIGHT * WIDTH;
int sVolume = MAXLEN * MAXLEN * AMOUNT;

struct square {
    sf::RectangleShape shape;
    sf::Vector2f spd;
    sf::Vector2f pos;
    float omega; // rad/s
    float len;
    float invMass;
    float invInertia;
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

float dot(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

float cross(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.y - a.y * b.x;
}

// velocity of a point on a body that is r away from its center
sf::Vector2f pointVel(const square &sq, sf::Vector2f r) {
    return sq.spd + sf::Vector2f(-sq.omega * r.y, sq.omega * r.x);
}

void applyImpulse(square &sq, sf::Vector2f impulse, sf::Vector2f r) {
    sq.spd += impulse * sq.invMass;
    sq.omega += cross(r, impulse) * sq.invInertia;
}

// the two edge directions of a rotated square
void getAxes(const square &sq, sf::Vector2f axes[2]) {
    float angle = sq.shape.getRotation().asRadians();
    axes[0] = sf::Vector2f(std::cos(angle), std::sin(angle));
    axes[1] = sf::Vector2f(-std::sin(angle), std::cos(angle));
}

void getCorners(const square &sq, const sf::Vector2f axes[2], sf::Vector2f corners[4]) {
    float half = sq.len / 2.f;
    corners[0] = sq.pos + half * axes[0] + half * axes[1];
    corners[1] = sq.pos - half * axes[0] + half * axes[1];
    corners[2] = sq.pos - half * axes[0] - half * axes[1];
    corners[3] = sq.pos + half * axes[0] - half * axes[1];
}

bool isInside(const square &sq, const sf::Vector2f axes[2], sf::Vector2f point) {
    sf::Vector2f rel = point - sq.pos;
    float half = sq.len / 2.f;
    return std::abs(dot(rel, axes[0])) <= half && std::abs(dot(rel, axes[1])) <= half;
}

// half the length of the square's shadow on an axis
float projHalfLen(const square &sq, const sf::Vector2f axes[2], sf::Vector2f axis) {
    return (sq.len / 2.f) * (std::abs(dot(axes[0], axis)) + std::abs(dot(axes[1], axis)));
}

// normal and friction impulse at a contact point, body1 can be null for an immovable wall
void resolveContact(square *body1, square &body2, sf::Vector2f norVec, sf::Vector2f contact) {
    sf::Vector2f r1, r2, relVel, tanVec;
    float velNor, velTan, rn1, rn2, rt1, rt2, invMass1, invInertia1, j, jt, tanLen;

    invMass1 = body1 ? body1->invMass : 0.f;
    invInertia1 = body1 ? body1->invInertia : 0.f;
    r1 = body1 ? contact - body1->pos : sf::Vector2f();
    r2 = contact - body2.pos;

    // relative velocity of the contact point, norVec points from body1 to body2
    relVel = pointVel(body2, r2) - (body1 ? pointVel(*body1, r1) : sf::Vector2f());
    velNor = dot(relVel, norVec);

    // skip if separating
    if (velNor > 0.f) return;

    rn1 = cross(r1, norVec);
    rn2 = cross(r2, norVec);
    j = -(1.f + RESTITUTION) * velNor /
        (invMass1 + body2.invMass + rn1 * rn1 * invInertia1 + rn2 * rn2 * body2.invInertia);

    if (body1) applyImpulse(*body1, -j * norVec, r1);
    applyImpulse(body2, j * norVec, r2);

    if (FRICTION <= 0.f) return;

    // coulomb friction along the sliding direction, limited by the normal impulse
    relVel = pointVel(body2, r2) - (body1 ? pointVel(*body1, r1) : sf::Vector2f());
    tanVec = relVel - dot(relVel, norVec) * norVec;
    tanLen = std::sqrt(dot(tanVec, tanVec));
    if (tanLen < 1e-6f) return;
    tanVec /= tanLen;
    velTan = dot(relVel, tanVec);

    rt1 = cross(r1, tanVec);
    rt2 = cross(r2, tanVec);
    jt = -velTan / (invMass1 + body2.invMass + rt1 * rt1 * invInertia1 + rt2 * rt2 * body2.invInertia);
    jt = std::clamp(jt, -FRICTION * j, FRICTION * j);

    if (body1) applyImpulse(*body1, -jt * tanVec, r1);
    applyImpulse(body2, jt * tanVec, r2);
}

void handleSquareColl(square &square1, square &square2) {
    sf::Vector2f relVec, norVec, contact, axes1[2], axes2[2], allAxes[4], corners[4];
    float overlap, minOverlap, reach, dist, totalInvMass;
    int contactCount;

    relVec = square2.pos - square1.pos;

    // skip if the bounding circles don't touch
    reach = (square1.len + square2.len) * std::sqrt(2.f) / 2.f;
    if (dot(relVec, relVec) > reach * reach) return;

    // separating axis test, only the edge directions of both squares need checking
    getAxes(square1, axes1);
    getAxes(square2, axes2);
    allAxes[0] = axes1[0];
    allAxes[1] = axes1[1];
    allAxes[2] = axes2[0];
    allAxes[3] = axes2[1];

    minOverlap = reach * 2.f;
    for (int k = 0; k < 4; k++) {
        dist = std::abs(dot(relVec, allAxes[k]));
        overlap = projHalfLen(square1, axes1, allAxes[k]) + projHalfLen(square2, axes2, allAxes[k]) - dist;
        if (overlap <= 0.f) return;
        if (overlap < minOverlap) {
            minOverlap = overlap;
            norVec = allAxes[k];
        }
    }

    // collision normal points from square1 to square2
    if (dot(norVec, relVec) < 0.f) norVec = -norVec;

    // contact point is the average of the corners poking into the other square
    contactCount = 0;
    getCorners(square2, axes2, corners);
    for (int k = 0; k < 4; k++) {
        if (isInside(square1, axes1, corners[k])) { contact += corners[k]; contactCount++; }
    }
    getCorners(square1, axes1, corners);
    for (int k = 0; k < 4; k++) {
        if (isInside(square2, axes2, corners[k])) { contact += corners[k]; contactCount++; }
    }
    if (contactCount > 0) contact /= static_cast<float>(contactCount);
    else contact = (square1.pos + square2.pos) / 2.f;

    // push back the squares so that it's not overlapping, lighter one moves more
    totalInvMass = square1.invMass + square2.invMass;
    square1.pos -= norVec * (minOverlap * square1.invMass / totalInvMass);
    square2.pos += norVec * (minOverlap * square2.invMass / totalInvMass);

    resolveContact(&square1, square2, norVec, contact);
}

void handleEdgeColl(square &sq, const float WIDTH, const float HEIGHT) {
    // inward normal and position of each wall: left, right, top, bottom
    const sf::Vector2f wallNor[4] = {{1.f, 0.f}, {-1.f, 0.f}, {0.f, 1.f}, {0.f, -1.f}};
    const float wallDist[4] = {0.f, -WIDTH, 0.f, -HEIGHT};
    sf::Vector2f axes[2], corners[4], contact;
    float depth, maxDepth;
    int contactCount;

    for (int w = 0; w < 4; w++) {
        getAxes(sq, axes);
        getCorners(sq, axes, corners);

        // contact point is the average of the corners past the wall
        contact = sf::Vector2f();
        contactCount = 0;
        maxDepth = 0.f;
        for (int k = 0; k < 4; k++) {
            depth = wallDist[w] - dot(corners[k], wallNor[w]);
            if (depth <= 0.f) continue;
            contact += corners[k];
            contactCount++;
            maxDepth = std::max(maxDepth, depth);
        }
        if (contactCount == 0) continue;
        contact /= static_cast<float>(contactCount);

        sq.pos += wallNor[w] * maxDepth;
        contact += wallNor[w] * maxDepth;
        resolveContact(nullptr, sq, wallNor[w], contact);
    }
}

void randSpd(sf::Vector2f spdVec[], float omega[], const float startSpd, const int AMOUNT) {
    int xSign[4] = {-1, +1, +1, -1};
    int ySign[4] = {+1, +1, -1, -1};
    int dir;
    float xRatio;
    for (int n = 0; n < AMOUNT; n++) {
        omega[n] = sf::degrees(static_cast<float>(genRandInt(-179, 179))).asRadians();
        xRatio = genRandInt(0, 90)/90.f;
        dir = genRandInt(0, 3);
        spdVec[n].x = xRatio * startSpd * xSign[dir];
        spdVec[n].y = std::sqrt(1 - xRatio*xRatio) * startSpd * ySign[dir];
    }
}

void randPos(sf::Vector2f poss[], const float reach, const int AMOUNT, const float WIDTH, const float HEIGHT) {
    float min[2], max[2];
    min[0] = min[1] = reach;
    max[0] = WIDTH - reach;
    max[1] = HEIGHT - reach;
    for (int n = 0; n < AMOUNT; n++) {
        poss[n].x = genRandInt(min[0], max[0]);
        poss[n].y = genRandInt(min[1], max[1]);
    }
}

void randLens(float lens[], const float MINLEN, const float MAXLEN, const int AMOUNT) {
    for (int n = 0; n < AMOUNT; n++) {
        lens[n] = genRandInt(MINLEN, MAXLEN);
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
    if (screenVolume < sVolume) {
        printf("not enough space\nscreenVol: %d\nsVol: %d\n", screenVolume, sVolume);
        exit(1);
    }

    sf::Font font;
    if (!font.openFromFile("SpaceMono-Regular.ttf")) return 1;
    std::string textStr;

    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(WIDTH), static_cast<unsigned int>(HEIGHT)}), "sfml test");
    window.setFramerateLimit(256);

    std::vector<std::vector<int>> grids(COLS * ROWS);
    sf::Color shapesColors[AMOUNT];
    float rotSpd[AMOUNT];
    sf::Vector2f startSpd[AMOUNT];
    sf::Vector2f startPos[AMOUNT];
    float lens[AMOUNT];
    randSpd(startSpd, rotSpd, SPEED, AMOUNT);
    randPos(startPos, GRIDSIZE / 2.f, AMOUNT, WIDTH, HEIGHT);
    randColors(shapesColors, AMOUNT);
    randLens(lens, MINLEN, MAXLEN, AMOUNT);
    square squares[AMOUNT];

    for (int n = 0; n < AMOUNT; n++) {
        squares[n].len = lens[n];
        squares[n].spd = startSpd[n];
        squares[n].pos = startPos[n];
        squares[n].omega = rotSpd[n];
        // mass is the side length squared, moment of inertia of a solid square about its center
        squares[n].invMass = 1.f / (squares[n].len * squares[n].len);
        squares[n].invInertia = 6.f * squares[n].invMass / (squares[n].len * squares[n].len);
        squares[n].shape.setSize(sf::Vector2f(squares[n].len, squares[n].len));
        squares[n].shape.setOrigin(sf::Vector2f(squares[n].len / 2.f, squares[n].len / 2.f));
        squares[n].shape.setPosition(squares[n].pos);
        squares[n].shape.setFillColor(shapesColors[n]);
    }

    float fps, dt, energy;
    int cx, cy, atHorizontalEdge, atVerticalEdge;
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
            // total kinetic energy, linear + rotational, should stay constant when elastic and frictionless
            energy = 0.f;
            for (int n = 0; n < AMOUNT; n++) {
                energy += 0.5f * dot(squares[n].spd, squares[n].spd) / squares[n].invMass;
                energy += 0.5f * squares[n].omega * squares[n].omega / squares[n].invInertia;
            }
            textStr = " FPS: " + numToStr(fps) + "\n KE: " + numToStr(energy / 1e6f) + "M";
            text.setString(textStr);
            clock2.restart();
        }

        for (auto &x : grids) x.clear();
        for (int n = 0; n < AMOUNT; n++) {
            cx = static_cast<int>(squares[n].pos.x / GRIDSIZE);
            cy = static_cast<int>(squares[n].pos.y / GRIDSIZE);
            cx = std::clamp(cx, 0, COLS - 1);
            cy = std::clamp(cy, 0, ROWS - 1);
            grids[cy * COLS + cx].push_back(n);
            atHorizontalEdge = ((cy == 0) || (cy == ROWS-1));
            atVerticalEdge = ((cx == 0) || (cx == COLS-1));

            if (!(atHorizontalEdge || atVerticalEdge)) continue;
            handleEdgeColl(squares[n], WIDTH, HEIGHT);
        }

        for (int rowIdx = 0; rowIdx < ROWS; rowIdx++) {
            for (int colIdx = 0; colIdx < COLS; colIdx++) {
                int idx = rowIdx * COLS + colIdx;
                auto &grid1 = grids[idx];

                for (int i = 0; i < grid1.size(); i++) {
                    for (int j = i + 1; j < grid1.size(); j++) {
                        handleSquareColl(squares[grid1[i]], squares[grid1[j]]);
                    }
                }

                auto checkNeighbour = [&](int nx, int ny) {
                    if (nx < 0 || ny < 0 || nx >= COLS || ny >= ROWS) return;
                    auto &grid2 = grids[ny * COLS + nx];
                    for (int i : grid1) {
                        for (int j : grid2) {
                            handleSquareColl(squares[i], squares[j]);
                        }
                    }
                };
                checkNeighbour(colIdx+1, rowIdx);
                checkNeighbour(colIdx, rowIdx+1);
                checkNeighbour(colIdx+1, rowIdx+1);
                checkNeighbour(colIdx-1, rowIdx+1);
            }
        }

        for (int n = 0; n < AMOUNT; n++) {
            squares[n].shape.rotate(sf::radians(squares[n].omega * dt));
            squares[n].pos += squares[n].spd * dt;
            squares[n].shape.setPosition(squares[n].pos);
            window.draw(squares[n].shape);
        }
        window.draw(text);
        window.display();
    }
}
