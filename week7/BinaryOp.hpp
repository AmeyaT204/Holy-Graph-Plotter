#ifndef _BINARYOP_HPP_
#define _BINARYOP_HPP_

#include "Expr.hpp"

enum class OpType { Add, Sub, Mul, Div };

class BinaryOp : public Expr {
public:
    BinaryOp(OpType op, ExprPtr lhs, ExprPtr rhs);
    double evaluate(double x, double y) const override;
    std::string toString() const override;

private:
    OpType op_;
    ExprPtr lhs_, rhs_;
};

#endif