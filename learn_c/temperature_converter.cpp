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
        if (temperature == "celcius" || temperature == "fahrenheit" || temperature == "kelvin") return temperature;
        else {
            std::cin.ignore(100000, '\n');
            std::cout << "not a valid temperature (case sensitive)" << '\n'
                      << "valid temperature: celcius, fahrenheit, kelvin" << '\n';
        }
    }
}

int main() {
    std::string first_temperature, second_temperature;
    double input_measurement, output_measurement;
    first_temperature = get_temperature("convert from");
    second_temperature = get_temperature("convert into");

    input_measurement = get_num(first_temperature);
    return 0;
}
