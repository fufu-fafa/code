#include <stdio.h>
#include <stdlib.h>

void clearStdin() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF);
}

int isInvalid(double p1[2], double p2[2]) {
    if (fabs(p1[0] - p2[0] < EPS) && (fabs(p1[1] - p2[1] < EPS))) return 1;
    if (fabs(p1[0] - p2[0] < EPS)) return 2;
    return 0;
}

double det3(double m[3][3]) {
    return (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
            m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
            m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

void getCoord(double c[2]) {
    while (1) {
        int isValid = scanf("%lf %lf", c, c+1);
        clearStdin();
        if (isValid == 2) return;
        printf("invalid input, retry: ");
    }
}

void getEquation(double points[3][2], double result[3]) {
    double d, dA, dB, dC;

    double matrixMain[3][3] = {
        {points[0][0]*points[0][0], points[0][0], 1},
        {points[1][0]*points[1][0], points[1][0], 1},
        {points[2][0]*points[2][0], points[2][0], 1}
    };

    double matrixA[3][3] = {
        {points[0][1], points[0][0], 1},
        {points[1][1], points[1][0], 1},
        {points[2][1], points[2][0], 1}
    };

    double matrixB[3][3] = {
        {points[0][0]*points[0][0], points[0][1], 1},
        {points[1][0]*points[1][0], points[1][1], 1},
        {points[2][0]*points[2][0], points[2][1], 1}
    };

    double matrixC[3][3] = {
        {points[0][0]*points[0][0], points[0][0], points[0][1]},
        {points[1][0]*points[1][0], points[1][0], points[1][1]},
        {points[2][0]*points[2][0], points[2][0], points[2][1]}
    };

    d  = det3(matrixMain);
    dA = det3(matrixA);
    dB = det3(matrixB);
    dC = det3(matrixC);

    result[2] = dA / d;
    result[1] = dB / d;
    result[0] = dC / d;
}

int main() {
    int inval;
    double p[3][2];
    double res[3];
    for (int n = 0; n < 3; n++) {
        printf("enter the coordinate for point %d (format: x y): ", n+1);
        getCoord(p[n]);
    }
    for (int x = 0; x < 3; x++) {
        for (int y = x+1; y < 3; y++) {
            inval = isInvalid(p[x], p[y]);
            if (inval) {
                if (inval == 1) printf("any two points must be different\n");
                if (inval == 2) printf("any two points must have different x coordinate\n");
                exit(1);
            }
        }
    }
    getEquation(p, res);
    printf("result equation:\n%gx^2 + %gx + %g\n", res[2], res[1], res[0]);
    return 0;
}
