#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include "Expr.hpp"
#include "Tokenizer.hpp"
#include <vector>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    ExprPtr parse();

private:
    std::vector<Token> tokens_;
    size_t pos_ = 0;

    const Token& peek() const;
    Token advance();
    void expect(TokenType type, const std::string& errMsg);

    ExprPtr parseExpr();
    ExprPtr parseTerm();
    ExprPtr parseUnary();
    ExprPtr parsePower();
    ExprPtr parsePrimary();
};

// Convenience: tokenize + parse a full equation string in one call.
ExprPtr parseEquation(const std::string& text);

#endif