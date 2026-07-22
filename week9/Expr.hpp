#ifndef _EXPR_HPP_
#define _EXPR_HPP_

#include <memory>
#include <string>

// Abstract base for every node in the expression tree.
//
// Week 9 change: evaluate() now takes a third argument, t, so surfaces
// can depend on time (e.g. sin(x + t)*cos(y)). Every subclass and every
// call site that used the old two-argument evaluate(x, y) needs updating.
class Expr {
public:
    virtual ~Expr() = default;
    virtual double evaluate(double x, double y, double t) const = 0;
    virtual std::string toString() const = 0;
};

// All tree nodes are held via shared_ptr — RAII means no manual
// delete anywhere, and shared ownership is exactly right here since
// the same subexpression can legally appear in multiple parents
// (e.g. reusing "x" across many terms).
using ExprPtr = std::shared_ptr<Expr>;

#endif
