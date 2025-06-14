#include <stdio.h>

void clear_stdin() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF);
}

int is_valid_op(const char in_char, const char valid_char[], int arr_len) {
    for (int x = 0; x < arr_len; x++) {
        if (valid_char[x] == in_char) return 1;
    }
    return 0;
}

double get_num() {
    double temp;

    while (1) {
        int is_valid = scanf("%lf", &temp);
        clear_stdin();
        if (is_valid == 1) return temp;
        printf("reinput the number: ");
    }
}

char get_op() {
    const char valid_op[4] = {'+', '-', '*', '/'};
    char temp;
    while (1) {
        scanf("%c", &temp);
        clear_stdin();
        if (is_valid_op(temp, valid_op, 4)) return temp;
        printf("input a valid operator (+, -, *, /): ");
    }
}

double calculate(const double num1, const double num2, const char op) {
    if (op == '+') return num1 + num2;
    else if (op == '-') return num1 - num2;
    else if (op == '*') return num1 * num2;
    else {
        if (num2 == 0) return 0.00;
        return num1 / num2;
    }
}

int main() {
    double num1, num2, result;
    char op;
    printf("input a number: ");
    num1 = get_num();

    printf("input the operator: ");
    op = get_op();

    printf("input another number: ");
    num2 = get_num();

    result = calculate(num1, num2, op);
    printf("result: %lf%c", result, '\n');
    return 0;
}
