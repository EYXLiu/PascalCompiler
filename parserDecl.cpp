#include "parser.hpp"

void Parser::next() {
    curr = std::move(lexer->nextToken());
}

bool Parser::match(TokenType t) {
    return curr->type == t;
}

void Parser::expect(TokenType t) {
    if (!match(t)) {
        throw new std::runtime_error("Expected token: " + std::to_string(t));
    };
    next();
}

explicit Parser::Parser(std::unique_ptr<Lexer> lexer) : lexer(std::move(lexer)) {
    next();
}

std::unique_ptr<Ast> Parser::parse() {
    return parseProgram();
}

std::unique_ptr<Ast> Parser::parseProgram() {
    expect(TokenType::tok_program);
    std::string name = curr->value;
    next();
    expect(TokenType::tok_semicolon);

    std::vector<std::unique_ptr<Decl>> varDecls;
    while (match(TokenType::tok_const) || match(TokenType::tok_type) || match(TokenType::tok_var) || match(TokenType::tok_function) || match(TokenType::tok_procedure)) {
        if (match(TokenType::tok_const)) {
            next();
            std::vector<std::unique_ptr<Decl>> consts = parseConstDecl();
            for (auto& ptr : consts) {
                varDecls.push_back(std::move(ptr));
            }
            consts.clear();
        } else if (match(TokenType::tok_type)) {
            next();
            std::vector<std::unique_ptr<Decl>> types = parseTypeDecl();
            for (auto& ptr : types) {
                varDecls.push_back(std::move(ptr));
            }
            types.clear();
        } else if (match(TokenType::tok_var)) {
            next();
            std::vector<std::unique_ptr<Decl>> vars = parseTypeDecl();
            for (auto& ptr : vars) {
                varDecls.push_back(std::move(ptr));
            }
            vars.clear();
        } else if (match(TokenType::tok_function)) {
            next();
            std::vector<std::unique_ptr<Decl>> funcs = parseTypeDecl();
            for (auto& ptr : funcs) {
                varDecls.push_back(std::move(ptr));
            }
            funcs.clear();
        } else if (match(TokenType::tok_procedure)) {
            next();
            std::vector<std::unique_ptr<Decl>> procs = parseTypeDecl();
            for (auto& ptr : procs) {
                varDecls.push_back(std::move(ptr));
            }
            procs.clear();
        } 
    }

};

std::vector<std::unique_ptr<Decl>> Parser::parseConstDecl() {
    std::vector<std::unique_ptr<Decl>> decls;

    while (match(TokenType::tok_identifier)) {
        std::string name = curr->value;
        std::unique_ptr<VarExpr> n = std::make_unique<VarExpr>(name);
        next();
        expect(TokenType::tok_assign);
        std::unique_ptr<Expr> v;
        if (curr->type == TokenType::tok_number) {
            v = std::make_unique<NumberExpr>(std::stod(curr->value));
        } else if (curr->type == TokenType::tok_boolean_literal) {
            v = std::make_unique<BoolExpr>(curr->value == "True");
        } else {
            v = std::make_unique<StringExpr>(curr->value);
        }
        next();
        expect(TokenType::tok_semicolon);
        decls.push_back(std::make_unique<ConstDecl>(n, v));
    }

    return decls;
}

