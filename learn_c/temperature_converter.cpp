#include <iostream>

double get_num(std::string temperature) {
    double temp;
    while (true){
        std::cout << "enter the temperature in " << temperature << ": ";
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
        if (temperature == "celsius" || temperature == "fahrenheit" || temperature == "kelvin") return temperature;
        else {
            std::cin.ignore(100000, '\n');
            std::cout << "input one of the valid measurement: 'celsius', 'fahrenheit', 'kelvin'" << '\n';
        }
    }
}

double into_celsius(std::string convert_from, double input_value) {
    if (convert_from == "celsius") return input_value;
    else if (convert_from == "fahrenheit") return (input_value - 32) * 5.0/9.0;
    else if (convert_from == "kelvin") return input_value - 273.15;
}

double from_celsius(std::string convert_into, double celsius_value) {
    if (convert_into == "celsius") return celsius_value;
    else if (convert_into == "fahrenheit") return (celsius_value * 9.0/5.0) + 32;
    else if (convert_into == "kelvin") return celsius_value + 273.15;
}

double calculate(std::string convert_from, std::string convert_into, double input_value) {
    double celsius_value, output_value;
    celsius_value = into_celsius(convert_from, input_value);
    output_value = from_celsius(convert_into, celsius_value);
    return output_value;
}

int main() {
    std::string convert_from, convert_into;
    double input_value, output_value;
    convert_from = get_temperature("convert from");
    convert_into = get_temperature("convert into");

    input_value = get_num(convert_from);
    output_value = calculate(convert_from, convert_into, input_value);
    std::cout << "result: " << output_value << '\n';
    return 0;
}
