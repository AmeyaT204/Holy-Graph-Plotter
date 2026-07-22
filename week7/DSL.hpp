#ifndef _DSL_HPP_
#define _DSL_HPP_

#include "Expr.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "BinaryOp.hpp"
#include "FuncCall.hpp"

// Global handles so you can write expressions like: sin(x) * cos(y)
extern const ExprPtr x;
extern const ExprPtr y;

ExprPtr constant(double v);

// Core operator overloading: ExprPtr (+ - * /) ExprPtr
ExprPtr operator+(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator-(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator*(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator/(const ExprPtr& lhs, const ExprPtr& rhs);

// Overloaded again for raw doubles on either side — this is
// function/operator overloading doing real work: "2.0 * x" and
// "x * 2.0" both need to resolve without the caller wrapping
// constant() everywhere.
ExprPtr operator+(double lhs, const ExprPtr& rhs);
ExprPtr operator+(const ExprPtr& lhs, double rhs);
ExprPtr operator-(double lhs, const ExprPtr& rhs);
ExprPtr operator-(const ExprPtr& lhs, double rhs);
ExprPtr operator*(double lhs, const ExprPtr& rhs);
ExprPtr operator*(const ExprPtr& lhs, double rhs);
ExprPtr operator/(double lhs, const ExprPtr& rhs);
ExprPtr operator/(const ExprPtr& lhs, double rhs);

// Function overloading: same names as <cmath>, different signature
ExprPtr sin(const ExprPtr& arg);
ExprPtr cos(const ExprPtr& arg);
ExprPtr exp(const ExprPtr& arg);
ExprPtr sqrt(const ExprPtr& arg);

#endif