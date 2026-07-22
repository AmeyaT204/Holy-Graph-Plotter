#include "FuncCall.hpp"

FuncCall::FuncCall(std::string name, std::function<double(double)> fn, ExprPtr arg)
    : name_(std::move(name)), fn_(std::move(fn)), arg_(std::move(arg)) {}

double FuncCall::evaluate(double x, double y, double t) const {
    return fn_(arg_->evaluate(x, y, t));
}

std::string FuncCall::toString() const {
    return name_ + "(" + arg_->toString() + ")";
}
