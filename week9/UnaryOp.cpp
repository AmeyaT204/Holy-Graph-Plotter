#include "UnaryOp.hpp"

UnaryOp::UnaryOp(UnaryOpType op, ExprPtr operand)
    : op_(op), operand_(std::move(operand)) {}

double UnaryOp::evaluate(double x, double y, double t) const {
    double v = operand_->evaluate(x, y, t);
    switch (op_) {
        case UnaryOpType::Neg: return -v;
    }
    return v; // unreachable
}

std::string UnaryOp::toString() const {
    return "(-" + operand_->toString() + ")";
}
