#ifndef CODEGENVISITOR_HPP
#define CODEGENVISITOR_HPP

class NumberExpr;
class StringExpr;
class CharExpr;
class BoolExpr;
class VarExpr;
class UnaryExpr;
class BinaryExpr;
class CallExpr;
class ArrayExpr;
class RecordExpr;

class Prototype;
class ExprStmt;
class CallStmt;
class AssignStmt;
class CompoundStmt;
class IfStmt;
class WhileStmt;
class RepeatStmt;
class ForStmt;
class CaseStmt;
class ReadStmt;
class WriteStmt;

class ConstDecl;
class TypeDecl;
class RangeType;
class RecordType;
class ArrayType;
class EnumType;
class VarDecl;
class RecordVar;
class ArrayVar;
class FuncDecl;
class ProcDecl;
class Program;

class AstVisitor {
public:
    virtual ~AstVisitor() = default;
    
    virtual void visit(NumberExpr& ast);
    virtual void visit(StringExpr& ast);
    virtual void visit(CharExpr& ast);
    virtual void visit(BoolExpr& ast);
    virtual void visit(VarExpr& ast);
    virtual void visit(UnaryExpr& ast);
    virtual void visit(BinaryExpr& ast);
    virtual void visit(CallExpr& ast);
    virtual void visit(ArrayExpr& ast);
    virtual void visit(RecordExpr& ast);

    virtual void visit(Prototype& ast);
    virtual void visit(ExprStmt& ast);
    virtual void visit(CallStmt& ast);
    virtual void visit(AssignStmt& ast);
    virtual void visit(CompoundStmt& ast);
    virtual void visit(IfStmt& ast);
    virtual void visit(WhileStmt& ast);
    virtual void visit(RepeatStmt& ast);
    virtual void visit(ForStmt& ast);
    virtual void visit(CaseStmt& ast);
    virtual void visit(ReadStmt& ast);
    virtual void visit(WriteStmt& ast);

    virtual void visit(ConstDecl& ast);
    virtual void visit(TypeDecl& ast);
    virtual void visit(RangeType& ast);
    virtual void visit(RecordType& ast);
    virtual void visit(ArrayType& ast);
    virtual void visit(EnumType& ast);
    virtual void visit(VarDecl& ast);
    virtual void visit(RecordVar& ast);
    virtual void visit(ArrayVar& ast);
    virtual void visit(FuncDecl& ast);
    virtual void visit(ProcDecl& ast);
    virtual void visit(Program& ast);
};

#endif