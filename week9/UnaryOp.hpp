#ifndef _UNARYOP_HPP_
#define _UNARYOP_HPP_

#include "Expr.hpp"

enum class UnaryOpType { Neg };

class UnaryOp : public Expr {
public:
    UnaryOp(UnaryOpType op, ExprPtr operand);
    double evaluate(double x, double y, double t) const override;
    std::string toString() const override;

private:
    UnaryOpType op_;
    ExprPtr operand_;
};

#endif
