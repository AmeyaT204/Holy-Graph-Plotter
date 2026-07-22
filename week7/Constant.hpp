#ifndef _CONSTANT_HPP_
#define _CONSTANT_HPP_

#include "Expr.hpp"

class Constant : public Expr {
public:
    explicit Constant(double value);
    double evaluate(double x, double y) const override;
    std::string toString() const override;

private:
    double value_;
};

#endif