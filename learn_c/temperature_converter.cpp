#include <iostream>

struct temp_limits {
    double boil, freeze;
};

double get_num() {
    double temp;
    while (true){
        std::cin >> temp;
        if (std::cin.fail()) {
            std::cout << "invalid number" << '\n';
            std::cin.clear();
            std::cin.ignore(100000, '\n');
        } else {
            std::cin.ignore(100000, '\n');
            return temp;
        }
    }
}

std::string get_temperature(std::string which) {
    std::string temperature;
    while (true) {
        std::cout << "enter the temperature to " << which << ": ";
        std::cin >> temperature;
        if (temperature == "celsius" || temperature == "fahrenheit" || temperature == "kelvin" || temperature == "custom") return temperature;
        else {
            std::cin.ignore(100000, '\n');
            std::cout << "input one of the valid measurement: 'celsius', 'fahrenheit', 'kelvin', 'custom'" << '\n';
        }
    }
}

double calculate(double input_value, temp_limits limit_in, temp_limits limit_out) {
    double celsius_value, output_value;
    celsius_value = ((input_value - limit_in.freeze) / (limit_in.boil - limit_in.freeze) * 100);
    output_value = ((limit_out.boil - limit_out.freeze) / 100) * celsius_value + limit_out.freeze;
    return output_value;
}

temp_limits parser(std::string user_in) {
    temp_limits range;
    if (user_in == "celsius") range = {100.0, 0.0};
    else if (user_in == "fahrenheit") range = {212, 32.0}; 
    else if (user_in == "kelvin") range = {373.15, 273.15};
    else {
        while (true) {
            std::cout << "enter the boiling point in that unit: ";
            range.boil = get_num();
            std::cout << "enter the freezing point in that unit: ";
            range.freeze = get_num();
            if (range.boil >= range.freeze) return range;
        }
    }
    return range;
}

int main() {
    std::string convert_from, convert_into;
    double input_value, output_value;

    convert_from = get_temperature("convert from");
    temp_limits limit_in = parser(convert_from);
    convert_into = get_temperature("convert into");
    temp_limits limit_out = parser(convert_into);

    std::cout << "input the value in " << convert_from << ": ";
    input_value = get_num();

    output_value = calculate(input_value, limit_in, limit_out);
    std::cout << "result: " << output_value << '\n';
    return 0;
}
