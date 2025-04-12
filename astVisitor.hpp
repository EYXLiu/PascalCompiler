#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "llvm.hpp"

struct NumberExpr;
struct StringExpr;
struct CharExpr;
struct BoolExpr;
struct VarExpr;
struct UnaryExpr;
struct BinaryExpr;
struct CallExpr;
struct ArrayExpr;
struct RecordExpr;

struct Prototype;
struct ExprStmt;
struct CallStmt;
struct AssignStmt;
struct CompoundStmt;
struct IfStmt;
struct WhileStmt;
struct RepeatStmt;
struct ForStmt;
struct CaseStmt;
struct ReadStmt;
struct WriteStmt;

struct ConstDecl;
struct TypeDecl;
struct RangeType;
struct RecordType;
struct ArrayType;
struct EnumType;
struct VarDecl;
struct RecordVar;
struct ArrayVar;
struct FuncDecl;
struct ProcDecl;
struct Program;

class AstVisitor {
public:
    virtual ~AstVisitor() = default;
    
    virtual llvm::Value* visit(NumberExpr& ast);
    virtual llvm::Value* visit(StringExpr& ast);
    virtual llvm::Value* visit(CharExpr& ast);
    virtual llvm::Value* visit(BoolExpr& ast);
    virtual llvm::Value* visit(VarExpr& ast);
    virtual llvm::Value* visit(UnaryExpr& ast);
    virtual llvm::Value* visit(BinaryExpr& ast);
    virtual llvm::Value* visit(CallExpr& ast);
    virtual llvm::Value* visit(ArrayExpr& ast);
    virtual llvm::Value* visit(RecordExpr& ast);

    virtual llvm::Value* visit(ExprStmt& ast);
    virtual llvm::Value* visit(CallStmt& ast);
    virtual llvm::Value* visit(AssignStmt& ast);
    virtual llvm::Value* visit(CompoundStmt& ast);
    virtual llvm::Value* visit(IfStmt& ast);
    virtual llvm::Value* visit(WhileStmt& ast);
    virtual llvm::Value* visit(RepeatStmt& ast);
    virtual llvm::Value* visit(ForStmt& ast);
    virtual llvm::Value* visit(CaseStmt& ast);
    virtual llvm::Value* visit(ReadStmt& ast);
    virtual llvm::Value* visit(WriteStmt& ast);

    virtual llvm::Function* visit(Prototype& ast);
    virtual void visit(ConstDecl& ast);
    virtual void visit(TypeDecl& ast);
    virtual void visit(RangeType& ast);
    virtual void visit(RecordType& ast);
    virtual void visit(ArrayType& ast);
    virtual void visit(EnumType& ast);
    virtual void visit(VarDecl& ast);
    virtual void visit(RecordVar& ast);
    virtual void visit(ArrayVar& ast);
    virtual llvm::Function* visit(FuncDecl& ast);
    virtual llvm::Function* visit(ProcDecl& ast);
    virtual void visit(Program& ast);
};

#endif