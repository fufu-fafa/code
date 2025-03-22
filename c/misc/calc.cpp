#include <iostream>
using namespace std;
int main() {
    double num1, num2;
    char op;
    cout << "enter first number: ";
    if (!(cin >> num1)) {
        cout << "err: invalid number" << endl;
        return 1;
    }
    cout << "enter an operator (+, -, *, /): ";
    cin >> op;
    if (op != '+' && op != '-' && op != '*' && op != '/') {
        cout << "err: invalid operator" << endl;
        return 1;
    }
    cout << "enter second number: ";
    if (!(cin >> num2)) {
        cout << "err: invalid number" << endl;
        return 1;
    }
    if (op == '/' && num2 == 0) {
        cout << "err: division by zero" << endl;
        return 1;
    }
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
            cout << "result: " << num1 / num2 << endl;
            break;
    }
    return 0;
}
