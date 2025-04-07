#include "parser.hpp"

std::vector<std::unique_ptr<Stmt>> Parser::parseStmts() {
    std::vector<std::unique_ptr<Stmt>> stmts;

    while (!match(TokenType::tok_end)) {
        stmts.push_back(parseExprStmt());
    }
    
    expect(TokenType::tok_dot);
    return stmts;
}

std::unique_ptr<Stmt> Parser::parseExprStmt() {
    if (match(TokenType::tok_identifier)) {
        std::string n = curr->value;
        next();
        if (match(TokenType::tok_assign)) {
            return parseAssignStmt(n);
        } else {
            return parseCallStmt(n);
        }
    } else if (match(TokenType::tok_if)) {
        return parseIfStmt();
    } else if (match(TokenType::tok_while)) {
        return parseWhileStmt();
    } else if (match(TokenType::tok_repeat)) {
        return parseRepeatStmt();
    } else if (match(TokenType::tok_for)) {
        return parseForStmt();
    } else if (match(TokenType::tok_case)) {
        return parseCaseStmt();
    } else if (match(TokenType::tok_read)) {
        return parseReadStmt();
    } else if (match(TokenType::tok_write)) {
        return parseWriteStmt();
    } else {
        throw new std::runtime_error("Invalid token");
    }
}

std::unique_ptr<Stmt> Parser::parseAssignStmt(const std::string &name) {
    expect(TokenType::tok_assign);
    if (!match(TokenType::tok_identifier)) {
        std::string unary = curr->value;
        next();
        std::unique_ptr<Expr> v = parseNestedExpr();
        expect(TokenType::tok_semicolon);
        return std::make_unique<AssignStmt>(std::make_unique<VarExpr>(name), std::make_unique<UnaryExpr>(unary, v));
    } else {
        std::unique_ptr<Expr> v = parseNestedExpr();
        expect(TokenType::tok_semicolon);
        return std::make_unique<AssignStmt>(std::make_unique<VarExpr>(name), v);
    }
    
}

std::unique_ptr<Expr> Parser::parseNestedExpr() {
    if (match(TokenType::tok_number)) {
        std::unique_ptr<Expr> LHS = std::make_unique<NumberExpr>(std::stod(curr->value));
        if ((curr->type >= TokenType::tok_divide && curr->type <= TokenType::tok_plus) || (curr->type >= TokenType::tok_div && curr->type <= TokenType::tok_equals)) {
            std::string binary = curr->value;
            next();
            std::unique_ptr<Expr> RHS = parseNestedExpr();
            return std::make_unique<BinaryExpr>(curr->value, LHS, RHS);
        }
        return LHS;
    } else if (match(TokenType::tok_open_paren)) {
        next();
        std::unique_ptr<Expr> v = parseNestedExpr();
        expect(TokenType::tok_close_bracket);
        return v;
    } else if (match(TokenType::tok_char_literal)) {
        std::unique_ptr<CharExpr> c = std::make_unique<CharExpr>((curr->value)[0]);
        next();
        return c;
    } else if (match(TokenType::tok_string_literal)) {
        std::unique_ptr<StringExpr> s = std::make_unique<StringExpr>(curr->value);
        next();
        return s;
    } else if (match(TokenType::tok_boolean_literal)) {
        std::unique_ptr<BoolExpr> LHS = std::make_unique<BoolExpr>(curr->value == "true");
        next();
        if ((curr->type >= TokenType::tok_divide && curr->type <= TokenType::tok_plus) || (curr->type >= TokenType::tok_greater_equal && curr->type <= TokenType::tok_equals)) {
            std::string binary = curr->value;
            next();
            std::unique_ptr<Expr> RHS = parseNestedExpr();
            return std::make_unique<BinaryExpr>(curr->value, LHS, RHS);
        }
        return LHS;
    } else if (match(TokenType::tok_identifier)) {
        std::string name = curr->value;
        next();
        if (match(TokenType::tok_open_bracket)) {
            std::unique_ptr<Expr> LHS = parseCallExpr(name);
            if ((curr->type >= TokenType::tok_divide && curr->type <= TokenType::tok_plus) || (curr->type >= TokenType::tok_div && curr->type <= TokenType::tok_equals)) {
                std::string binary = curr->value;
                next();
                std::unique_ptr<Expr> RHS = parseNestedExpr();
                return std::make_unique<BinaryExpr>(curr->value, LHS, RHS);
            }
            return LHS;
        } else {
            std::unique_ptr<VarExpr> LHS = std::make_unique<VarExpr>(name);
            if ((curr->type >= TokenType::tok_divide && curr->type <= TokenType::tok_plus) || (curr->type >= TokenType::tok_greater_equal && curr->type <= TokenType::tok_equals)) {
                std::string binary = curr->value;
                next();
                std::unique_ptr<Expr> RHS = parseNestedExpr();
                return std::make_unique<BinaryExpr>(curr->value, LHS, RHS);
            }
            return LHS;
        }
    }
}



