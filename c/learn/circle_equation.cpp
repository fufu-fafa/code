#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>

struct coordinate {
    double x, y;
};

struct circle {
    double r;
    coordinate center;
};

struct line {
    double gradient;
    coordinate midpoint;
};

std::string format_double(double value, bool reversed = false) {
    std::ostringstream temp;

    if (reversed) {
        if (value < 0) temp << "+ " << -value;
        else temp << "- " << value;
    } else {
        if (value < 0) temp << "- " << -value;
        else temp << "+ " << value;
    }
    return temp.str();
}

std::string get_equation(circle result) {
    // format into
    // (x - a)^2 + (y - b)^2 = (radius)^2
    std::ostringstream temp;
    double r_squared = result.r * result.r;

    temp << "(x " << format_double(result.center.x, true) << ")^2 + " 
         << "(y " << format_double(result.center.y, true) << ")^2 = " << r_squared;
    return temp.str();
}

std::string line_circle_intersection(circle circle1, line line1) {
    // y = k +- sqrt(r^2 - (x - h)^2)
    coordinate point1, point2;
    std::ostringstream temp;
    double offset, discriminant, a, b, c;

    offset = sqrt(std::pow(circle1.r, 2) - std::pow(line1.midpoint.x - circle1.center.x, 2));
    if (std::isinf(line1.gradient)) {
        point1 = {line1.midpoint.x, circle1.center.y + offset};
        point2 = {line1.midpoint.x, circle1.center.y - offset};
    } else {
        // ax^2 + bx + c = 0
        a = 1 + std::pow(line1.gradient, 2);
        b = 2 * (line1.gradient * (line1.midpoint.y - circle1.center.y - line1.gradient * line1.midpoint.x) - circle1.center.x);
        c = std::pow(circle1.center.x, 2) + std::pow(line1.midpoint.y - circle1.center.y - line1.gradient * line1.midpoint.x, 2) - std::pow(circle1.r, 2);

        // (-b +- sqrt(b^2 - 4ac)) / 2a
        discriminant = std::pow(b, 2) - 4 * a * c;
        if (discriminant < 0) return "line does not intercept with circle";
        else if (discriminant == 0) return "line is tangent with the cirlce";

        point1.x = (-b + sqrt(discriminant)) / (2 * a);
        point2.x = (-b - sqrt(discriminant)) / (2 * a);

        point1.y = line1.gradient * (point1.x - line1.midpoint.x) + line1.midpoint.y;
        point2.y = line1.gradient * (point2.x - line1.midpoint.x) + line1.midpoint.y;
    }
    temp << "line intercepts the circle at:" << '\n'
         << "(" << point1.x << ", " << point1.y << ") and (" << point2.x << ", " << point2.y << ")";
    return temp.str();
}

double get_gradient(coordinate start, coordinate end) {
    // m = (y2 - y1) / (x2 - x1)
    if (end.x == start.x) return std::numeric_limits<double>::infinity();
    else return (end.y - start.y) / (end.x - start.x);
}

double get_num() {
    double temp;

    while (true) {
        std::cin >> temp;
        if (std::cin.fail()) {
            std::cout << "invalid input, reinput the number: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return temp;
        }
    }
}

coordinate get_coordinate() {
    coordinate temp;

    while (true) {
        std::cin >> temp.x >> temp.y;
        if (std::cin.fail()) {
            std::cout << "invalid input, reinput the coordinate (format: x y):" << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return temp;
        }
    }
}

coordinate get_intersection(line line1, line line2) {
    // y = mx + c
    // c = offset
    coordinate result;
    double offset1, offset2;

    if (line1.gradient == line2.gradient) {
        result.x = result.y = std::numeric_limits<double>::quiet_NaN();
    } else if (std::isinf(line1.gradient)) {
        offset2 = line2.midpoint.y - line2.gradient * line2.midpoint.x;
        result.x = line1.midpoint.x;
        result.y = line2.gradient * result.x + offset2;
    } else if (std::isinf(line2.gradient)) {
        offset1 = line1.midpoint.y - line1.gradient * line1.midpoint.x;
        result.x = line2.midpoint.x;
        result.y = line1.gradient * result.x + offset1;
    } else {
        offset1 = line1.midpoint.y - line1.gradient * line1.midpoint.x;
        offset2 = line2.midpoint.y - line2.gradient * line2.midpoint.x; 
        result.x = (offset2 - offset1) / (line1.gradient - line2.gradient);
        result.y = line1.gradient * result.x + offset1;
    }
    return result;
}

