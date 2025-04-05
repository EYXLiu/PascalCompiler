#include "expr.cpp"

struct Stmt : public Ast {
    virtual ~Stmt() = default;
};

struct ExprStmt : public Stmt {
    std::unique_ptr<Expr> expr;

    ExprStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr)) {};
};

struct CallStmt : public Stmt {
    std::string callee;
    std::vector<std::unique_ptr<Expr>>  args;

    CallStmt(std::string callee, std::vector<std::unique_ptr<Expr>> args) : callee(callee), args(std::move(args)) {};
};

struct AssignmentStmt : public Stmt {
    std::string name;
    std::unique_ptr<Expr> value;

    AssignmentStmt(std::string name, std::unique_ptr<Expr> value) : name(name), value(std::move(value)) {};
};

struct CompoundStmt : public Stmt {
    std::vector<std::unique_ptr<Expr>> statements;

    CompoundStmt(std::vector<std::unique_ptr<Expr>> statements) : statements(std::move(statements)) {};
};

struct IfStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch=nullptr) : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {};
};

struct WhileStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body)) {};
};

struct RepeatStmt : public Stmt {
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> body;

    RepeatStmt(std::unique_ptr<Expr> condition, std::vector<std::unique_ptr<Stmt>> body) : condition(std::move(condition)), body(std::move(body)) {};
};

struct ForStmt : public Stmt {
    std::string name;
    std::unique_ptr<Expr> start;
    std::unique_ptr<Expr> end;
    bool isdownto;
    std::unique_ptr<Stmt> body;

    ForStmt(std::string name, std::unique_ptr<Expr> start, std::unique_ptr<Expr> end, bool isdownto, std::unique_ptr<Stmt> body) : name(name), start(std::move(start)), end(std::move(end)), isdownto(isdownto), body(std::move(body)) {};
};

struct CaseStmt : public Stmt {
    std::unique_ptr<Expr> expr; 
    std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Stmt>>> cases;
    std::unique_ptr<Stmt> elseBranch;

    CaseStmt(std::unique_ptr<Expr> expr, std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<Stmt>>> cases, std::unique_ptr<Stmt> elseBranch) : expr(std::move(expr)), cases(std::move(cases)), elseBranch(std::move(elseBranch)) {};
};

struct ReadStmt : public Stmt {
    std::vector<std::string> variables;

    ReadStmt(std::vector<std::string> variables) : variables(std::move(variables)) {};
};

struct WriteStmt : public Stmt {
    std::vector<std::unique_ptr<Expr>> exprs;

    WriteStmt(std::vector<std::unique_ptr<Expr>> exprs) : exprs(std::move(exprs)) {};
};