std::unique_ptr<Expr> Parser::parseCallExpr(const std::string &name) {
    expect(TokenType::tok_open_bracket);
    std::vector<std::unique_ptr<Expr>> args;
    while (!match(TokenType::tok_close_bracket)) {
        args.push_back(parseNestedExpr());
        if(match(TokenType::tok_comma)) { next(); };
    };
    expect(TokenType::tok_close_bracket);
    return std::make_unique<CallExpr>(name, args);
}

std::unique_ptr<Stmt> Parser::parseCallStmt(const std::string &name) {
    expect(TokenType::tok_open_bracket);
    std::vector<std::unique_ptr<Expr>> args;
    while (!match(TokenType::tok_close_bracket)) {
        args.push_back(parseNestedExpr());
        if(match(TokenType::tok_comma)) { next(); };
    };
    return std::make_unique<CallStmt>(name, args);
}

std::unique_ptr<Stmt> Parser::parseCompoundStmt() {
    expect(TokenType::tok_begin);
    std::vector<std::unique_ptr<Stmt>> stmts;

    while (!match(TokenType::tok_end)) {
        stmts.push_back(parseExprStmt());
    }

    expect(TokenType::tok_end);
    
    return std::make_unique<CompoundStmt>(stmts);
}

std::unique_ptr<Stmt> Parser::parseIfStmt() {
    expect(TokenType::tok_if);
    std::unique_ptr<Expr> cond = parseNestedExpr();
    expect(TokenType::tok_then);
    std::unique_ptr<Stmt> t;

    if (match(TokenType::tok_begin)) {
        t = parseCompoundStmt();
    } else {
        t = parseExprStmt();
    }

    if (match(TokenType::tok_else)) {
        next();
        std::unique_ptr<Stmt> e;
        if (match(TokenType::tok_begin)) {
            e = parseCompoundStmt();
        } else {
            e = parseExprStmt();
        }
        expect(TokenType::tok_semicolon);
        return std::make_unique<IfStmt>(cond, t, e);
    }
    expect(TokenType::tok_semicolon);
    return std::make_unique<IfStmt>(cond, t);
}

std::unique_ptr<Stmt> Parser::parseWhileStmt() {
    expect(TokenType::tok_while);
    std::unique_ptr<Expr> cond = parseNestedExpr();
    expect(TokenType::tok_do);
    std::unique_ptr<Stmt> d;
    if (match(TokenType::tok_begin)) {
        d = parseCompoundStmt();
    } else {
        d = parseExprStmt();
    }
    expect(TokenType::tok_semicolon);
    return std::make_unique<WhileStmt>(cond, d);
}

std::unique_ptr<Stmt> Parser::parseRepeatStmt() {
    expect(TokenType::tok_repeat);
    std::vector<std::unique_ptr<Stmt>> stmts;
    while(!match(TokenType::tok_until)) {
        stmts.push_back(parseExprStmt());
        expect(TokenType::tok_semicolon);
    }
    expect(TokenType::tok_until);
    std::unique_ptr<Expr> cond = parseNestedExpr();
    expect(TokenType::tok_semicolon);
    return std::make_unique<RepeatStmt>(cond, stmts);
}

