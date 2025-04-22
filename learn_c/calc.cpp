#include <iostream>
#include <string>
using namespace std;

int main() {
    double num1, num2, result;
    string chosen_op;
    cout << "input the first number: ";
    cin >> num1;

    cout << "input one of the valid operator: <+, -, *, /> ";
    cin >> chosen_op;

    cout << "input the second number: ";
    cin >> num2;
    if (chosen_op == "+") {
        result = num1 + num2;
        cout << "result: " << result << endl;
    } else if (chosen_op == "-") {
        result = num1 - num2;
        cout << "result: " << result << endl;
    } else if (chosen_op == "*") {
        result = num1 * num2;
        cout << "result: " << result << endl;
    } else if (chosen_op == "/") {
        result = num1 / num2;
        cout << "result: " << result << endl;
    } else {
        cout << "invalid operator" << endl;
    }
    return 0;
}
