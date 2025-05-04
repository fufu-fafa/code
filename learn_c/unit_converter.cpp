#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <set>

struct unit_struct {
    std::string in, out;
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

double into_meter(std::string input_unit, double value_in) {
    if (input_unit == "meter") return value_in;
    else if (input_unit == "kilometer") return value_in * 1000;
    else if (input_unit == "centimeter") return value_in * 0.01;
    else if (input_unit == "mile") return value_in * 1609.344;
    else if (input_unit == "yard") return value_in * 0.9144;
    else if (input_unit == "foot") return value_in * 0.3048;
    else if (input_unit == "inch") return value_in * 0.0254;
    else return 0.0;
}

double from_meter(std::string output_unit, double in_meter) {
    if (output_unit == "meter") return in_meter;
    else if (output_unit == "kilometer") return in_meter / 1000;
    else if (output_unit == "centimeter") return in_meter / 0.01;
    else if (output_unit == "mile") return in_meter / 1609.344;
    else if (output_unit == "yard") return in_meter / 0.9144;
    else if (output_unit == "foot") return in_meter / 0.3048;
    else if (output_unit == "inch") return in_meter / 0.0254;
    else return 0.0;
}

double calculate(unit_struct unit, double value_in) {
    double in_meter = into_meter(unit.in, value_in);
    double result = from_meter(unit.out, in_meter);
    return result;
}

int main() {
    std::set<std::string> valid_units = {"kilometer", "meter", "centimeter", "mile", "yard", "foot", "inch"};
    double value_in, value_out;
    unit_struct unit;
    std::cout << "valid units:" << '\n' << "kilometer, meter, centimeter, mile, yard, foot, inch" << '\n' << '\n';

    std::cout << "enter the unit to convert from: ";
    unit.in = validate_input(valid_units);
    std::cout << "enter the unit to convert into: ";
    unit.out = validate_input(valid_units);
    std::cout << "enter the value in " << unit.in << ": ";
    value_in = get_num();
    result = calculate(unit, value_in);
    std::cout << "result in " << unit.out << ": " << result << '\n';

    return 0;
}
