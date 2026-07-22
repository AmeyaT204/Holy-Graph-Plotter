#include "UnaryOp.hpp"

UnaryOp::UnaryOp(UnaryOpType op, ExprPtr operand)
    : op_(op), operand_(std::move(operand)) {}

double UnaryOp::evaluate(double x, double y) const {
    double v = operand_->evaluate(x, y);
    switch (op_) {
        case UnaryOpType::Neg: return -v;
    }
    return v;
}

std::string UnaryOp::toString() const {
    return "(-" + operand_->toString() + ")";
}
