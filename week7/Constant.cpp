#include "Constant.hpp"
#include <sstream>

Constant::Constant(double value) : value_(value) {}

double Constant::evaluate(double, double) const {
    return value_;
}

std::string Constant::toString() const {
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}