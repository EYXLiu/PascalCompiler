#ifndef LEXER_HPP
#define LEXER_HPP

#include "token.hpp"
#include <iostream>
#include <vector>
#include <cctype>

class Lexer {
private:
    std::string input;
    size_t position = 0;

public:
    explicit Lexer(std::string input);

    char peek() const;
    char next();

    void skipWhitespace();
    std::unique_ptr<Token> number();
    std::unique_ptr<Token> identifier();
    std::unique_ptr<Token> special();
    std::unique_ptr<Token> stringLiteral();

    std::unique_ptr<Token> nextToken();
};

#endif
