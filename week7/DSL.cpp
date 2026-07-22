#include "DSL.hpp"
#include <cmath>

const ExprPtr x = std::make_shared<Variable>(VarName::X);
const ExprPtr y = std::make_shared<Variable>(VarName::Y);

ExprPtr constant(double v) {
    return std::make_shared<Constant>(v);
}

ExprPtr operator+(const ExprPtr& lhs, const ExprPtr& rhs) {
    return std::make_shared<BinaryOp>(OpType::Add, lhs, rhs);
}
ExprPtr operator-(const ExprPtr& lhs, const ExprPtr& rhs) {
    return std::make_shared<BinaryOp>(OpType::Sub, lhs, rhs);
}
ExprPtr operator*(const ExprPtr& lhs, const ExprPtr& rhs) {
    return std::make_shared<BinaryOp>(OpType::Mul, lhs, rhs);
}
ExprPtr operator/(const ExprPtr& lhs, const ExprPtr& rhs) {
    return std::make_shared<BinaryOp>(OpType::Div, lhs, rhs);
}

ExprPtr operator+(double lhs, const ExprPtr& rhs) { return constant(lhs) + rhs; }
ExprPtr operator+(const ExprPtr& lhs, double rhs) { return lhs + constant(rhs); }
ExprPtr operator-(double lhs, const ExprPtr& rhs) { return constant(lhs) - rhs; }
ExprPtr operator-(const ExprPtr& lhs, double rhs) { return lhs - constant(rhs); }
ExprPtr operator*(double lhs, const ExprPtr& rhs) { return constant(lhs) * rhs; }
ExprPtr operator*(const ExprPtr& lhs, double rhs) { return lhs * constant(rhs); }
ExprPtr operator/(double lhs, const ExprPtr& rhs) { return constant(lhs) / rhs; }
ExprPtr operator/(const ExprPtr& lhs, double rhs) { return lhs / constant(rhs); }

ExprPtr sin(const ExprPtr& arg) {
    return std::make_shared<FuncCall>("sin", [](double v){ return std::sin(v); }, arg);
}
ExprPtr cos(const ExprPtr& arg) {
    return std::make_shared<FuncCall>("cos", [](double v){ return std::cos(v); }, arg);
}
ExprPtr exp(const ExprPtr& arg) {
    return std::make_shared<FuncCall>("exp", [](double v){ return std::exp(v); }, arg);
}
ExprPtr sqrt(const ExprPtr& arg) {
    return std::make_shared<FuncCall>("sqrt", [](double v){ return std::sqrt(v); }, arg);
}