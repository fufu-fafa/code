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
const float MINSIZE = 20.f;
const float MAXSIZE = 60.f;
const float RESTITUTION = 1.f; // 1 = perfectly elastic
const float FRICTION = 0.f;    // raise to let sliding contacts transfer spin (drains energy)
const float PI = 3.14159265f;
int screenVolume = HEIGHT * WIDTH;
int sVolume = MAXSIZE * MAXSIZE * AMOUNT;

enum shapeType { SQUARE, RECTANGLE, TRIANGLE, CIRCLE, SHAPETYPES };

struct body {
    shapeType type;
    sf::CircleShape circleShape;
    sf::ConvexShape polyShape;
    std::vector<sf::Vector2f> localVerts; // polygon corners relative to the centroid
    std::vector<sf::Vector2f> verts;      // polygon corners in the world
    sf::Vector2f spd;
    sf::Vector2f pos;
    float angle;
    float omega; // rad/s
    float rad;   // circle radius, or the furthest corner of a polygon
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
sf::Vector2f pointVel(const body &b, sf::Vector2f r) {
    return b.spd + sf::Vector2f(-b.omega * r.y, b.omega * r.x);
}

void applyImpulse(body &b, sf::Vector2f impulse, sf::Vector2f r) {
    b.spd += impulse * b.invMass;
    b.omega += cross(r, impulse) * b.invInertia;
}

void updateVerts(body &b) {
    float c = std::cos(b.angle);
    float s = std::sin(b.angle);
    for (std::size_t k = 0; k < b.localVerts.size(); k++) {
        sf::Vector2f v = b.localVerts[k];
        b.verts[k] = b.pos + sf::Vector2f(v.x * c - v.y * s, v.x * s + v.y * c);
    }
}

void moveBody(body &b, sf::Vector2f offset) {
    b.pos += offset;
    for (auto &v : b.verts) v += offset;
}

bool isInside(const body &b, sf::Vector2f point) {
    if (b.type == CIRCLE) {
        sf::Vector2f rel = point - b.pos;
        return dot(rel, rel) <= b.rad * b.rad;
    }
    // inside a convex polygon when the point is on the same side of every edge
    bool left = false, right = false;
    int count = b.verts.size();
    for (int k = 0; k < count; k++) {
        float side = cross(b.verts[(k+1) % count] - b.verts[k], point - b.verts[k]);
        if (side > 0.f) left = true;
        if (side < 0.f) right = true;
    }
    return !(left && right);
}

// projected shadow of a body on an axis
void project(const body &b, sf::Vector2f axis, float &min, float &max) {
    if (b.type == CIRCLE) {
        min = dot(b.pos, axis) - b.rad;
        max = dot(b.pos, axis) + b.rad;
        return;
    }
    min = max = dot(b.verts[0], axis);
    for (auto &v : b.verts) {
        min = std::min(min, dot(v, axis));
        max = std::max(max, dot(v, axis));
    }
}

// separating axis test using the edge normals of both polygons, normal points from poly1 to poly2
bool collidePolyPoly(const body &poly1, const body &poly2, sf::Vector2f &norVec, float &overlap, sf::Vector2f &contact) {
    float min1, max1, min2, max2, axisOverlap;
    int contactCount;

    overlap = (poly1.rad + poly2.rad) * 2.f;
    for (const body *b : {&poly1, &poly2}) {
        int count = b->verts.size();
        for (int k = 0; k < count; k++) {
            sf::Vector2f edge = b->verts[(k+1) % count] - b->verts[k];
            sf::Vector2f axis = sf::Vector2f(-edge.y, edge.x) / std::sqrt(dot(edge, edge));
            project(poly1, axis, min1, max1);
            project(poly2, axis, min2, max2);
            axisOverlap = std::min(max1, max2) - std::max(min1, min2);
            if (axisOverlap <= 0.f) return false;
            if (axisOverlap < overlap) {
                overlap = axisOverlap;
                norVec = axis;
            }
        }
    }
    if (dot(norVec, poly2.pos - poly1.pos) < 0.f) norVec = -norVec;

    // contact point is the average of the corners poking into the other polygon
    contact = sf::Vector2f();
    contactCount = 0;
    for (auto &v : poly2.verts) {
        if (isInside(poly1, v)) { contact += v; contactCount++; }
    }
    for (auto &v : poly1.verts) {
        if (isInside(poly2, v)) { contact += v; contactCount++; }
    }
    if (contactCount > 0) contact /= static_cast<float>(contactCount);
    else contact = (poly1.pos + poly2.pos) / 2.f;
    return true;
}

// normal points from poly to circle
bool collidePolyCircle(const body &poly, const body &circle, sf::Vector2f &norVec, float &overlap, sf::Vector2f &contact) {
    sf::Vector2f closest, diff;
    float bestDistSquared, distSquared, dist, t;
    int count = poly.verts.size();

    // closest point on the polygon outline to the circle center
    bestDistSquared = -1.f;
    for (int k = 0; k < count; k++) {
        sf::Vector2f a = poly.verts[k];
        sf::Vector2f edge = poly.verts[(k+1) % count] - a;
        t = std::clamp(dot(circle.pos - a, edge) / dot(edge, edge), 0.f, 1.f);
        sf::Vector2f point = a + edge * t;
        diff = circle.pos - point;
        distSquared = dot(diff, diff);
        if (bestDistSquared < 0.f || distSquared < bestDistSquared) {
            bestDistSquared = distSquared;
            closest = point;
        }
    }

    diff = circle.pos - closest;
    dist = std::sqrt(bestDistSquared);
    if (isInside(poly, circle.pos)) {
        // center is inside, push the circle out through the nearest edge
        if (dist == 0.f) return false;
        norVec = -diff / dist;
        overlap = circle.rad + dist;
    } else {
        if (dist >= circle.rad) return false;
        if (dist == 0.f) {
            diff = circle.pos - poly.pos;
            dist = std::sqrt(dot(diff, diff));
            if (dist == 0.f) return false;
            norVec = diff / dist;
        } else {
            norVec = diff / dist;
        }
        overlap = circle.rad - std::sqrt(bestDistSquared);
    }
    contact = closest;
    return true;
}

// normal points from circle1 to circle2
bool collideCircleCircle(const body &circle1, const body &circle2, sf::Vector2f &norVec, float &overlap, sf::Vector2f &contact) {
    sf::Vector2f relVec = circle2.pos - circle1.pos;
    float dist = std::sqrt(dot(relVec, relVec));
    if (dist == 0.f) return false;
    overlap = circle1.rad + circle2.rad - dist;
    if (overlap <= 0.f) return false;
    norVec = relVec / dist;
    contact = circle1.pos + norVec * (circle1.rad - overlap / 2.f);
    return true;
}

// normal and friction impulse at a contact point, body1 can be null for an immovable wall
void resolveContact(body *body1, body &body2, sf::Vector2f norVec, sf::Vector2f contact) {
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

void handleBodyColl(body &body1, body &body2) {
    sf::Vector2f relVec, norVec, contact;
    float overlap, reach, totalInvMass;
    bool hit;

    // skip if the bounding circles don't touch
    relVec = body2.pos - body1.pos;
    reach = body1.rad + body2.rad;
    if (dot(relVec, relVec) > reach * reach) return;

    if (body1.type == CIRCLE && body2.type == CIRCLE) {
        hit = collideCircleCircle(body1, body2, norVec, overlap, contact);
    } else if (body1.type == CIRCLE) {
        hit = collidePolyCircle(body2, body1, norVec, overlap, contact);
        norVec = -norVec;
    } else if (body2.type == CIRCLE) {
        hit = collidePolyCircle(body1, body2, norVec, overlap, contact);
    } else {
        hit = collidePolyPoly(body1, body2, norVec, overlap, contact);
    }
    if (!hit) return;

    // push back the bodies so that it's not overlapping, lighter one moves more
    totalInvMass = body1.invMass + body2.invMass;
    moveBody(body1, -norVec * (overlap * body1.invMass / totalInvMass));
    moveBody(body2, norVec * (overlap * body2.invMass / totalInvMass));

    resolveContact(&body1, body2, norVec, contact);
}

void handleEdgeColl(body &b, const float WIDTH, const float HEIGHT) {
    // inward normal and position of each wall: left, right, top, bottom
    const sf::Vector2f wallNor[4] = {{1.f, 0.f}, {-1.f, 0.f}, {0.f, 1.f}, {0.f, -1.f}};
    const float wallDist[4] = {0.f, -WIDTH, 0.f, -HEIGHT};
    sf::Vector2f contact;
    float depth, maxDepth;
    int contactCount;

    for (int w = 0; w < 4; w++) {
        if (b.type == CIRCLE) {
            maxDepth = wallDist[w] - dot(b.pos, wallNor[w]) + b.rad;
            if (maxDepth <= 0.f) continue;
            moveBody(b, wallNor[w] * maxDepth);
            resolveContact(nullptr, b, wallNor[w], b.pos - wallNor[w] * b.rad);
            continue;
        }

        // contact point is the average of the corners past the wall
        contact = sf::Vector2f();
        contactCount = 0;
        maxDepth = 0.f;
        for (auto &v : b.verts) {
            depth = wallDist[w] - dot(v, wallNor[w]);
            if (depth <= 0.f) continue;
            contact += v;
            contactCount++;
            maxDepth = std::max(maxDepth, depth);
        }
        if (contactCount == 0) continue;
        contact /= static_cast<float>(contactCount);

        moveBody(b, wallNor[w] * maxDepth);
        contact += wallNor[w] * maxDepth;
        resolveContact(nullptr, b, wallNor[w], contact);
    }
}

// corners of each polygon type, sized so the shape roughly fits a size x size box
std::vector<sf::Vector2f> makeVerts(shapeType type, float size) {
    std::vector<sf::Vector2f> verts;
    float half = size / 2.f;
    if (type == SQUARE) {
        verts = {{half, half}, {-half, half}, {-half, -half}, {half, -half}};
    } else if (type == RECTANGLE) {
        float halfH = half * genRandInt(35, 70) / 100.f;
        verts = {{half, halfH}, {-half, halfH}, {-half, -halfH}, {half, -halfH}};
    } else if (type == TRIANGLE) {
        // corners around a circle, jittered so not every triangle is equilateral
        float triRad = size / std::sqrt(3.f);
        for (int k = 0; k < 3; k++) {
            float a = sf::degrees(-90.f + 120.f * k + genRandInt(-20, 20)).asRadians();
            verts.push_back(sf::Vector2f(std::cos(a), std::sin(a)) * triRad);
        }
    }
    return verts;
}

// mass is the area, inertia of a solid shape about its centroid
void setupBody(body &b, shapeType type, float size) {
    b.type = type;
    if (type == CIRCLE) {
        b.rad = size / 2.f;
        float mass = PI * b.rad * b.rad;
        b.invMass = 1.f / mass;
        b.invInertia = 2.f / (mass * b.rad * b.rad);
        b.circleShape.setRadius(b.rad);
        b.circleShape.setOrigin(sf::Vector2f(b.rad, b.rad));
        return;
    }

    b.localVerts = makeVerts(type, size);
    int count = b.localVerts.size();

    // area and centroid, then move the corners so the centroid is the origin
    float area = 0.f;
    sf::Vector2f centroid;
    for (int k = 0; k < count; k++) {
        sf::Vector2f v1 = b.localVerts[k], v2 = b.localVerts[(k+1) % count];
        area += cross(v1, v2) / 2.f;
        centroid += (v1 + v2) * cross(v1, v2);
    }
    centroid /= 6.f * area;
    for (auto &v : b.localVerts) v -= centroid;

    float inertia = 0.f;
    b.rad = 0.f;
    for (int k = 0; k < count; k++) {
        sf::Vector2f v1 = b.localVerts[k], v2 = b.localVerts[(k+1) % count];
        inertia += cross(v1, v2) * (dot(v1, v1) + dot(v1, v2) + dot(v2, v2)) / 12.f;
        b.rad = std::max(b.rad, std::sqrt(dot(v1, v1)));
    }
    b.invMass = 1.f / std::abs(area);
    b.invInertia = 1.f / std::abs(inertia);

    b.verts.resize(count);
    b.polyShape.setPointCount(count);
    for (int k = 0; k < count; k++) b.polyShape.setPoint(k, b.localVerts[k]);
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

void randSizes(float sizes[], const float MINSIZE, const float MAXSIZE, const int AMOUNT) {
    for (int n = 0; n < AMOUNT; n++) {
        sizes[n] = genRandInt(MINSIZE, MAXSIZE);
    }
}

void randTypes(shapeType types[], const int AMOUNT) {
    for (int n = 0; n < AMOUNT; n++) {
        types[n] = static_cast<shapeType>(genRandInt(0, SHAPETYPES - 1));
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

    sf::Color shapesColors[AMOUNT];
    float rotSpd[AMOUNT];
    sf::Vector2f startSpd[AMOUNT];
    sf::Vector2f startPos[AMOUNT];
    float sizes[AMOUNT];
    shapeType types[AMOUNT];
    randSpd(startSpd, rotSpd, SPEED, AMOUNT);
    randColors(shapesColors, AMOUNT);
    randSizes(sizes, MINSIZE, MAXSIZE, AMOUNT);
    randTypes(types, AMOUNT);
    std::vector<body> bodies(AMOUNT);

    for (int n = 0; n < AMOUNT; n++) setupBody(bodies[n], types[n], sizes[n]);

    // grid cells must fit the widest body so only neighbouring cells need checking
    float maxRad = 0.f;
    for (auto &b : bodies) maxRad = std::max(maxRad, b.rad);
    const float GRIDSIZE = 2.f * maxRad;
    const int COLS = (WIDTH + GRIDSIZE -1) / GRIDSIZE;
    const int ROWS = (HEIGHT + GRIDSIZE -1) / GRIDSIZE;
    std::vector<std::vector<int>> grids(COLS * ROWS);
    randPos(startPos, maxRad, AMOUNT, WIDTH, HEIGHT);

    for (int n = 0; n < AMOUNT; n++) {
        bodies[n].spd = startSpd[n];
        bodies[n].pos = startPos[n];
        bodies[n].angle = 0.f;
        bodies[n].omega = rotSpd[n];
        bodies[n].circleShape.setFillColor(shapesColors[n]);
        bodies[n].polyShape.setFillColor(shapesColors[n]);
        updateVerts(bodies[n]);
    }

    float fps, dt, energy;
    int cx, cy, atHorizontalEdge, atVerticalEdge;
    sf::Clock clock;
    sf::Clock clock2;
    sf::Text text(font, textStr, 20);
    sf::Vertex spinMark[2];
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
            for (auto &b : bodies) {
                energy += 0.5f * dot(b.spd, b.spd) / b.invMass;
                energy += 0.5f * b.omega * b.omega / b.invInertia;
            }
            textStr = " FPS: " + numToStr(fps) + "\n KE: " + numToStr(energy / 1e6f) + "M";
            text.setString(textStr);
            clock2.restart();
        }

        for (auto &x : grids) x.clear();
        for (int n = 0; n < AMOUNT; n++) {
            cx = static_cast<int>(bodies[n].pos.x / GRIDSIZE);
            cy = static_cast<int>(bodies[n].pos.y / GRIDSIZE);
            cx = std::clamp(cx, 0, COLS - 1);
            cy = std::clamp(cy, 0, ROWS - 1);
            grids[cy * COLS + cx].push_back(n);
            atHorizontalEdge = ((cy == 0) || (cy == ROWS-1));
            atVerticalEdge = ((cx == 0) || (cx == COLS-1));

            if (!(atHorizontalEdge || atVerticalEdge)) continue;
            handleEdgeColl(bodies[n], WIDTH, HEIGHT);
        }

        for (int rowIdx = 0; rowIdx < ROWS; rowIdx++) {
            for (int colIdx = 0; colIdx < COLS; colIdx++) {
                int idx = rowIdx * COLS + colIdx;
                auto &grid1 = grids[idx];

                for (std::size_t i = 0; i < grid1.size(); i++) {
                    for (std::size_t j = i + 1; j < grid1.size(); j++) {
                        handleBodyColl(bodies[grid1[i]], bodies[grid1[j]]);
                    }
                }

                auto checkNeighbour = [&](int nx, int ny) {
                    if (nx < 0 || ny < 0 || nx >= COLS || ny >= ROWS) return;
                    auto &grid2 = grids[ny * COLS + nx];
                    for (int i : grid1) {
                        for (int j : grid2) {
                            handleBodyColl(bodies[i], bodies[j]);
                        }
                    }
                };
                checkNeighbour(colIdx+1, rowIdx);
                checkNeighbour(colIdx, rowIdx+1);
                checkNeighbour(colIdx+1, rowIdx+1);
                checkNeighbour(colIdx-1, rowIdx+1);
            }
        }

        for (auto &b : bodies) {
            b.angle += b.omega * dt;
            b.pos += b.spd * dt;
            if (b.type == CIRCLE) {
                b.circleShape.setPosition(b.pos);
                window.draw(b.circleShape);
                // a line from the center so the circle's spin is visible
                spinMark[0] = sf::Vertex{b.pos, sf::Color::Black};
                spinMark[1] = sf::Vertex{b.pos + sf::Vector2f(std::cos(b.angle), std::sin(b.angle)) * b.rad, sf::Color::Black};
                window.draw(spinMark, 2, sf::PrimitiveType::Lines);
            } else {
                updateVerts(b);
                b.polyShape.setPosition(b.pos);
                b.polyShape.setRotation(sf::radians(b.angle));
                window.draw(b.polyShape);
            }
        }
        window.draw(text);
        window.display();
    }
}
