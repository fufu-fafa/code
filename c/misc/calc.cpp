#include <iostream>
using namespace std;
int main() {
    double num1, num2;
    char op;
    cout << "enter first number: ";
    cin >> num1;
    cout << "enter an operator (+, -, *, /): ";
    cin >> op;
    cout << "enter second number: ";
    cin >> num2;
    switch (op) {
        case '+':
            cout << "result: " << num1 + num2 << endl;
            break;
        case '-':
            cout << "result: " << num1 - num2 << endl;
            break;
        case '*':
            cout << "result: " << num1 * num2 << endl;
            break;
        case '/':
            if (num2 != 0)
                cout << "result: " << num1 / num2 << endl;
            else
                cout << "err: division by zero" << endl;
            break;
        default:
            cout << "invalid operator" << endl;
    }
    return 0;
}
