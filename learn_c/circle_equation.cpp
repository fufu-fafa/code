#include <iostream>
#include <limits>
#include <cmath>

struct coordinate {
    double x, y;
};

struct circle {
    double r;
    coordinate center;
};

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
            std::cout << "invalid number, reinput the coordinate (format: x y): " << '\n';
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
    std::cout << "enter the center point coordinate (format: x y): " << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the radius of the circle: ";
    temp.r = get_num();
    return temp;
}

circle option2() {}

circle option3() {
    circle temp;
    coordinate edge;
    std::cout << "enter the center point coordinate (format: x y): " << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the coordinate of a point on the circle: ";
    edge = get_coordinate();

    if (temp.center.x == edge.x && temp.center.y == edge.y) {
        std::cout << "both coordinate must be different" << '\n';
        exit();
    }

    temp.r = sqrt(pow(edge.x - temp.center.x, 2) + pow(edge.y - temp.center.y, 2));
    return temp;
}

cirlce option4() {
    circle temp;
    coordinate endpoint1 endpoint2;
    std::cout << "enter the first diameter endpoint (format: x y): " << '\n';
    endpoint1 = get_coordinate();
    std::cout << "enter the second diameter endpoint (format: x y): " << '\n';
    endpoint2 = get_coordinate();
    temp.center.x = (endpoint1.x + endpoint2.x) / 2;
    temp.center.y = (endpoint1.y + endpoint2.y) / 2;
    temp.r = sqrt(pow(endpoint2.x - endpoint1.x, 2) + pow(endpoint2.y - endpoint1.y, 2)) / 2;
    return temp;
}

int main() {
    int choice;
    std::cout << "1. center point and radius" << '\n'
              << "2. three points on the circle" << '\n'
              << "3. center and one point on the circle" << '\n'
              << "4. diameter endpoints" << '\n'
              << "enter one of the option above: ";

    choice = static_cast<int>(std::floor(get_num()));

    if (choice != 1 && choice != 2 && choice != 3 && choice !=4) {
        std::cout << "not a valid option" << '\n';
        return 1;
    }
    return 0;
}
