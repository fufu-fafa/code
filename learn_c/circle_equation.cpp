#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>

struct coordinate {
    double x, y;
};

struct circle {
    double r;
    coordinate center;
};

struct line {
    double gradient;
    coordinate midpoint;
};

std::string format_double(double value) {
    std::ostringstream temp;
    if (value < 0) temp << "+ " << -value;
    else temp << "- " << value;
    return temp.str();
}

double get_gradient(coordinate start, coordinate end) {
    if (end.x == start.x) return std::numeric_limits<double>::infinity();
    else return (end.y - start.y) / (end.x - start.x);
}

std::string get_equation(circle result) {
    std::ostringstream temp;
    temp << "(x " << format_double(result.center.x) << ")^2 + " 
         << "(y " << format_double(result.center.y) << ")^2 = " << std::pow(result.r, 2);
    return temp.str();
}

double get_num() {
    double temp;
    while (true) {
        std::cin >> temp;
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

coordinate get_coordinate() {
    coordinate temp;
    while (true) {
        std::cin >> temp.x >> temp.y;
        if (std::cin.fail()) {
            std::cout << "invalid number, reinput the coordinate (format: x y):" << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return temp;
        }
    }
}

coordinate get_intersection(line line1, line line2) {
    coordinate result;
    double offset1, offset2;
    if (std::isinf(line1.gradient)) {
        offset2 = line2.midpoint.y - line2.gradient * line2.midpoint.x;
        result.x = line1.midpoint.x;
        result.y = line2.gradient * result.x + offset2;
    } else if (std::isinf(line2.gradient)) {
        offset1 = line1.midpoint.y - line1.gradient * line1.midpoint.x;
        result.x = line2.midpoint.x;
        result.y = line1.gradient * result.x + offset1;
    } else {
        offset1 = line1.midpoint.y - line1.gradient * line1.midpoint.x;
        offset2 = line2.midpoint.y - line2.gradient * line2.midpoint.x; 
        result.x = (offset2 - offset1) / (line1.gradient - line2.gradient);
        result.y = line1.gradient * result.x + offset1;
    }
    return result;
}

line get_perpendicular(line input) {
    line temp;
    temp.midpoint = input.midpoint;

    if (std::isinf(input.gradient)) temp.gradient = 0;
    else if (input.gradient == 0) temp.gradient = std::numeric_limits<double>::infinity();
    else {
        temp.gradient = -1 / input.gradient;
    }
    return temp;
}

circle option1() {
    circle temp;
    std::cout << "enter the center point coordinate (format: x y):" << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the radius of the circle: ";
    temp.r = get_num();
    return temp;
}

circle option2() {
    circle temp;
    double determinant;
    coordinate a, b, c, result;
    line ab, bc, perp_ab, perp_bc;

    std::cout << "enter the first point (format: x y):" << '\n';
    a = get_coordinate();
    std::cout << "enter the second point (format: x y):" << '\n';
    b = get_coordinate();
    std::cout << "enter the third point (format: x y):" << '\n';
    c = get_coordinate();
    determinant = (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));

    if ((a.x == b.x && a.y == b.y) || (b.x == c.x && b.y == c.y) || (c.x == a.x && c.y == a.y)) {
        std::cout << "points must be distinct" << '\n';
        exit(1);
    } else if (determinant == 0) {
        std::cout << "points are collinear, unable to form circle" << '\n';
        exit(1);
    }

    ab.midpoint = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    ab.gradient = get_gradient(a, b);
    perp_ab = get_perpendicular(ab);

    bc.midpoint = {(b.x + c.x) / 2, (b.y + c.y) / 2};
    bc.gradient = get_gradient(b, c);
    perp_bc = get_perpendicular(bc);

    temp.center = get_intersection(perp_ab, perp_bc);
    temp.r = sqrt(std::pow(a.x - temp.center.x, 2) + std::pow(a.y - temp.center.y, 2));
    return temp;
}

circle option3() {
    circle temp;
    coordinate edge;
    std::cout << "enter the center point coordinate (format: x y):" << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the coordinate of a point on the circle:" << '\n';
    edge = get_coordinate();

    if (temp.center.x == edge.x && temp.center.y == edge.y) {
        std::cout << "both coordinate must be different" << '\n';
        exit(1);
    }

    temp.r = sqrt(std::pow(edge.x - temp.center.x, 2) + std::pow(edge.y - temp.center.y, 2));
    return temp;
}

circle option4() {
    circle temp;
    coordinate endpoint1, endpoint2;
    std::cout << "enter the first diameter endpoint (format: x y):" << '\n';
    endpoint1 = get_coordinate();
    std::cout << "enter the second diameter endpoint (format: x y):" << '\n';
    endpoint2 = get_coordinate();
    temp.center.x = (endpoint1.x + endpoint2.x) / 2;
    temp.center.y = (endpoint1.y + endpoint2.y) / 2;
    temp.r = sqrt(std::pow(endpoint2.x - endpoint1.x, 2) + std::pow(endpoint2.y - endpoint1.y, 2)) / 2;
    return temp;
}

int main() {
    circle result;
    int choice;
    std::string equation;
    std::cout << "1. center point and radius" << '\n'
              << "2. three points on the circle" << '\n'
              << "3. center and one point on the circle" << '\n'
              << "4. diameter endpoints" << '\n'
              << "5. exit" << '\n'
              << "enter one of the option above: ";

    choice = static_cast<int>(std::floor(get_num()));

    if (choice == 1) result = option1();
    else if (choice == 2) result = option2();
    else if (choice == 3) result = option3();
    else if (choice == 4) result = option4();
    else if (choice == 5) return 0;
    else {
        std::cout << "not a valid option" << '\n';
        return 1;
    }

    equation = get_equation(result);
    std::cout << "circle equation: " << equation << '\n';
    return 0;
}
