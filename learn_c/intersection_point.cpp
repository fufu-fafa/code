#include <iostream>
#include <limits>

struct vec2 {
    double x, y;
};

struct line {
    vec2 start, end;
    double gradient;
};

double get_gradient(vec2 start, vec2 end) {
    double gradient;
    if (end.x == start.x) return std::numeric_limits<double>::infinity();
    else return (end.y - start.y) / (end.x - start.x);
}

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

line get_line(std::string which) {
    line temp;
    std::cout << "for the " << which << " line" << '\n';
    std::cout << "enter the first coordinate (format: x y): " << '\n';
    temp.start = get_vec();
    std::cout << "enter the second coordinate (format: x y): " << '\n';
    temp.end = get_vec();
    if (temp.start.x == temp.end.x && temp.start.y == temp.end.y) {
        std::cout << "first and second coordinate must be different" << '\n';
        exit(1);
    }
    temp.gradient = get_gradient(temp.start, temp.end);
    return temp;
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

int main() {
    line line1 = get_line("first");
    line line2 = get_line("second");

    vec2 result = calculate(line1, line2);
    if (std::isnan(result.x) || std::isnan(result.y)) std::cout << "lines do not intersect" << '\n';
    else std::cout << "intersection point: (" << result.x << ", " << result.y << ")" << '\n';

    return 0;
}
