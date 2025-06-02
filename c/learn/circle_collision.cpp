#include <iostream>

int main() {
    // [0][0] = x position
    // [0][1] = y position

    // [1][0] = x velocity
    // [1][1] = y velocity

    double circle1[2][2] = {
        {1.0, 2.0}, {3.0, 4.0}
    };

    for (int x = 0; x <= 1; x++) {
        std::cout << circle1[x][0] << " and " << circle1[x][1] << '\n';
    }
    return 0;
}
