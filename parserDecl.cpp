#include "parser.hpp"

void Parser::next() {
    curr = lexer->nextToken();
}

bool Parser::match(TokenType t) {
    return curr->type == t;
}

void Parser::expect(TokenType t) {
    if (!match(t)) {
        std::cout << "\"" << curr->value << "\"" << std::endl;
        std::cout << std::to_string(curr->type) << std::endl;
        throw new std::runtime_error("Expected token: " + std::to_string(t));
    };
    next();
}

Parser::Parser(std::unique_ptr<Lexer> lexer) : lexer(std::move(lexer)) {
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
            std::vector<std::unique_ptr<Decl>> consts = parseConstDecl();
            for (auto& ptr : consts) {
                varDecls.push_back(std::move(ptr));
            }
            consts.clear();
        } else if (match(TokenType::tok_type)) {
            std::vector<std::unique_ptr<Decl>> types = parseTypeDecl();
            for (auto& ptr : types) {
                varDecls.push_back(std::move(ptr));
            }
            types.clear();
        } else if (match(TokenType::tok_var)) {
            std::vector<std::unique_ptr<Decl>> vars = parseVarDecl();
            for (auto& ptr : vars) {
                varDecls.push_back(std::move(ptr));
            }
            vars.clear();
        } else if (match(TokenType::tok_function)) {
            std::unique_ptr<Decl> func = parseFuncDecl();
            varDecls.push_back(std::move(func));
        } else if (match(TokenType::tok_procedure)) {
            std::unique_ptr<Decl> func = parseProcDecl();
            varDecls.push_back(std::move(func));
        }
    }

    std::vector<std::unique_ptr<Stmt>> stmts = parseStmts();

    return std::make_unique<Program>(name, std::move(varDecls), std::move(stmts));
};

std::vector<std::unique_ptr<Decl>> Parser::parseConstDecl() {
    expect(TokenType::tok_const);
    std::vector<std::unique_ptr<Decl>> decls;

    while (match(TokenType::tok_identifier)) {
        std::string n = curr->value;
        next();
        expect(TokenType::tok_assign);
        std::unique_ptr<Expr> v;
        if (curr->type == TokenType::tok_number) {
            v = std::make_unique<NumberExpr>(std::stod(curr->value));
        } else if (curr->type == TokenType::tok_boolean_literal) {
            v = std::make_unique<BoolExpr>(curr->value == "true");
        } else if (curr->type == TokenType::tok_char) {
            v = std::make_unique<CharExpr>((curr->value)[0]);
        } else {
            v = std::make_unique<StringExpr>(curr->value);
        }
        next();
        expect(TokenType::tok_semicolon);
        decls.push_back(std::make_unique<ConstDecl>(n, std::move(v)));
    }

    return decls;
}

