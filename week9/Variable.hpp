#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include "Expr.hpp"

// Week 9: added T so expressions can reference time directly, e.g.
// sin(x + t)*cos(y).
enum class VarName { X, Y, T };

class Variable : public Expr {
public:
    explicit Variable(VarName name);
    double evaluate(double x, double y, double t) const override;
    std::string toString() const override;

private:
    VarName name_;
};

#endif
