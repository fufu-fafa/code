#include <stdio.h>
#include <math.h>

void clearStdin() {
    int x;
    while ((x = getchar()) != '\n' && x != EOF);
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
    double vec1[2];
    double vec2[2];
    double len1, len2, dot, theta;
    printf("the vector to be projected: ");
    getVec(vec1);
    printf("projected to: ");
    getVec(vec2);

    len1 = sqrt(vec1[0]*vec1[0] + vec1[1]*vec1[1]);
    len2 = sqrt(vec2[0]*vec2[0] + vec2[1]*vec2[1]);
    dot = vec1[0]*vec2[0] + vec1[1]*vec2[1];
    theta = acos(dot/(len1*len2));
    printf("projected vector: %lf %lf\n", dot*vec2[0]/len2, dot*vec2[1]/len2);
    printf("angle in rad: %lf\n angle in deg: %lf\n", theta, theta*180/M_PI);
    return 0;
}