std::vector<std::unique_ptr<Decl>> Parser::parseTypeDecl() {
    std::vector<std::unique_ptr<Decl>> decls;

    while (match(TokenType::tok_identifier)) {
        std::string name = curr->value;
        std::unique_ptr<VarExpr> n = std::make_unique<VarExpr>(name);
        next();

        if (match(TokenType::tok_assign)) {
            next();
            if (match(TokenType::tok_record)) {
                next();
                std::vector<std::unique_ptr<Decl>> v = parseVarDecl();
                expect(TokenType::tok_end);
                expect(TokenType::tok_semicolon);
                decls.push_back(std::make_unique<RecordType>(n, v));
            } else if (match(TokenType::tok_array)) {
                next();
                expect(TokenType::tok_open_bracket);
                int min = std::stoi(curr->value);
                next();
                expect(TokenType::tok_range);
                int max = std::stoi(curr->value);
                next();
                expect(TokenType::tok_close_bracket);
                expect(TokenType::tok_of);
                if (match(TokenType::tok_identifier)) {
                    decls.push_back(std::make_unique<ArrayType>(curr->type, min, max, curr->value));
                } else {
                    decls.push_back(std::make_unique<ArrayType>(curr->type, min, max));
                }
                next();
                expect(TokenType::tok_semicolon);
            } else if (match(TokenType::tok_open_paren)) {
                next();
                std::vector<std::pair<std::unique_ptr<Decl>, int>> values;
                int counter = 0;
                while (!match(TokenType::tok_close_paren)) {
                    std::unique_ptr<VarDecl> value = std::make_unique<VarDecl>(curr->value);
                    next();
                    int num = counter++;
                    values.push_back(std::make_pair(std::move(value), num));
                    expect(TokenType::tok_comma);
                }
                expect(TokenType::tok_close_paren);
                decls.push_back(std::make_unique<EnumType>(n, values));
            } else if (match(TokenType::tok_number)) {
                std::unique_ptr<NumberExpr> min = std::make_unique<NumberExpr>(std::stod(curr->value));
                next();
                expect(TokenType::tok_range);
                std::unique_ptr<NumberExpr> max = std::make_unique<NumberExpr>(std::stod(curr->value));
                next();
                expect(TokenType::tok_semicolon);
                decls.push_back(std::make_unique<RangeType>(n, curr->type, min, max));
            } else if (match(TokenType::tok_char_literal)) {
                std::unique_ptr<CharExpr> min = std::make_unique<CharExpr>((curr->value)[0]);
                next();
                expect(TokenType::tok_range);
                std::unique_ptr<CharExpr> max = std::make_unique<CharExpr>((curr->value)[0]);
                next();
                expect(TokenType::tok_semicolon);
                decls.push_back(std::make_unique<RangeType>(n, curr->type, min, max));
            }
        } else {
            expect(TokenType::tok_colon);
            TokenType type = curr->type;
            next();
            expect(TokenType::tok_semicolon);
            decls.push_back(std::make_unique<TypeDecl>(n, type));
        }
    }
    return decls;
}

std::vector<std::unique_ptr<Decl>> Parser::parseVarDecl() {
    std::vector<std::unique_ptr<Decl>> decls;

    while (match(TokenType::tok_identifier)) {
        std::vector<std::unique_ptr<VarExpr>> n;
        while (match(TokenType::tok_identifier)) {
            n.push_back(std::make_unique<VarExpr>(curr->value));
            expect(TokenType::tok_comma);
        }
        expect(TokenType::tok_colon);
        if (match(TokenType::tok_array)) {
            next();
            expect(TokenType::tok_open_bracket);
            int min = std::stoi(curr->value);
            next();
            expect(TokenType::tok_range);
            int max = std::stoi(curr->value);
            next();
            expect(TokenType::tok_close_bracket);
            expect(TokenType::tok_of);
            if (match(TokenType::tok_identifier)) {
                decls.push_back(std::make_unique<ArrayVar>(n, curr->type, min, max, curr->value));
            } else {
                decls.push_back(std::make_unique<ArrayVar>(n, curr->type, min, max));
            }
        } else if (match(TokenType::tok_identifier)) {
            decls.push_back(std::make_unique<VarDecl>(n, curr->type, curr->value));
        } else {
            decls.push_back(std::make_unique<VarDecl>(n, curr->type));
        }
    }

    return decls;
}

std::vector<std::unique_ptr<Decl>> Parser::parseFuncDecl() {

}

std::vector<std::unique_ptr<Decl>> Parser::parseProcDecl() {
    
}
