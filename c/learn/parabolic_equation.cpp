#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>

struct coordinate {
    double x, y;
};

struct parabole {
    double p;
    coordinate focus_point;
};

std::string format_double(double value) {
    std::ostringstream temp;

    if (value < 0) temp << "- " << -value;
    else temp << "+ " << value;
    return temp.str();
}

double get_num() {
    double temp;

    while (true) {
        std::cin >> temp;
        if (std::cin.fail()) {
            std::cout << "invalid input, reinput the number: ";
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
            std::cout << "invalid input, reinput the coordinate (format: x y): " << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return temp;
        }
    }
}

int main() {
    parabole result;
    return 0;
}
