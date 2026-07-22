#include "Variable.hpp"

Variable::Variable(VarName name) : name_(name) {}

double Variable::evaluate(double x, double y) const {
    return (name_ == VarName::X) ? x : y;
}

std::string Variable::toString() const {
    return (name_ == VarName::X) ? "x" : "y";
}