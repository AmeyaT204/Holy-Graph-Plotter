#include "DSL.hpp"
#include <cmath>
#include <functional>
#include <stdexcept>
#include <unordered_map>

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

ExprPtr operator-(const ExprPtr& operand) {
    return std::make_shared<UnaryOp>(UnaryOpType::Neg, operand);
}

ExprPtr operator+(double lhs, const ExprPtr& rhs) { return constant(lhs) + rhs; }
ExprPtr operator+(const ExprPtr& lhs, double rhs) { return lhs + constant(rhs); }
ExprPtr operator-(double lhs, const ExprPtr& rhs) { return constant(lhs) - rhs; }
ExprPtr operator-(const ExprPtr& lhs, double rhs) { return lhs - constant(rhs); }
ExprPtr operator*(double lhs, const ExprPtr& rhs) { return constant(lhs) * rhs; }
ExprPtr operator*(const ExprPtr& lhs, double rhs) { return lhs * constant(rhs); }
ExprPtr operator/(double lhs, const ExprPtr& rhs) { return constant(lhs) / rhs; }
ExprPtr operator/(const ExprPtr& lhs, double rhs) { return lhs / constant(rhs); }

ExprPtr pow(const ExprPtr& base, const ExprPtr& exponent) {
    return std::make_shared<BinaryOp>(OpType::Pow, base, exponent);
}

ExprPtr makeFunction(const std::string& name, ExprPtr arg) {
    static const std::unordered_map<std::string, std::function<double(double)>> funcs = {
        {"sin",  [](double v){ return std::sin(v); }},
        {"cos",  [](double v){ return std::cos(v); }},
        {"tan",  [](double v){ return std::tan(v); }},
        {"exp",  [](double v){ return std::exp(v); }},
        {"sqrt", [](double v){ return std::sqrt(v); }},
        {"log",  [](double v){ return std::log(v); }},
        {"abs",  [](double v){ return std::fabs(v); }},
    };
    auto it = funcs.find(name);
    if (it == funcs.end())
        throw std::runtime_error("Unknown function: " + name);
    return std::make_shared<FuncCall>(name, it->second, std::move(arg));
}

ExprPtr sin(const ExprPtr& arg)  { return makeFunction("sin", arg); }
ExprPtr cos(const ExprPtr& arg)  { return makeFunction("cos", arg); }
ExprPtr tan(const ExprPtr& arg)  { return makeFunction("tan", arg); }
ExprPtr exp(const ExprPtr& arg)  { return makeFunction("exp", arg); }
ExprPtr sqrt(const ExprPtr& arg) { return makeFunction("sqrt", arg); }
ExprPtr log(const ExprPtr& arg)  { return makeFunction("log", arg); }
ExprPtr abs(const ExprPtr& arg)  { return makeFunction("abs", arg); }