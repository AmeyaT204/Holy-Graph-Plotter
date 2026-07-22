#include "DSL.hpp"
#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // 1. Runtime-parsed expression, loaded from a text file
    std::ifstream file("equation.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open equation.txt" << std::endl;
        return 1;
    }
    std::string line;
    std::getline(file, line);
    file.close();

    ExprPtr parsedSurface;
    try {
        parsedSurface = parseEquation(line);
    } catch (const std::exception& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Parsed from file: " << parsedSurface->toString() << std::endl;

    // 2. The same shape built directly in C++ with the DSL (week 7 style)
    ExprPtr hardcodedSurface = sin(x) * cos(y);
    std::cout << "Hardcoded DSL:    " << hardcodedSurface->toString() << std::endl;

    std::cout << "\nSampling both over a small grid:\n";
    for (double xv = -1.0; xv <= 1.0; xv += 0.5) {
        for (double yv = -1.0; yv <= 1.0; yv += 0.5) {
            std::cout << "(" << xv << "," << yv << ") "
                      << "parsed=" << parsedSurface->evaluate(xv, yv)
                      << " hardcoded=" << hardcodedSurface->evaluate(xv, yv) << std::endl;
        }
    }

    // 3. Exercise pow / unary minus / precedence via the parser directly
    ExprPtr tricky = parseEquation("x^2 - y^2 + -3*sin(x)");
    std::cout << "\nTricky expr: " << tricky->toString() << std::endl;
    std::cout << "value at (2,1) = " << tricky->evaluate(2.0, 1.0) << std::endl;

    return 0;
}