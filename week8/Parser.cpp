#include "Parser.hpp"
#include "DSL.hpp"
#include <stdexcept>

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

const Token& Parser::peek() const {
    return tokens_[pos_];
}

Token Parser::advance() {
    return tokens_[pos_++];
}

void Parser::expect(TokenType type, const std::string& errMsg) {
    if (peek().type != type)
        throw std::runtime_error(errMsg);
    advance();
}

ExprPtr Parser::parse() {
    ExprPtr result = parseExpr();
    expect(TokenType::End, "Unexpected trailing tokens after expression");
    return result;
}

ExprPtr Parser::parseExpr() {
    ExprPtr left = parseTerm();
    while (peek().type == TokenType::Plus || peek().type == TokenType::Minus) {
        TokenType op = advance().type;
        ExprPtr right = parseTerm();
        left = (op == TokenType::Plus) ? (left + right) : (left - right);
    }
    return left;
}

ExprPtr Parser::parseTerm() {
    ExprPtr left = parseUnary();
    while (peek().type == TokenType::Star || peek().type == TokenType::Slash) {
        TokenType op = advance().type;
        ExprPtr right = parseUnary();
        left = (op == TokenType::Star) ? (left * right) : (left / right);
    }
    return left;
}

ExprPtr Parser::parseUnary() {
    if (peek().type == TokenType::Minus) {
        advance();
        return -parseUnary(); // uses DSL's unary operator-
    }
    return parsePower();
}

ExprPtr Parser::parsePower() {
    ExprPtr base = parsePrimary();
    if (peek().type == TokenType::Caret) {
        advance();
        ExprPtr exponent = parseUnary(); // right-associative, allows x^-2
        return pow(base, exponent);
    }
    return base;
}

ExprPtr Parser::parsePrimary() {
    const Token& tok = peek();

    if (tok.type == TokenType::Number) {
        advance();
        return constant(tok.numValue);
    }

    if (tok.type == TokenType::Ident) {
        std::string name = tok.text;
        advance();

        if (peek().type == TokenType::LParen) {
            advance(); // consume '('
            ExprPtr arg = parseExpr();
            expect(TokenType::RParen, "Expected ')' after function argument");
            return makeFunction(name, arg);
        }

        if (name == "x") return x;
        if (name == "y") return y;
        if (name == "pi") return constant(3.14159265358979323846);

        throw std::runtime_error("Unknown identifier: " + name);
    }

    if (tok.type == TokenType::LParen) {
        advance();
        ExprPtr e = parseExpr();
        expect(TokenType::RParen, "Expected ')'");
        return e;
    }

    throw std::runtime_error("Unexpected token in expression");
}

ExprPtr parseEquation(const std::string& text) {
    Parser parser(tokenize(text));
    return parser.parse();
}