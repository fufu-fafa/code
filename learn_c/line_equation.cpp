#include <iostream>
#include <iomanip>
#include <limits>

struct coordinate {
    double x, y;
};

struct line {
    coordinate start, end;
    double gradient, offset;
};

double get_gradient(coordinate start, coordinate end) {
    if (end.x == start.x) return std::numeric_limits<double>::infinity();
    else return (end.y - start.y) / (end.x - start.x);
}

coordinate get_coordinate() {
    coordinate temp;
    while (true) {
        std::cin >> temp.x >> temp.y;
        if (std::cin.fail()) {
            std::cout << "invalid number, reinput the coordinate (format: x y): " << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return temp;
        }
    }
}

int main() {
    line line1;
    std::cout << "enter the first coordinate for the line: " << '\n';
    line1.start = get_coordinate();
    std::cout << "enter the second coordinate for the line: " << '\n';
    line1.end = get_coordinate();
    line1.gradient = get_gradient(line1.start, line1.end);

    if (std::isinf(line1.gradient)) {
        std::cout << "x = " << line1.start.x << '\n';
    } else {
        line1.offset = line1.start.y - line1.gradient * line1.start.x;
        std::cout << std::fixed << std::setprecision(2);
        if (line1.offset > 0) std::cout << "y = " << line1.gradient << "x + " << line1.offset << '\n';
        else if (line1.offset == 0) std::cout << "y = " << line1.gradient << "x" << '\n';
        else std::cout << "y = " << line1.gradient << "x - " << -line1.offset << '\n';
    }
    return 0;
}
