#include <iostream>
#include <limits>

struct vec2 {
    double x, y;
};

struct line {
    vec2 start, end;
    double gradient;
};

vec2 get_vec() {
    vec2 temp;
    while (true) {
        std::cin >> temp.x >> temp.y;
        if (std::cin.fail()) {
            std::cout << "invalid number" << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return temp;
        }
    } 
}

vec2 calculate(line line1, line line2) {
    vec2 result;
    double offset1, offset2;
    if (line1.gradient == line2.gradient) {
        result.x = result.y = std::numeric_limits<double>::quiet_NaN();
    } else if (std::isinf(line1.gradient)) {
        offset2 = line2.start.y - line2.gradient * line2.start.x;
        result.x = line1.start.x;
        result.y = line2.gradient * result.x + offset2;
    } else if (std::isinf(line2.gradient)) {
        offset1 = line1.start.y - line1.gradient * line1.start.x;
        result.x = line2.start.x;
        result.y = line1.gradient * result.x + offset1;
    } else {
        offset1 = line1.start.y - line1.gradient * line1.start.x;
        offset2 = line2.start.y - line2.gradient * line2.start.x; 
        result.x = (offset2 - offset1) / (line1.gradient - line2.gradient);
        result.y = line1.gradient * result.x + offset1;
    }
    return result;
}

double get_gradient(vec2 start, vec2 end) {
    double gradient;
    if (end.x == start.x) gradient = std::numeric_limits<double>::infinity();
    else gradient = (end.y - start.y) / (end.x - start.x);
    return gradient;
}

int main() {
    std::cout << "for the first line" << '\n';
    std::cout << "enter the first coordinate (format: x y): " << '\n';
    vec2 line1_start = get_vec();
    std::cout << "enter the second coordinate (format: x y): " << '\n';
    vec2 line1_end = get_vec();
    if (line1_start.x == line1_end.x && line1_start.y == line1_end.y) {
        std::cout << "first and second coordinate must be different" << '\n';
        return 1;
    }
    line line1 = {line1_start, line1_end, get_gradient(line1_start, line1_end)};

    std::cout << '\n' << "for the second line" << '\n';
    std::cout << "enter the first coordinate (format: x y): " << '\n';
    vec2 line2_start = get_vec();
    std::cout << "enter the second coordinate (format: x y): " << '\n';
    vec2 line2_end = get_vec();
    if (line2_start.x == line2_end.x && line2_start.y == line2_end.y) {
        std::cout << "first and second coordinate must be different" << '\n';
        return 1;
    }
    line line2 = {line2_start, line2_end, get_gradient(line2_start, line2_end)};

    vec2 result = calculate(line1, line2);
    if (std::isnan(result.x) || std::isnan(result.y)) std::cout << "lines do not intersect" << '\n';
    else std::cout << "intersection point: (" << result.x << ", " << result.y << ")" << '\n';

    return 0;
}
