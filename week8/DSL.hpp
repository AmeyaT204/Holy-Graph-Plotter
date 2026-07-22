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

ExprPtr constant(double v);

ExprPtr operator+(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator-(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator*(const ExprPtr& lhs, const ExprPtr& rhs);
ExprPtr operator/(const ExprPtr& lhs, const ExprPtr& rhs);

// New: unary minus. Same name as binary operator-, disambiguated by
// arity — this is operator overloading picking the right overload
// purely from how many arguments are passed.
ExprPtr operator-(const ExprPtr& operand);

ExprPtr operator+(double lhs, const ExprPtr& rhs);
ExprPtr operator+(const ExprPtr& lhs, double rhs);
ExprPtr operator-(double lhs, const ExprPtr& rhs);
ExprPtr operator-(const ExprPtr& lhs, double rhs);
ExprPtr operator*(double lhs, const ExprPtr& rhs);
ExprPtr operator*(const ExprPtr& lhs, double rhs);
ExprPtr operator/(double lhs, const ExprPtr& rhs);
ExprPtr operator/(const ExprPtr& lhs, double rhs);

// New: exponentiation as a plain function (not operator^ — in C++,
// ^ is bitwise XOR with the wrong precedence for math, so pow() is
// the correct move here even though the string parser below is free
// to use ^ as a math operator in its own grammar).
ExprPtr pow(const ExprPtr& base, const ExprPtr& exponent);

ExprPtr sin(const ExprPtr& arg);
ExprPtr cos(const ExprPtr& arg);
ExprPtr tan(const ExprPtr& arg);
ExprPtr exp(const ExprPtr& arg);
ExprPtr sqrt(const ExprPtr& arg);
ExprPtr log(const ExprPtr& arg);
ExprPtr abs(const ExprPtr& arg);

// New: dynamic dispatch by name — this is what lets the runtime
// parser build a FuncCall node from a string like "sin" without
// the parser knowing about every function at compile time.
ExprPtr makeFunction(const std::string& name, ExprPtr arg);

#endif