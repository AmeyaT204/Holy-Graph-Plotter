#include "BinaryOp.hpp"
#include <cmath>
#include <stdexcept>

BinaryOp::BinaryOp(OpType op, ExprPtr lhs, ExprPtr rhs)
    : op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

double BinaryOp::evaluate(double x, double y) const {
    double l = lhs_->evaluate(x, y);
    double r = rhs_->evaluate(x, y);
    switch (op_) {
        case OpType::Add: return l + r;
        case OpType::Sub: return l - r;
        case OpType::Mul: return l * r;
        case OpType::Div: return l / r;
        case OpType::Pow: return std::pow(l, r);
    }
    throw std::runtime_error("Unknown operator");
}

std::string BinaryOp::toString() const {
    char sym = '?';
    switch (op_) {
        case OpType::Add: sym = '+'; break;
        case OpType::Sub: sym = '-'; break;
        case OpType::Mul: sym = '*'; break;
        case OpType::Div: sym = '/'; break;
        case OpType::Pow: sym = '^'; break;
    }
    return "(" + lhs_->toString() + " " + sym + " " + rhs_->toString() + ")";
}