line get_line() {
    line temp;
    coordinate start, end;

    std::cout << "enter the first coordinate on the line (format: x y):" << '\n';
    start = get_coordinate();
    std::cout << "enter the second coordinate on the line (format: x y):" << '\n';
    end = get_coordinate();

    temp.midpoint.x = (start.x + end.x) / 2;
    temp.midpoint.y = (start.y + end.y) / 2;
    temp.gradient = get_gradient(start, end);
    return temp;
}

line get_perpendicular(line input) {
    // m2 = -1 / m1
    line temp;
    temp.midpoint = input.midpoint;

    if (std::isinf(input.gradient)) temp.gradient = 0;
    else if (input.gradient == 0) temp.gradient = std::numeric_limits<double>::infinity();
    else {
        temp.gradient = -1 / input.gradient;
    }
    return temp;
}

circle choice1() {
    circle temp;

    std::cout << "enter the center point coordinate (format: x y):" << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the radius of the circle: ";
    temp.r = get_num();
    if (temp.r <= 0) {
        std::cout << "radius cannot be lower or equal to 0" << '\n';
        exit(1);
    }
    return temp;
}

circle choice2() {
    circle temp;
    double determinant;
    coordinate a, b, c, result;
    line ab, bc, perp_ab, perp_bc;

    std::cout << "enter the first point (format: x y):" << '\n';
    a = get_coordinate();
    std::cout << "enter the second point (format: x y):" << '\n';
    b = get_coordinate();
    std::cout << "enter the third point (format: x y):" << '\n';
    c = get_coordinate();
    determinant = (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));

    if ((a.x == b.x && a.y == b.y) || (b.x == c.x && b.y == c.y) || (c.x == a.x && c.y == a.y)) {
        std::cout << "points must be distinct" << '\n';
        exit(1);
    } else if (determinant == 0) {
        std::cout << "points are collinear, unable to form circle" << '\n';
        exit(1);
    }

    ab.midpoint = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    ab.gradient = get_gradient(a, b);
    perp_ab = get_perpendicular(ab);

    bc.midpoint = {(b.x + c.x) / 2, (b.y + c.y) / 2};
    bc.gradient = get_gradient(b, c);
    perp_bc = get_perpendicular(bc);

    temp.center = get_intersection(perp_ab, perp_bc);
    temp.r = sqrt(std::pow(a.x - temp.center.x, 2) + std::pow(a.y - temp.center.y, 2));
    return temp;
}

circle choice3() {
    circle temp;
    coordinate edge;

    std::cout << "enter the center point coordinate (format: x y):" << '\n';
    temp.center = get_coordinate();
    std::cout << "enter the coordinate of a point on the circle:" << '\n';
    edge = get_coordinate();

    if (temp.center.x == edge.x && temp.center.y == edge.y) {
        std::cout << "both coordinate must be different" << '\n';
        exit(1);
    }

    temp.r = sqrt(std::pow(edge.x - temp.center.x, 2) + std::pow(edge.y - temp.center.y, 2));
    return temp;
}

circle choice4() {
    circle temp;
    coordinate endpoint1, endpoint2;

    std::cout << "enter the first diameter endpoint (format: x y):" << '\n';
    endpoint1 = get_coordinate();
    std::cout << "enter the second diameter endpoint (format: x y):" << '\n';
    endpoint2 = get_coordinate();
    if (endpoint1.x == endpoint2.x && endpoint1.y == endpoint2.y) {
        std::cout << "both coordinate must be different" << '\n';
        exit(1);
    }
    temp.center.x = (endpoint1.x + endpoint2.x) / 2;
    temp.center.y = (endpoint1.y + endpoint2.y) / 2;
    temp.r = sqrt(std::pow(endpoint2.x - endpoint1.x, 2) + std::pow(endpoint2.y - endpoint1.y, 2)) / 2;
    return temp;
}

std::string option1(circle prev) {
    coordinate coordinate1;
    double distance;
    double r_squared = prev.r * prev.r;

    std::cout << "enter the coordinate to check (format: x y): ";
    coordinate1 = get_coordinate();
    distance = std::pow(coordinate1.x - prev.center.x, 2) + std::pow(coordinate1.y - prev.center.y, 2);

    // allow less precise user input with "< 1e-2"
    if (distance > r_squared) return "the coordinate is outside of the circle";
    else if (std::abs(distance - r_squared) < 1e-2) return "the coordinate is on the edge of the circle";
    else return "the coordinate is inside of the circle";
}

