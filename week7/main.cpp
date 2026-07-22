#include "DSL.hpp"
#include <iostream>

int main() {
    // This is the payoff of the whole week: building z = sin(x)*cos(y)
    // by writing ordinary-looking C++ expressions, not calling
    // "makeBinaryOp(Mul, makeFuncCall(...), ...)" by hand.
    ExprPtr surface = sin(x) * cos(y);

    std::cout << "Expression: " << surface->toString() << std::endl;

    for (double xv = -1.0; xv <= 1.0; xv += 0.5) {
        for (double yv = -1.0; yv <= 1.0; yv += 0.5) {
            std::cout << "z(" << xv << ", " << yv << ") = "
                      << surface->evaluate(xv, yv) << std::endl;
        }
    }

    // A slightly nastier expression to prove double-mixing works
    ExprPtr complex = 2.0 * sin(x) + cos(y) * y - 1.0;
    std::cout << "\nComplex expr: " << complex->toString() << std::endl;
    std::cout << "value at (1,1) = " << complex->evaluate(1.0, 1.0) << std::endl;

    return 0;
}