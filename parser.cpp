#include "parser.hpp"

void Parser::next() {
    curr = std::move(lexer->nextToken());
}

bool Parser::match(TokenType t) {
    return curr->type == t;
}

explicit Parser::Parser(std::unique_ptr<Lexer> lexer) : lexer(std::move(lexer)) {
    next();
}


