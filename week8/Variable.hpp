#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include "Expr.hpp"

enum class VarName { X, Y };

class Variable : public Expr {
public:
    explicit Variable(VarName name);
    double evaluate(double x, double y) const override;
    std::string toString() const override;

private:
    VarName name_;
};

#endif