std::unique_ptr<Stmt> Parser::parseForStmt() {
    expect(TokenType::tok_for);
    std::string name = curr->value;
    expect(TokenType::tok_identifier);
    expect(TokenType::tok_assign);
    bool ischar = false;
    int start;
    if (match(TokenType::tok_number)) {
        start = std::stoi(curr->value);
        next();
    } else {
        ischar = true;
        start = static_cast<int>((curr->value)[0]);
        expect(TokenType::tok_char_literal);
    }
    bool downto = false;
    if (match(TokenType::tok_downto)) {
        downto = true;
    } else {
        expect(TokenType::tok_to);
    }
    int end;
    if (ischar) {
        end = static_cast<int>((curr->value)[0]);
        expect(TokenType::tok_char_literal);
    } else {
        end = std::stoi(curr->value);
        expect(TokenType::tok_number);
    }
    expect(TokenType::tok_number);
    expect(TokenType::tok_do);
    std::unique_ptr<Stmt> b;
    if (match(TokenType::tok_begin)) {
        b = parseCompoundStmt();
    } else {
        b = parseExprStmt();
    }
    expect(TokenType::tok_semicolon);
    return std::make_unique<ForStmt>(name, start, end, ischar, downto, b);
}

std::unique_ptr<Stmt> Parser::parseCaseStmt() {
    expect(TokenType::tok_case);
    std::unique_ptr<VarExpr> value = std::make_unique<VarExpr>(curr->value);
    expect(TokenType::tok_identifier);
    expect(TokenType::tok_of);
    std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Stmt>>> cases;
    while(!match(TokenType::tok_else) && !(match(TokenType::tok_end))) {
        std::unique_ptr<Expr> p;
        if (match(TokenType::tok_number)) {
            p = std::make_unique<NumberExpr>(std::stod(curr->value));
        } else if (match(TokenType::tok_char_literal)) {
            p = std::make_unique<CharExpr>((curr->value)[0]);
        } else if (match(TokenType::tok_boolean_literal)) {
            p = std::make_unique<BoolExpr>(curr->value == "true");
        } else {
            throw new std::runtime_error("Invalid type");
        }
        expect(TokenType::tok_colon);
        std::unique_ptr<Stmt> s = parseExprStmt();
        cases.push_back(std::pair(std::move(p), std::move(s)));
        expect(TokenType::tok_semicolon);
    }
    std::unique_ptr<Stmt> s;
    if (match(TokenType::tok_else)) {
        next();
        s = parseExprStmt();
        expect(TokenType::tok_semicolon);
    }
    expect(TokenType::tok_end);
    expect(TokenType::tok_semicolon);
    return std::make_unique<CaseStmt>(value, cases, s);
}

std::unique_ptr<Stmt> Parser::parseReadStmt() {
    expect(TokenType::tok_read);
    expect(TokenType::tok_open_paren);
    std::vector<std::string> variables;

    while (!match(TokenType::tok_close_bracket)) {
        std::string var = curr->value;
        expect(TokenType::tok_identifier);
        variables.push_back(var);
        if (match(TokenType::tok_comma)) {
            next();
        }
    }

    expect(TokenType::tok_close_paren);
    expect(TokenType::tok_semicolon);
    
    return std::make_unique<ReadStmt>(variables);
}

std::unique_ptr<Stmt> Parser::parseWriteStmt() {
    expect(TokenType::tok_read);
    expect(TokenType::tok_open_paren);
    std::vector<std::unique_ptr<Expr>> exprs;

    while (!match(TokenType::tok_close_bracket)) {
        std::unique_ptr<Expr> e = parseNestedExpr();
        exprs.push_back(e);
        if (match(TokenType::tok_comma)) {
            next();
        }
    }

    expect(TokenType::tok_close_paren);
    expect(TokenType::tok_semicolon);

    return std::make_unique<WriteStmt>(exprs);
}
