#include <iostream>

int main() {
    double pre_tax = 95000;
    double state_tax = 4.0/100 * pre_tax;
    double county_tax = 2.0/100 * pre_tax;
    std::cout << "pre tax: $" << pre_tax << '\n'
              << "state tax: $" << state_tax << '\n'
              << "county tax: $" << county_tax << '\n'
              << "post tax: $" << pre_tax - (state_tax + county_tax) << '\n';
    return 0;
}
