#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <set>

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

std::string validate_input(const std::set<std::string>& valid_units) {
    std::string temp;
    while (true) {
        std::cin >> temp;
        std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char x) {return std::tolower(x);});

        if (valid_units.find(temp) != valid_units.end()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return temp;
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "invalid unit" << '\n';
        }
    }
}

int main() {
    std::set<std::string> valid_units = {"kilometer", "meter", "centimeter", "mile", "yard", "foot", "inch"};
    double value_in, value_out;
    std::string input_unit, output_unit;
    std::cout << "valid units:" << '\n' << "kilometer, meter, centimeter, mile, yard, foot, inch" << '\n' << '\n';

    std::cout << "enter the unit to convert from: ";
    input_unit = validate_input(valid_units);
    std::cout << "enter the unit to convert into: ";
    output_unit = validate_input(valid_units);
    std::cout << "enter the value in " << input_unit << ": ";
    value_in = get_num();

    return 0;
}
