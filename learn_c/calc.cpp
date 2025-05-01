#include <iostream>
#include <limits>

float get_num(std::string which_num) {
    float temp;
    while (true) {
        std::cout << "input the " << which_num << " number: ";
        std::cin >> temp;
        if (std::cin.fail()) {
            std::cout << "invalid number" << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else return temp;
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

float calculate(float num1, float num2, std::string op) {
    float result;
    if (op == "+") result = num1 + num2;
    else if (op == "-") result = num1 - num2;
    else if (op == "*") result = num1 * num2;
    else result = num1 / num2;
    return result;
}

int main() {
    float num1, num2, result;
    std::string chosen_op;
    num1 = get_num("first");
    chosen_op = get_op();
    num2 = get_num("second");
    if (chosen_op == "/" && num2 == 0) return 1;
    result = calculate(num1, num2, chosen_op);
    std::cout << "result: " << result << '\n';
    return 0;
}
