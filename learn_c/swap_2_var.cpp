#include <iostream>

int main() {
    std::string x, y, z;
    std::cout << "input the first word: ";
    std::cin >> x;
    std::cout << "input the second word: ";
    std::cin >> y;
    z = x;
    x = y;
    y = z;
    std::cout << x << " " << y << std::endl;
}
