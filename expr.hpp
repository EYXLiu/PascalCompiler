#ifndef EXPR_HPP
#define EXPR_HPP

#include <string>
#include <vector>
#include "token.hpp"
#include "llvm.hpp"

#include "astVisitor.hpp"

struct Ast {
    virtual ~Ast() = default;
};

struct Expr : Ast {
    virtual ~Expr() = default;
    virtual llvm::Value* accept(AstVisitor& visitor) = 0;
};

struct NumberExpr : public Expr {
    double value;

    NumberExpr(double value) : value(value) {};
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct StringExpr : public Expr {
    std::string value;

    StringExpr(const std::string &value) : value(value) {};
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct CharExpr : public Expr {
    char value;

    CharExpr(char value) : value(value) {};
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct BoolExpr : public Expr {
    bool value;

    BoolExpr(bool value) : value(value) {};
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct VarExpr : public Expr {
    std::string name;
    TokenType t;

    VarExpr(const std::string &name, TokenType t) : name(name), t(t) {};
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct UnaryExpr : public Expr {
    std::string op;
    std::unique_ptr<Expr> rhs;

    UnaryExpr(std::string op, std::unique_ptr<Expr> rhs) : op(op), rhs(std::move(rhs)) {};
    UnaryExpr(UnaryExpr&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct BinaryExpr : public Expr {
    std::string op;
    std::unique_ptr<Expr> lhs, rhs;

    BinaryExpr(std::string op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {};
    BinaryExpr(BinaryExpr&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct CallExpr : public Expr {
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;

    CallExpr(const std::string &callee, std::vector<std::unique_ptr<Expr>> args) : callee(callee), args(std::move(args)) {};
    CallExpr(CallExpr&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct ArrayExpr : public Expr {
    std::unique_ptr<VarExpr> arr;
    std::unique_ptr<Expr> index;

    ArrayExpr(std::unique_ptr<VarExpr> arr, std::unique_ptr<Expr> index) : arr(std::move(arr)), index(std::move(index)) {};
    ArrayExpr(ArrayExpr&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

struct RecordExpr : public Expr {
    std::unique_ptr<VarExpr> record;
    std::string field;

    RecordExpr(std::unique_ptr<VarExpr> record, const std::string &field) : record(std::move(record)), field(field) {};
    RecordExpr(RecordExpr&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) override {
        return visitor.visit(*this);
    };
};

#endif
