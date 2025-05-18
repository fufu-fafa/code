#include <iostream>
#include <sstream>
#include <vector>
#include <limits>

struct coordinate {
    double x, y;
};

struct line {
    coordinate point;
    double gradient, offset;
};

std::string format_double(double value) {
    std::ostringstream temp;
    if (value < 0) temp << "- " << -value;
    else temp << "+ " << value;
    return temp.str();
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

std::vector<coordinate> get_points() {
    std::vector<coordinate> points;
    coordinate input;
    char choice;

    while (true) {
        std::cout << "enter a coordinate (format: x y): ";
        input = get_coordinate();
        points.push_back(input);

        std::cout << "add another coordinate? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'n' || choice == 'N') return points;
    }
}

line calculate(const std::vector<coordinate>& points) {
    line temp;
    double denominator;
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_squared_x = 0;
    int n = points.size();

    for (const coordinate& point : points) {
        sum_x += point.x;
        sum_y += point.y;
        sum_xy += point.x * point.y;
        sum_squared_x += point.x * point.x;
    }
    denominator = n * sum_squared_x - sum_x * sum_x;

    temp.point.x = sum_x / n;
    temp.point.y = sum_y / n;
    if (denominator == 0) {
        temp.gradient = std::numeric_limits<double>::infinity();
        temp.offset = 0;
    } else {
        temp.gradient = (n * sum_xy - sum_x * sum_y) / denominator;
        temp.offset = temp.point.y - temp.gradient * temp.point.x;
    }
    return temp;
}

int main() {
    std::vector<coordinate> points;
    line line1;

    points = get_points();
    if (points.size() < 2) {
        std::cout << "unable to get line equation with one coordinate" << '\n';
        return 1;
    }

    line1 = calculate(points);

    std::cout << '\n' << "gradient = " << line1.gradient << '\n';
    if (std::isinf(line1.gradient)) {
        std::cout << "x = " << line1.point.x << '\n';
    } else {
        if (line1.offset == 0) std::cout << "y = " << line1.gradient << "x" << '\n';
        else std::cout << "y = " << line1.gradient << "x " << format_double(line1.offset) << '\n';
    }
    return 0;
}
