#include "Tokenizer.hpp"
#include <cctype>
#include <stdexcept>

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        char c = input[i];

        if (std::isspace(static_cast<unsigned char>(c))) { i++; continue; }

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            size_t start = i;
            while (i < input.size() &&
                   (std::isdigit(static_cast<unsigned char>(input[i])) || input[i] == '.'))
                i++;
            double val = std::stod(input.substr(start, i - start));
            tokens.push_back({TokenType::Number, val, ""});
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            size_t start = i;
            while (i < input.size() && std::isalnum(static_cast<unsigned char>(input[i])))
                i++;
            tokens.push_back({TokenType::Ident, 0.0, input.substr(start, i - start)});
            continue;
        }

        switch (c) {
            case '+': tokens.push_back({TokenType::Plus, 0.0, ""}); i++; break;
            case '-': tokens.push_back({TokenType::Minus, 0.0, ""}); i++; break;
            case '*': tokens.push_back({TokenType::Star, 0.0, ""}); i++; break;
            case '/': tokens.push_back({TokenType::Slash, 0.0, ""}); i++; break;
            case '^': tokens.push_back({TokenType::Caret, 0.0, ""}); i++; break;
            case '(': tokens.push_back({TokenType::LParen, 0.0, ""}); i++; break;
            case ')': tokens.push_back({TokenType::RParen, 0.0, ""}); i++; break;
            default:
                throw std::runtime_error(std::string("Unexpected character: ") + c);
        }
    }

    tokens.push_back({TokenType::End, 0.0, ""});
    return tokens;
}
