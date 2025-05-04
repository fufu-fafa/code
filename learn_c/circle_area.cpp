#include <iostream>
#include <limits>

double get_num() {
    double temp;
    while (true) {
        std::cout << "enter the radius of the circle: ";
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

int main() {
    const double pi = 3.1415;
    double radius, result;
    radius = get_num();
    result = pi * radius * radius;
    std::cout << "result: " << result;
}
