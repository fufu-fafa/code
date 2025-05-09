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

std::string format_double(double value) {
    std::ostringstream temp;
    if (value < 0) temp << "+ " << -value;
    else temp << "- " << value;
    return temp.str();
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

circle option1() {
    circle temp;
    std::cout << "enter the center point coordinate (format: x y):" << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the radius of the circle:";
    temp.r = get_num();
    return temp;
}

circle option2() {
    circle temp;
    coordinate p1, p2, p3;
    double determinant, p1_sq, p2_sq, p3_sq;

    std::cout << "enter the first point (format: x y):" << '\n';
    p1 = get_coordinate();
    std::cout << "enter the second point (format: x y):" << '\n';
    p2 = get_coordinate();
    std::cout << "enter the third point (format: x y):" << '\n';
    p3 = get_coordinate();

    determinant = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p1.y));
    if (determinant == 0) {
        std::cout << "points are collinear, cannot form a circle" << '\n';
        exit(1);
    }

    p1_sq = std::pow(p1.x, 2) + std::pow(p1.y, 2);
    p2_sq = std::pow(p2.x, 2) + std::pow(p2.y, 2);
    p3_sq = std::pow(p3.x, 2) + std::pow(p3.y, 2);

    temp.center.x = (p1_sq * (p2.y - p3.y) + p2_sq * (p3.y - p1.y) + p3_sq * (p1.y - p2.y)) / determinant;
    temp.center.y = (p1_sq * (p3.x - p2.x) + p2_sq * (p1.x - p3.x) + p3_sq * (p2.x - p3.x)) / determinant;
    temp.r = std::sqrt(std::pow(p1.x - temp.center.x, 2) + std::pow(p1.y - temp.center.y, 2));
    return temp;
}

circle option3() {
    circle temp;
    coordinate edge;
    std::cout << "enter the center point coordinate (format: x y):" << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the coordinate of a point on the circle:";
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
