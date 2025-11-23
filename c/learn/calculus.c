#include <stdio.h>

typedef struct {
    double coeff;
    double exp;
} term;

void clear_stdin() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF);
}

void swap2(double x1, double x2) {
    double temp;
    temp = x1;
    x1 = x2;
    x2 = temp;
}

void eq_parse(term eq[], int n) {
    for (int x = 0; x < n; x++) {
        if (eq[x].coeff == 0) continue;
        for (int y = x+1; y < n; y++) {
            if (eq[x].exp != eq[y].exp) continue;
            eq[x].coeff += eq[y].coeff;
            eq[y].coeff = 0;
            eq[y].exp = 0;
        }
    }

    for (int x = 0; x < n; x++) {
        if (eq[x].coeff != 0) continue;
        for (int y = x+1; y < n; y++) {
            if (eq[y].coeff == 0) continue;
            swap2(eq[x].coeff, eq[y].coeff);
            swap2(eq[x].exp, eq[y].exp);
            break;
        }
    }
}

void print_eq(term eq[], int n) {
    for (int x = 0; x < n; x++) {
        if (eq[x].coeff == 0) continue;

        if (eq[x].coeff > 0) printf("+%g", eq[x].coeff);
        else printf("%g", eq[x].coeff);

        if (eq[x].exp == 0) printf(" ");
        else if (eq[x].exp == 1) printf("x ");
        else printf("x^%g ", eq[x].exp);

    }
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

int main() {
    printf("enter the amount of term/number: ");
    int n = (int)get_num();
    term eq[n], eq_d[n], eq_ad[n];
    for (int x = 0; x < n; x++) {
        printf("enter the coefficient: ");
        eq[x].coeff = get_num();
        printf("enter the exponent: ");
        eq[x].exp = get_num();
    }

    eq_parse(eq, n);
    for (int x = 0; x < n; x++) {
        eq_ad[x].coeff = eq_d[x].coeff = eq[x].coeff;
        eq_ad[x].exp = eq_d[x].exp = eq[x].exp;
    }

    printf("normal:\n");
    print_eq(eq, n);
    printf("\n");

    for (int x = 0; x < n; x++) {
        if (eq_ad[x].coeff == 0) continue;
        eq_ad[x].exp += 1;
        eq_ad[x].coeff /= eq_ad[x].exp;
    }
    printf("antiderivative:\n");
    print_eq(eq_ad, n);
    printf("+C \n");

    for (int x = 0; x < n; x++) {
        if (eq_d[x].coeff == 0) continue;
        eq_d[x].coeff *= eq_d[x].exp;
        eq_d[x].exp -= 1.0;
    }
    printf("derivative:\n");
    print_eq(eq_d, n);
    printf("\n");

    return 0;
}
