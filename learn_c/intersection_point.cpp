#include <iostream>
#include <limits>

struct vec2 {
    double x, y;
};

vec2 get_vec() {
    vec2 temp;
    while (true) {
        std::cin >> temp.x >> temp.y;
        if (std::cin.fail()) {
            std::cout << "invalid number" << '\n';
            std::cin.clear();
            std::cin.ignore(100000, '\n');
        } else {
            std::cin.ignore(100000, '\n');
            return temp;
        }
    } 
}

vec2 calculate(vec2 coordinate1, vec2 coordinate2, double gradient1, double gradient2) {
    vec2 result;
    double offset1, offset2;
    if (gradient1 == gradient2) {
        result.x = result.y = std::numeric_limits<double>::quiet_NaN();
    } else if (std::isinf(gradient1)) {
        offset2 = coordinate2.y - gradient2 * coordinate2.x;
        result.x = coordinate1.x;
        result.y = gradient2 * result.x + offset2;
    } else if (std::isinf(gradient2)) {
        offset1 = coordinate1.y - gradient1 * coordinate1.x;
        result.x = coordinate2.x;
        result.y = gradient1 * result.x + offset1;
    } else {
        offset1 = coordinate1.y - gradient1 * coordinate1.x;
        offset2 = coordinate2.y - gradient2 * coordinate2.x; 

        result.x = (offset2 - offset1) / (gradient1 - gradient2);
        result.y = gradient1 * result.x + offset1;
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
    double line1_gradient = get_gradient(line1_start, line1_end);

    std::cout << '\n' << "for the second line" << '\n';
    std::cout << "enter the first coordinate (format: x y): " << '\n';
    vec2 line2_start = get_vec();
    std::cout << "enter the second coordinate (format: x y): " << '\n';
    vec2 line2_end = get_vec();
    if (line2_start.x == line2_end.x && line2_start.y == line2_end.y) {
        std::cout << "first and second coordinate must be different" << '\n';
        return 1;
    }
    double line2_gradient = get_gradient(line2_start, line2_end);

    vec2 result = calculate(line1_start, line2_start, line1_gradient, line2_gradient);
    if (std::isnan(result.x) || std::isnan(result.y)) std::cout << "lines do not intersect" << '\n';
    else std::cout << "result: (" << result.x << ", " << result.y << ")" << '\n';

    return 0;
}
