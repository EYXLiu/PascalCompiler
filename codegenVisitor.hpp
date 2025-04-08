#ifndef CODEGENVISITOR_HPP
#define CODEGENVISITOR_HPP

#include "astVisitor.hpp"
#include "expr.hpp"
#include "function.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include "llvm.hpp"

class codegenVisitor {
public:
    void visit(NumberExpr& ast);
    void visit(StringExpr& ast);
    void visit(CharExpr& ast);
    void visit(BoolExpr& ast);
    void visit(VarExpr& ast);
    void visit(UnaryExpr& ast);
    void visit(BinaryExpr& ast);
    void visit(CallExpr& ast);
    void visit(ArrayExpr& ast);
    void visit(RecordExpr& ast);

    void visit(Prototype& ast);
    void visit(ExprStmt& ast);
    void visit(CallStmt& ast);
    void visit(AssignStmt& ast);
    void visit(CompoundStmt& ast);
    void visit(IfStmt& ast);
    void visit(WhileStmt& ast);
    void visit(RepeatStmt& ast);
    void visit(ForStmt& ast);
    void visit(CaseStmt& ast);
    void visit(ReadStmt& ast);
    void visit(WriteStmt& ast);

    void visit(ConstDecl& ast);
    void visit(TypeDecl& ast);
    void visit(RangeType& ast);
    void visit(RecordType& ast);
    void visit(ArrayType& ast);
    void visit(EnumType& ast);
    void visit(VarDecl& ast);
    void visit(RecordVar& ast);
    void visit(ArrayVar& ast);
    void visit(FuncDecl& ast);
    void visit(ProcDecl& ast);
    void visit(Program& ast);
};

#endif