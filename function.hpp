#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <vector>
#include "stmt.hpp"
#include "expr.hpp"
#include "token.hpp"

#include "astVisitor.hpp"

struct Prototype : public Ast {
    std::unique_ptr<VarExpr> name;
    std::vector<std::string> args;

    Prototype(std::unique_ptr<VarExpr> name, std::vector<std::string> args) : name(std::move(name)), args(std::move(args)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct Decl : public Ast {
    virtual ~Decl() = default;
};

struct ConstDecl : public Decl {
    std::unique_ptr<VarExpr> name;
    std::unique_ptr<Expr> value;

    ConstDecl(std::unique_ptr<VarExpr> name, std::unique_ptr<Expr> value) : name(std::move(name)), value(std::move(value)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct TypeDecl : public Decl {
    std::unique_ptr<VarExpr> name;
    TokenType type;

    TypeDecl(std::unique_ptr<VarExpr> name, TokenType type) : name(std::move(name)), type(type) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct RangeType : public Decl {
    std::unique_ptr<VarExpr> name;
    TokenType type;
    std::unique_ptr<Expr> min; 
    std::unique_ptr<Expr> max;

    RangeType(std::unique_ptr<VarExpr> name, TokenType type, std::unique_ptr<Expr> min, std::unique_ptr<Expr> max) : name(std::move(name)), type(type), min(std::move(min)), max(std::move(max)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct RecordType : public Decl {
    std::unique_ptr<VarExpr> name;
    std::vector<std::pair<std::string, std::unique_ptr<Decl>>> values;

    RecordType(std::unique_ptr<VarExpr> name, std::vector<std::pair<std::string, std::unique_ptr<Decl>>> values) : name(std::move(name)), values(std::move(values)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct ArrayType : public Decl {
    TokenType type;
    int min;
    int max;
    std::string identifier;

    ArrayType(TokenType type, int min, int max, const std::string &identifier = "") : type(type), min(min), max(max), identifier(identifier) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct EnumType : public Decl {
    std::unique_ptr<VarExpr> name;
    std::vector<std::pair<std::unique_ptr<Decl>, int>> values;

    EnumType(std::unique_ptr<VarExpr> name, std::vector<std::pair<std::unique_ptr<Decl>, int>> values) : name(std::move(name)), values(std::move(values)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct VarDecl : public Decl {
    std::vector<std::unique_ptr<VarExpr>> names;
    TokenType type;
    std::string identifier;

    VarDecl(std::vector<std::unique_ptr<VarExpr>> names, TokenType type, const std::string &identifier = "") : names(std::move(names)), type(type), identifier(identifier) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct RecordVar : public Decl {
    std::vector<std::unique_ptr<VarExpr>> names;
    RecordType* record;
    std::vector<std::pair<std::string, std::unique_ptr<Decl>>> values;

    RecordVar(std::vector<std::unique_ptr<VarExpr>> names, std::unique_ptr<RecordType> record, std::vector<std::pair<std::string, std::unique_ptr<Decl>>> values = {}) : names(std::move(names)), record(record.get()) {
        if (values.size() != 0) {
            this->values = values;
        } else {
            for (auto& a : values) {
                this->values.push_back(std::make_pair(a.first, nullptr));
            }
        }
    };
    RecordVar(std::unique_ptr<VarExpr> name, RecordType* record, std::vector<std::pair<std::string, std::unique_ptr<Decl>>> values = {}) : names({std::move(names)}), record(record) {
        if (values.size() != 0) {
            this->values = values;
        } else {
            for (auto& a : values) {
                this->values.push_back(std::make_pair(a.first, nullptr));
            }
        }
    };
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct ArrayVar : public Decl {
    std::vector<std::unique_ptr<VarExpr>> names;
    TokenType type;
    int min;
    int max;
    std::vector<std::unique_ptr<Decl>> values;
    std::string identifier;

    ArrayVar(std::vector<std::unique_ptr<VarExpr>> names, TokenType type, int min, int max, const std::string &identifier = "") : names(std::move(names)), type(type), min(min), max(max), identifier(identifier) {
        for (int i = min; i <= max; i++) {
            values.push_back(nullptr);
        }
    };
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct FuncDecl : public Decl {
    std::unique_ptr<Prototype> proto;
    TokenType type;
    std::vector<std::unique_ptr<Stmt>> body;

    FuncDecl(std::unique_ptr<Prototype> proto, TokenType type, std::vector<std::unique_ptr<Stmt>> body) : proto(std::move(proto)), type(type), body(std::move(body)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct ProcDecl : public Decl {
    std::unique_ptr<Prototype> proto;
    std::vector<std::unique_ptr<Stmt>> body;

    ProcDecl(std::unique_ptr<Prototype> proto, std::vector<std::unique_ptr<Stmt>> body) : proto(std::move(proto)), body(std::move(body)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

struct Program : public Ast {
    std::unique_ptr<VarExpr> name;
    std::vector<std::unique_ptr<Decl>> varDecls;

    Program(std::unique_ptr<VarExpr> name, std::vector<std::unique_ptr<Decl>> varDecls) : name(std::move(name)), varDecls(std::move(varDecls)) {};
    void accept(AstVisitor& visitor) override {
        visitor.visit(*this);
    };
};

#endif
