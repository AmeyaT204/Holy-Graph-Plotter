#ifndef _DSL_HPP_
#define _DSL_HPP_

#include "Expr.hpp"
#include "Constant.hpp"
#include "Variable.hpp"
#include "BinaryOp.hpp"
#include "UnaryOp.hpp"
#include "FuncCall.hpp"
#include <string>

extern const ExprPtr x;
extern const ExprPtr y;
// New in week 9: t, the time variable, usable in the DSL exactly like
// x and y (e.g. sin(x + t) * cos(y)).
extern const ExprPtr t;

ExprPtr constant(double v);

ExprPtr operator+(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator-(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator*(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator/(const ExprPtr& lhs, const ExprPtr& rhs);

// Unary minus. Same name as binary operator-, disambiguated by arity.
ExprPtr operator-(const ExprPtr& operand);

ExprPtr operator+(double lhs, const ExprPtr& rhs);
ExprPtr operator+(const ExprPtr& lhs, double rhs);
ExprPtr operator-(double lhs, const ExprPtr& rhs);
ExprPtr operator-(const ExprPtr& lhs, double rhs);
ExprPtr operator*(double lhs, const ExprPtr& rhs);
ExprPtr operator*(const ExprPtr& lhs, double rhs);
ExprPtr operator/(double lhs, const ExprPtr& rhs);
ExprPtr operator/(const ExprPtr& lhs, double rhs);

// Exponentiation as a plain function (^ is bitwise XOR in C++, wrong
// precedence for math).
ExprPtr pow(const ExprPtr& base, const ExprPtr& exponent);

ExprPtr sin(const ExprPtr& arg);
ExprPtr cos(const ExprPtr& arg);
ExprPtr tan(const ExprPtr& arg);
ExprPtr exp(const ExprPtr& arg);
ExprPtr sqrt(const ExprPtr& arg);
ExprPtr log(const ExprPtr& arg);
ExprPtr abs(const ExprPtr& arg);

// Dynamic dispatch by name — lets the runtime parser build a FuncCall
// node from a string like "sin" without knowing every function at
// compile time.
ExprPtr makeFunction(const std::string& name, ExprPtr arg);

#endif
