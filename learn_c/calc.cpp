#include <iostream>

double get_num(std::string which_num) {
    double temp;
    while (true) {
        std::cout << "input the " << which_num << " number: ";
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

std::string get_op() {
    std::string temp;
    while (true) {
        std::cout << "input a valid operator: (+, -, *, /) ";
        std::cin >> temp;
        if (temp == "+" || temp == "-" || temp == "*" || temp == "/") return temp;
        else std::cout << "invalid operator" << '\n';
    }
}

double calculate(double num1, double num2, std::string op) {
    double result;
    if (op == "+") result = num1 + num2;
    else if (op == "-") result = num1 - num2;
    else if (op == "*") result = num1 * num2;
    else result = num1 / num2;
    return result;
}

int main() {
    double num1, num2, result;
    std::string chosen_op;
    num1 = get_num("first");
    chosen_op = get_op();
    num2 = get_num("second");
    if (chosen_op == "/" && num2 == 0) return 1;
    result = calculate(num1, num2, chosen_op);
    std::cout << "result: " << result << '\n';
    return 0;
}
