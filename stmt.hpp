#ifndef STMT_HPP
#define STMT_HPP

#include "expr.hpp"
#include "astVisitor.hpp"
#include "llvm.hpp"

struct Stmt : public Ast {
    virtual ~Stmt() = default;
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;

    ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {};
    ExprStmt(ExprStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct CallStmt : public Stmt {
    std::string callee;
    std::vector<std::unique_ptr<Expr>>  args;

    CallStmt(const std::string &callee, std::vector<std::unique_ptr<Expr>> args) : callee(callee), args(std::move(args)) {};
    CallStmt(CallStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct AssignStmt : public Stmt {
    std::unique_ptr<Expr> name;
    std::unique_ptr<Expr> value;

    AssignStmt(std::unique_ptr<Expr> name, std::unique_ptr<Expr> value) : name(std::move(name)), value(std::move(value)) {};
    AssignStmt(AssignStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct CompoundStmt : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;

    CompoundStmt(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements)) {};
    CompoundStmt(CompoundStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct IfStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch=nullptr) : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {};
    IfStmt(IfStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct WhileStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body)) {};
    WhileStmt(WhileStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct RepeatStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> body;

    RepeatStmt(std::unique_ptr<Expr> condition, std::vector<std::unique_ptr<Stmt>> body) : condition(std::move(condition)), body(std::move(body)) {};
    RepeatStmt(RepeatStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct ForStmt : public Stmt {
    std::string name;
    int start;
    int end;
    bool ischar;
    bool isdownto;
    std::unique_ptr<Stmt> body;

    ForStmt(const std::string &name, int start, int end, bool ischar, bool isdownto, std::unique_ptr<Stmt> body) : name(name), start(start), end(end), ischar(ischar), isdownto(isdownto), body(std::move(body)) {};
    ForStmt(ForStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct CaseStmt : public Stmt {
    std::unique_ptr<Expr> expr; 
    std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Stmt>>> cases;
    std::unique_ptr<Stmt> elseBranch;

    CaseStmt(std::unique_ptr<Expr> expr, std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Stmt>>> cases, std::unique_ptr<Stmt> elseBranch = nullptr) : expr(std::move(expr)), cases(std::move(cases)), elseBranch(std::move(elseBranch)) {};
    CaseStmt(CaseStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct ReadStmt : public Stmt {
    std::vector<std::string> variables;

    ReadStmt(std::vector<std::string> variables) : variables(std::move(variables)) {};
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct WriteStmt : public Stmt {
    std::vector<std::unique_ptr<Expr>> exprs;

    WriteStmt(std::vector<std::unique_ptr<Expr>> exprs) : exprs(std::move(exprs)) {};
    WriteStmt(WriteStmt&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

#endif
