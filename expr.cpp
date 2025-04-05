#include <string>
#include <vector>

struct Ast {
    virtual ~Ast() = default;
};

struct Expr : Ast {
    virtual ~Expr() = default;
};

struct NumberExpr : public Expr {
    double val;

    NumberExpr(double val) : val(val) {};
};

struct StringExpr : public Expr {
    std::string value;

    StringExpr(const std::string &value) : value(value) {};
};

struct BoolExpr : public Expr {
    bool value;

    BoolExpr(bool value) : value(value) {};
};

struct VarExpr : public Expr {
    std::string name;

    VarExpr(const std::string &name) : name(name) {};
};

struct UnaryExpr : public Expr {
    char op;
    std::unique_ptr<Expr> rhs;

    UnaryExpr(char op, std::unique_ptr<Expr> rhs) : op(op), rhs(std::move(rhs)) {};
};

struct BinaryExpr : public Expr {
    char op;
    std::unique_ptr<Expr> lhs, rhs;

    BinaryExpr(char op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {};
};

struct CallExpr : public Expr {
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;

    CallExpr(const std::string &callee, std::vector<std::unique_ptr<Expr>> args) : callee(callee), args(std::move(args)) {};
};

struct ArrayExpr : public Expr {
    std::unique_ptr<Expr> arr;
    std::unique_ptr<Expr> index;

    ArrayExpr(std::unique_ptr<Expr> arr, std::unique_ptr<Expr> index) : arr(std::move(arr)), index(std::move(index)) {};
};

struct RecordExpr : public Expr {
    std::unique_ptr<Expr> record;
    std::string field;

    RecordExpr(std::unique_ptr<Expr> record, std::string field) : record(std::move(record)), field(field) {};
};
