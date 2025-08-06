#include <stdio.h>

void clear_stdin() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF);
}

int get_num() {
    int temp;
    while (1) {
        int is_valid = scanf("%d", &temp);
        clear_stdin();
        if ((temp > 20) || (temp < 1)) {
            printf("the number must range from 1-20\n");
        } else {
            if (is_valid == 1) return temp;
        }
        printf("reinput the number: ");
    }
}

unsigned long long get_factorial(int num) {
    unsigned long long result = 1;
    for (int x = num; x > 1; x--) {
        result *= x;
    }
    return result;
}

int main() {
    int input_num;
    unsigned long long factorial;
    printf("input a positive rounded number: ");
    input_num = get_num();
    factorial = get_factorial(input_num);
    printf("%d! = %llu\n", input_num, factorial);
    return 0;
}
