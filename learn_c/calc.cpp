#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <set>
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

string get_operator() {
    const set<string> valid_op = {"+", "-", "*", "/"};
    string temp;
    while (true) {
        cout << "input an operator: <+, -, *, /> ";
        cin >> temp;
        if (valid_op.count(temp)) return temp;
        else cout << "invalid operator" << endl; 
    }
}

int check(float num2, string op) {
    int valid = 1;
    if (num2 == 0 && op == "/") valid = 0;
    return valid;
}

float calculate(float num1, float num2, string op) {
    float result;
    if (op == "+") result = num1 + num2;
    else if (op == "-") result = num1 - num2;
    else if (op == "*") result = num1 * num2;
    else if (op == "/") result = num1 / num2;
    return result;
}

int main() {
    int val;
    float num1, num2, result;
    string chosen_op;
    num1 = get_float("first");
    chosen_op = get_operator();
    num2 = get_float("second");
    val = check(num2, chosen_op);
    if (val != 1) {
        cout << "unable to divide by zero" << endl;
        exit(1);
    }
    result = calculate(num1, num2, chosen_op);
    cout << "result: " << result << endl;

    return 0;
}
