#ifndef CODEGENVISITOR_HPP
#define CODEGENVISITOR_HPP

#include "astVisitor.hpp"
#include "expr.hpp"
#include "decl.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "llvm.hpp"

class CodegenVisitor : public AstVisitor {
private:
    llvm::Value* LogErrorV(const char *str);
    llvm::Function* getFunction(std::string name);
    llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* TheFunction, llvm::StringRef name);

public:
    llvm::Value* visit(NumberExpr& ast);
    llvm::Value* visit(StringExpr& ast);
    llvm::Value* visit(CharExpr& ast);
    llvm::Value* visit(BoolExpr& ast);
    llvm::Value* visit(VarExpr& ast);
    llvm::Value* visit(UnaryExpr& ast);
    llvm::Value* visit(BinaryExpr& ast);
    llvm::Value* visit(CallExpr& ast);
    llvm::Value* visit(ArrayExpr& ast);
    llvm::Value* visit(RecordExpr& ast);

    llvm::Value* visit(ExprStmt& ast);
    llvm::Value* visit(CallStmt& ast);
    llvm::Value* visit(AssignStmt& ast);
    llvm::Value* visit(CompoundStmt& ast);
    llvm::Value* visit(IfStmt& ast);
    llvm::Value* visit(WhileStmt& ast);
    llvm::Value* visit(RepeatStmt& ast);
    llvm::Value* visit(ForStmt& ast);
    llvm::Value* visit(CaseStmt& ast);
    llvm::Value* visit(ReadStmt& ast);
    llvm::Value* visit(WriteStmt& ast);

    llvm::Function* visit(Prototype& ast);
    void visit(ConstDecl& ast);
    void visit(TypeDecl& ast);
    void visit(RangeType& ast);
    void visit(RecordType& ast);
    void visit(ArrayType& ast);
    void visit(EnumType& ast);
    void visit(VarDecl& ast);
    void visit(RecordVar& ast);
    void visit(ArrayVar& ast);
    llvm::Function* visit(FuncDecl& ast);
    llvm::Function* visit(ProcDecl& ast);
    void visit(Program& ast);
};

#endif