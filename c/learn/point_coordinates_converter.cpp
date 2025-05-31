#include <iostream>
#include <cmath>
#include <limits>
#include <sstream>

struct cartesian_coord {
    double x, y;
};

struct polar_coord {
    double length, angle;
};

void handle_cin(const std::string& retry_message) {
    if (std::cin.fail()) {
        std::cout << retry_message;
        std::cin.clear();
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string format_cartesian(cartesian_coord coord) {
    std::ostringstream temp;
    temp << "(" << coord.x << ", " << coord.y << ")";
    return temp.str();
}

std::string format_polar(polar_coord coord) {
    std::ostringstream temp;
    temp << "(" << coord.length << ", " << coord.angle << "°)";
    return temp.str();
}

double get_num() {
    double temp;
    bool failed;
    while (true) {
        std::cin >> temp;
        failed = std::cin.fail();
        handle_cin("invalid input, reinput the number: ");
        if (!failed) return temp;
    }
}

cartesian_coord get_cartesian_coord() {
    cartesian_coord temp;
    bool failed;
    while (true) {
        std::cin >> temp.x >> temp.y;
        failed = std::cin.fail();
        handle_cin("invalid input, reinput the coordinate (format: x y): ");
        if (!failed) return temp;
    }
}

polar_coord get_polar_coord() {
    polar_coord temp;
    bool failed;
    while (true) {
        std::cin >> temp.length >> temp.angle;
        failed = std::cin.fail();
        handle_cin("invalid input, reinput the polar coordinate (format: length angle): ");
        if (!failed) {
            temp.angle = std::fmod(temp.angle, 360.0); // normalize
            if (temp.angle < 0) temp.angle += 360.0;

            if (temp.length < 0) temp.angle += 180;
            temp.angle = std::fmod(temp.angle, 360.0); // renormalize
            temp.length = std::abs(temp.length);
            return temp;
        }
    }
}

polar_coord cartesian_to_polar(cartesian_coord coord_in) {
    polar_coord temp;
    double rad;

    rad = atan2(coord_in.y, coord_in.x);
    temp.angle = rad * 180.0 / M_PI;
    temp.length = sqrt(coord_in.x * coord_in.x + coord_in.y * coord_in.y);
    return temp;
}

cartesian_coord polar_to_cartesian(polar_coord value) {
    cartesian_coord temp;
    double rad;

    rad = value.angle * M_PI / 180.0;
    temp.x = value.length * cos(rad);
    temp.y = value.length * sin(rad);
    return temp;
}

int main() {
    int choice;
    std::string result;
    polar_coord polar;
    cartesian_coord cartesian;

    std::cout << "1. cartesian to polar coordinate" << '\n'
            << "2. polar to cartesian coordinate" << '\n'
            << "input one of the valid option: ";
    choice = static_cast<int>(std::floor(get_num()));
    if (choice == 1) {
        std::cout << "input the cartesian coordinate (format: x y): "; 
        cartesian = get_cartesian_coord();
        polar = cartesian_to_polar(cartesian);
        result = format_polar(polar);
        std::cout << "result: " << result << '\n';
    } else if (choice == 2) {
        std::cout << "input the polar coordinate (format: length degrees): "; 
        polar = get_polar_coord();
        cartesian = polar_to_cartesian(polar);
        result = format_cartesian(cartesian);
        std::cout << "result: " << result << '\n';
    } else {
        std::cout << "invalid option" << '\n';
        return 1;
    }
    return 0;
}
