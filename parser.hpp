#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include "function.hpp"
#include "token.hpp"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

class Parser {
private:
    std::unique_ptr<Lexer> lexer;
    std::unique_ptr<Token> curr;

    void next();
    bool match(TokenType t);
    void expect(TokenType t);
public:
    explicit Parser(std::unique_ptr<Lexer> lexer);
    std::unique_ptr<Ast> parse();
    std::unique_ptr<Ast> parseProgram();

    std::vector<std::unique_ptr<Decl>> parseConstDecl();
    std::vector<std::unique_ptr<Decl>> parseTypeDecl();
    std::vector<std::unique_ptr<Decl>> parseVarDecl();
    std::vector<std::unique_ptr<Decl>> parseFuncDecl();
    std::vector<std::unique_ptr<Decl>> parseProcDecl();

    std::vector<std::unique_ptr<Stmt>> parseExprStmt();

    std::unique_ptr<Stmt> parseCallStmt(const std::string &name);
    std::unique_ptr<Stmt> parseAssignStmt(const std::string &name);
    std::unique_ptr<Stmt> parseCompountStmt();
    std::unique_ptr<Stmt> parseIfStmt();
    std::unique_ptr<Stmt> parseWhileStmt(); 
    std::unique_ptr<Stmt> parseRepeatStmt();
    std::unique_ptr<Stmt> parseForStmt();
    std::unique_ptr<Stmt> parseCaseStmt();
    std::unique_ptr<Stmt> parseReadStmt();
    std::unique_ptr<Stmt> parseWriteStmt();
};

#endif