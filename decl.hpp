#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <vector>
#include "stmt.hpp"
#include "expr.hpp"
#include "token.hpp"
#include "llvm.hpp"

#include "astVisitor.hpp"

struct Decl : public Ast {
    std::string name;
    virtual ~Decl() = default;

    Decl(const std::string &name) : name(name) {};
};

struct ConstDecl : public Decl {
    std::unique_ptr<Expr> value;

    ConstDecl(const std::string &name, std::unique_ptr<Expr> value) : Decl(name), value(std::move(value)) {};
    ConstDecl(ConstDecl&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct TypeDecl : public Decl {
    TokenType type;

    TypeDecl(const std::string &name, TokenType type) : Decl(name), type(type) {};
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct RangeType : public Decl {
    TokenType type;
    std::unique_ptr<Expr> min; 
    std::unique_ptr<Expr> max;

    RangeType(const std::string &name, TokenType type, std::unique_ptr<Expr> min, std::unique_ptr<Expr> max) : Decl(name), type(type), min(std::move(min)), max(std::move(max)) {};
    RangeType(RangeType&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct RecordType : public Decl {
    std::vector<std::unique_ptr<TypeDecl>> values;

    RecordType(const std::string &name, std::vector<std::unique_ptr<TypeDecl>> values) : Decl(name), values(std::move(values)) {};
    RecordType(RecordType&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct ArrayType : public Decl {
    TokenType type;
    int min;
    int max;
    std::string identifier;

    ArrayType(const std::string &name, TokenType type, int min, int max, const std::string &identifier = "") : Decl(name), type(type), min(min), max(max), identifier(identifier) {};
    ArrayType(ArrayType&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct EnumType : public Decl {
    std::vector<std::pair<std::unique_ptr<Expr>, int>> values;

    EnumType(const std::string &name, std::vector<std::pair<std::unique_ptr<Expr>, int>> values) : Decl(name), values(std::move(values)) {};
    EnumType(EnumType&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct VarDecl : public Decl {
    TokenType type;
    std::string identifier;

    VarDecl(const std::string &name, TokenType type, const std::string &identifier = "") : Decl(name), type(type), identifier(identifier) {};
    VarDecl(VarDecl&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct RecordVar : public Decl {
    std::string record;
    std::vector<std::pair<std::string, std::unique_ptr<Decl>>> values;

    RecordVar(const std::string &name, const std::string &record, std::vector<std::pair<std::string, std::unique_ptr<Decl>>> values = {}) : Decl(name), record(record) {
        if (values.size() != 0) {
            this->values = std::move(values);
        } else {
            for (auto& a : values) {
                this->values.push_back(std::make_pair(a.first, nullptr));
            }
        }
    };
    RecordVar(RecordVar&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct ArrayVar : public Decl {
    TokenType type;
    int min;
    int max;
    std::vector<std::unique_ptr<Decl>> values;
    std::string identifier;

    ArrayVar(const std::string &name, TokenType type, int min, int max, const std::string &identifier = "") : Decl(name), type(type), min(min), max(max), identifier(identifier) {
        for (int i = min; i <= max; i++) {
            values.push_back(nullptr);
        }
    };
    ArrayVar(ArrayVar&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct Prototype : public Ast {
    std::string name;
    std::vector<std::unique_ptr<Decl>> args;

    Prototype(const std::string &name, std::vector<std::unique_ptr<Decl>> args) : name(name), args(std::move(args)) {};
    llvm::Function* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct FuncDecl : public Decl {
    std::unique_ptr<Prototype> proto;
    TokenType type;
    std::unique_ptr<CompoundStmt> body;

    FuncDecl(std::unique_ptr<Prototype> proto, TokenType type, std::vector<std::unique_ptr<Stmt>> body) : Decl(proto->name), proto(std::move(proto)), type(type), body(std::make_unique<CompoundStmt>(std::move(body))) {};
    FuncDecl(FuncDecl&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct ProcDecl : public Decl {
    std::unique_ptr<Prototype> proto;
    std::unique_ptr<CompoundStmt> body;

    ProcDecl(std::unique_ptr<Prototype> proto, std::vector<std::unique_ptr<Stmt>> body) : Decl(proto->name), proto(std::move(proto)), body(std::make_unique<CompoundStmt>(std::move(body))) {};
    ProcDecl(ProcDecl&&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

struct Program : public Ast {
    std::string name;
    std::vector<std::unique_ptr<Decl>> decls;
    std::vector<std::unique_ptr<Stmt>> body;

    Program(const std::string &name, std::vector<std::unique_ptr<Decl>> decls, std::vector<std::unique_ptr<Stmt>> body) : name(name), decls(std::move(decls)), body(std::move(body)) {};
    Program(Program&) noexcept = default;
    llvm::Value* accept(AstVisitor& visitor) {
        visitor.visit(*this);
    };
};

#endif

