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
            std::cout << "not a valid temperature (case sensitive)" << '\n'
                      << "valid temperature: celsius, fahrenheit, kelvin" << '\n';
        }
    }
}

double calculate(std::string first_temperature, std::string second_temperature, double input_measurement) {
    double output_measurement;
    if (first_temperature == "celsius") {
        if (second_temperature == "fahrenheit") output_measurement = input_measurement * 9.0/5.0 + 32;
        else if (second_temperature == "kelvin") output_measurement = input_measurement + 273.15;
        else output_measurement = input_measurement;
    } else if (first_temperature == "fahrenheit") {
        if (second_temperature == "celsius") output_measurement = (input_measurement - 32) * 5.0/9.0;
        else if (second_temperature == "kelvin") output_measurement = (input_measurement - 32) * 5.0/9.0 + 273.15;
        else output_measurement = input_measurement;
    } else {
        if (second_temperature == "celsius") output_measurement = input_measurement - 273.15;
        else if (second_temperature == "fahrenheit") output_measurement = (input_measurement - 273.15) * 9.0/5.0 + 32;
        else output_measurement = input_measurement;
    }
    return output_measurement;
}

int main() {
    std::string first_temperature, second_temperature;
    double input_measurement, output_measurement;
    first_temperature = get_temperature("convert from");
    second_temperature = get_temperature("convert into");

    input_measurement = get_num(first_temperature);
    output_measurement = calculate(first_temperature, second_temperature, input_measurement);
    std::cout << "result: " << output_measurement << '\n';
    return 0;
}
