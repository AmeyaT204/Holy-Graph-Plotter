#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_

#include <string>
#include <vector>

enum class TokenType { Number, Ident, Plus, Minus, Star, Slash, Caret, LParen, RParen, End };

struct Token {
    TokenType type;
    double numValue;
    std::string text;
};

std::vector<Token> tokenize(const std::string& input);

#endif
