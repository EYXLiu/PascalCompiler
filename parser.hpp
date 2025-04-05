#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "expr.cpp"
#include "stmt.cpp"
#include "function.cpp"

#include <string>
#include <vector>

class Parser {
private:
    std::unique_ptr<Lexer> lexer;
    std::unique_ptr<Token> curr;

    void next();
    bool match(TokenType t);
public:
    explicit Parser(std::unique_ptr<Lexer> lexer);
    std::unique_ptr<Ast> parseProgram();
    std::unique_ptr<Ast> parse();
};

#endif