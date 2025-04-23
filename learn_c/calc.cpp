#include <iostream>
#include <string>
using namespace std;

float get_float(string var) {
    float temp;
    while (true) {
        cout << "enter the " << var << " number: ";
        cin >> temp;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "invalid number" << endl;
        } else {
            break;
        }
    }
    return temp;
}

float calculate(float num1, float num2) {

}

int main() {
    float num1, num2, result;
    string chosen_op;
    num1 = get_float("first");

    cout << "input one of the valid operator: <+, -, *, /> ";
    cin >> chosen_op;

    if (chosen_op != "+" && chosen_op != "-" && chosen_op != "*" && chosen_op != "/") {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "invalid operator" << endl;
        cout << "input one of the valid operator: <+, -, *, /> "
        cin >> chosen_op;
    }

    num2 = get_float("second");

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