std::string option2(circle prev) {
    line line1;
    double distance, offset;
    std::string str_output;

    line1 = get_line();
    if (std::isinf(line1.gradient)) {
        distance = std::abs(line1.midpoint.x - prev.center.x);
    } else {
        offset = line1.midpoint.y - line1.gradient * line1.midpoint.x;
        distance = std::abs(line1.gradient * prev.center.x + -1 * prev.center.y + offset) / sqrt(std::pow(line1.gradient, 2) + 1);
    }

    if (distance > prev.r) str_output = "the line is outside of the circle";
    else if (distance == prev.r) str_output = "the line is tangent to the circle";
    else str_output = line_circle_intersection(prev, line1);
    return str_output;
}

std::string option3(circle prev) {
    line line1, radius_line;
    coordinate point1;
    double dist_squared, offset;
    std::ostringstream str_output;

    while (true) {
        std::cout << "enter a point on the edge of the circle (format: x y): ";
        point1 = get_coordinate();
        dist_squared = std::pow(prev.center.x - point1.x, 2) + std::pow(prev.center.y - point1.y, 2);      

        // allow less precise user input
        if (!(std::abs(dist_squared - prev.r * prev.r) < 1e-2)) {
            std::cout << "the point is not on the edge of the circle" << '\n';
            continue;
        }

        radius_line.midpoint = point1;
        radius_line.gradient = get_gradient(prev.center, point1);
        line1 = get_perpendicular(radius_line);

        if (std::isinf(line1.gradient)) {
            str_output << "x = " << line1.midpoint.x;
        } else if (line1.gradient == 0) {
            str_output << "y = " << line1.midpoint.y;
        } else {
            offset = line1.midpoint.y - line1.gradient * line1.midpoint.x;
            str_output << "y = " << line1.gradient << "x " << format_double(offset);
        }
        return str_output.str();
    }
}

std::string option4(circle prev) {
    line line1;
    double radius_offset, positive_offset, negative_offset;
    std::ostringstream str_output;
    std::string vertical;

    std::cout << "are the lines vertical or infinite gradient: (y/n) ";
    std::cin >> vertical;

    if (vertical == "y" || vertical == "Y") {
        double temp1 = prev.center.x + prev.r;
        double temp2 = prev.center.x - prev.r;
        str_output << "x = " << temp1 << '\n'
                << "x = " << temp2 << '\n';
        return str_output.str();
    } else {
        std::cout << "enter the gradient for the tangent lines: ";
        line1.gradient = get_num();
    }

    // -ma + b +- r * sqrt(m * m + 1)
    radius_offset = prev.r * sqrt(line1.gradient * line1.gradient + 1);
    positive_offset = -1 * (line1.gradient * prev.center.x) + prev.center.y + radius_offset; 
    negative_offset = -1 * (line1.gradient * prev.center.x) + prev.center.y - radius_offset;

    str_output << "y = " << line1.gradient << "x " << format_double(positive_offset) << '\n'
            << "y = " << line1.gradient << "x " << format_double(negative_offset) << '\n';
    return str_output.str();
}

int main() {
    std::string equation, result2;
    circle result;
    int choice, option;
    std::cout << "1. center point and radius" << '\n'
            << "2. three points on the circle" << '\n'
            << "3. center and one point on the circle" << '\n'
            << "4. diameter endpoints" << '\n'
            << "5. exit" << '\n'
            << "enter one of the options above: ";

    choice = static_cast<int>(std::floor(get_num()));

    if (choice == 1) result = choice1();
    else if (choice == 2) result = choice2();
    else if (choice == 3) result = choice3();
    else if (choice == 4) result = choice4();
    else if (choice == 5) return 0;
    else {
        std::cout << "not a valid option" << '\n'
                << "exiting.." << '\n';
        return 1;
    }

    equation = get_equation(result);
    std::cout << "circle equation: " << equation << '\n' << '\n'
            << "1. check where a coordinate is on the circle" << '\n'
            << "2. check where a line is on the circle" << '\n'
            << "3. check tangent line equation with a point on the circle" << '\n'
            << "4. check tangent lines equation with gradient" << '\n'
            << "5. exit" << '\n'
            << "enter one of the options above: ";

    while (true) {
        option = static_cast<int>(std::floor(get_num()));

        if (option == 1) result2 = option1(result);
        else if (option == 2) result2 = option2(result);
        else if (option == 3) result2 = option3(result);
        else if (option == 4) result2 = option4(result);
        else if (option == 5) return 0;
        else {
            std::cout << "not a valid option, retry: ";
            continue;
        }
        std::cout << result2 << '\n' << '\n';
        break;
    }
    std::cout << "might not be fully accurate due to floating point errors" << '\n';
    return 0;
}
