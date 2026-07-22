#ifndef _FUNCCALL_HPP_
#define _FUNCCALL_HPP_

#include "Expr.hpp"
#include <functional>
#include <string>

class FuncCall : public Expr {
public:
    FuncCall(std::string name, std::function<double(double)> fn, ExprPtr arg);
    double evaluate(double x, double y) const override;
    std::string toString() const override;

private:
    std::string name_;
    std::function<double(double)> fn_;
    ExprPtr arg_;
};

#endif