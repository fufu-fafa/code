#include <stdio.h>
#include <math.h>

void clearStdin() {
    int x;
    while ((x = getchar()) != EOF && x != '\n');
}

double getDouble() {
    double temp;
    while (1) {
        int isValid = scanf("%lf", &temp);
        clearStdin();
        if (isValid == 1) return temp;
        printf("invalid input, reenter the number: ");
    }
}

void getVec(double temp[2]) {
    while (1) {
        int isValid = scanf("%lf %lf", temp, temp+1);
        clearStdin();
        if (isValid == 2) return;
        printf("reinput the vector: ");
    }
}

int main() {
    int option;
    double v[2];
    double mass, time, height, dist;
    printf("1. known force vector\n2. known elevation angle and velocity\nselect one of the option above: ");
    option = (int) getDouble();

    if (option == 1) {
        printf("input the vector: ");
        getVec(v);
    }
    else {
        double angleDeg, angleRad, vel;
        printf("enter the elevation angle in degrees: ");
        angleDeg = getDouble();
        printf("enter the velocity: ");
        vel = getDouble();

        angleRad = angleDeg * M_PI/180;
        v[0] = cos(angleRad) * vel;
        v[1] = sin(angleRad) * vel;
    }

    time = v[1]/9.8;
    height = v[1]*time - (9.8 * time * time)/2;
    dist = v[0]*time;

    printf("time to reach max height, max height: %lf %lf\n", time, height);
    printf("time to landing, distance reached: %lf %lf\n", time*2.f, dist);
    return 0;
}
