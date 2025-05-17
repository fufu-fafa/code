#include <iostream>
#include <algorithm>
#include <cctype>

std::string lower_pls(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char x) {return std::tolower(x);});
    return str;
}

int main() {
    std::string x = "MiXEd CasE";
    std::cout << "mixed: " << x << '\n';
    std::string result = lower_pls(x);
    std::cout << "lowered: " << result << '\n';
    return 0;
}