std::vector<std::unique_ptr<Decl>> Parser::parseTypeDecl() {
    expect(TokenType::tok_type);
    std::vector<std::unique_ptr<Decl>> decls;

    while (match(TokenType::tok_identifier)) {
        std::string n = curr->value;
        next();

        if (match(TokenType::tok_assign)) {
            next();
            if (match(TokenType::tok_record)) {
                next();
                std::vector<std::unique_ptr<TypeDecl>> v;
                while(!match(TokenType::tok_end)) {
                    std::string n = curr->value;
                    expect(TokenType::tok_identifier);
                    expect(TokenType::tok_colon);
                    std::unique_ptr<TypeDecl> t = std::make_unique<TypeDecl>(n, curr->type);
                    next();
                    v.push_back(std::move(t));
                }
                expect(TokenType::tok_end);
                expect(TokenType::tok_semicolon);
                decls.push_back(std::make_unique<RecordType>(n, std::move(v)));
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
                    decls.push_back(std::make_unique<ArrayType>(n, curr->type, min, max, curr->value));
                } else {
                    decls.push_back(std::make_unique<ArrayType>(n, curr->type, min, max));
                }
                next();
                expect(TokenType::tok_semicolon);
            } else if (match(TokenType::tok_open_paren)) {
                next();
                std::vector<std::pair<std::unique_ptr<Expr>, int>> values;
                int counter = 0;
                while (!match(TokenType::tok_close_paren)) {
                    std::unique_ptr<VarExpr> value = std::make_unique<VarExpr>(curr->value, curr->type);
                    next();
                    int num = counter++;
                    values.push_back(std::make_pair(std::move(value), num));
                    expect(TokenType::tok_comma);
                }
                expect(TokenType::tok_close_paren);
                decls.push_back(std::make_unique<EnumType>(n, std::move(values)));
            } else if (match(TokenType::tok_number)) {
                std::unique_ptr<Expr> min = std::make_unique<NumberExpr>(std::stod(curr->value));
                next();
                expect(TokenType::tok_range);
                std::unique_ptr<Expr> max = std::make_unique<NumberExpr>(std::stod(curr->value));
                next();
                expect(TokenType::tok_semicolon);
                decls.push_back(std::make_unique<RangeType>(n, curr->type, std::move(min), std::move(max)));
            } else if (match(TokenType::tok_char_literal)) {
                std::unique_ptr<Expr> min = std::make_unique<CharExpr>((curr->value)[0]);
                next();
                expect(TokenType::tok_range);
                std::unique_ptr<Expr> max = std::make_unique<CharExpr>((curr->value)[0]);
                next();
                expect(TokenType::tok_semicolon);
                decls.push_back(std::make_unique<RangeType>(n, curr->type, std::move(min), std::move(max)));
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
    expect(TokenType::tok_var);
    std::vector<std::unique_ptr<Decl>> decls;

    while (match(TokenType::tok_identifier)) {
        std::vector<std::string> n;
        while (match(TokenType::tok_identifier)) {
            n.push_back(curr->value);
            if (match(TokenType::tok_comma)) {
                next();
            }
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
        expect(TokenType::tok_semicolon);
    }
    return decls;
}

std::unique_ptr<Decl> Parser::parseFuncDecl() {
    expect(TokenType::tok_function);
    std::vector<std::unique_ptr<Decl>> decls;

    std::string name = curr->value;
    expect(TokenType::tok_identifier);
    if (match(TokenType::tok_open_paren)) {
        expect(TokenType::tok_open_paren);
        while (!match(TokenType::tok_close_paren)) {
            std::vector<std::string> ids;
            while (!match(TokenType::tok_colon)) {
                ids.push_back(curr->value);
                expect(TokenType::tok_identifier);
                if (match(TokenType::tok_comma)) {
                    next();
                }
            }
            expect(TokenType::tok_colon);
            decls.push_back(std::make_unique<VarDecl>(ids, curr->type));
        }
        expect(TokenType::tok_close_paren);
    }
    expect(TokenType::tok_colon);
    TokenType t = curr->type;
    next();
    expect(TokenType::tok_semicolon);
    if (match(TokenType::tok_var)) {
        std::vector<std::unique_ptr<Decl>> vars = parseVarDecl();
        for (auto& a : vars) {
            decls.push_back(std::move(a));
        }
        vars.clear();
    }
    expect(TokenType::tok_begin);
    std::vector<std::unique_ptr<Stmt>> stmts;

    while (!match(TokenType::tok_end)) {
        stmts.push_back(parseExprStmt());
    }

    expect(TokenType::tok_end);
    expect(TokenType::tok_semicolon);
    return (std::make_unique<FuncDecl>(std::make_unique<Prototype>(name, std::move(decls)), t, std::move(stmts)));
}

std::unique_ptr<Decl> Parser::parseProcDecl() {
    expect(TokenType::tok_procedure);
    std::vector<std::unique_ptr<Decl>> decls;

    std::string name = curr->value;
    expect(TokenType::tok_identifier);
    expect(TokenType::tok_open_paren);
    while (!match(TokenType::tok_close_paren)) {
        std::vector<std::string> ids;
        while (!match(TokenType::tok_colon)) {
            ids.push_back(curr->value);
            expect(TokenType::tok_identifier);
            if (match(TokenType::tok_comma)) {
                next();
            }
        }
        expect(TokenType::tok_colon);
        decls.push_back(std::make_unique<VarDecl>(ids, curr->type));
    }
    expect(TokenType::tok_close_paren);
    expect(TokenType::tok_semicolon);
    if (match(TokenType::tok_var)) {
        std::vector<std::unique_ptr<Decl>> vars = parseVarDecl();
        for (auto& a : vars) {
            decls.push_back(std::move(a));
        }
        vars.clear();
    }
    expect(TokenType::tok_begin);
    std::vector<std::unique_ptr<Stmt>> stmts;

    while (!match(TokenType::tok_end)) {
        stmts.push_back(parseExprStmt());
    }

    expect(TokenType::tok_end);
    expect(TokenType::tok_semicolon);
    return (std::make_unique<ProcDecl>(std::make_unique<Prototype>(name, std::move(decls)), std::move(stmts)));
}
