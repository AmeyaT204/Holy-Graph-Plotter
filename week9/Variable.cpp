#include "Variable.hpp"

Variable::Variable(VarName name) : name_(name) {}

double Variable::evaluate(double x, double y, double t) const {
    switch (name_) {
        case VarName::X: return x;
        case VarName::Y: return y;
        case VarName::T: return t;
    }
    return 0.0; // unreachable
}

std::string Variable::toString() const {
    switch (name_) {
        case VarName::X: return "x";
        case VarName::Y: return "y";
        case VarName::T: return "t";
    }
    return "?"; // unreachable
}
