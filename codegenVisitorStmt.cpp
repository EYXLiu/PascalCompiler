#include "codegenVisitor.hpp"

llvm::Function* CodegenVisitor::visit(Prototype& ast) {
    std::vector<llvm::Type*> Doubles(ast.args.size(), llvm::Type::getDoubleTy(*TheContext));
    
    llvm::FunctionType* FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);

    llvm::Function* F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, ast.name, TheModule.get());
    
    unsigned idx = 0;
    for (auto &A : F->args()) {
        A.setName(ast.args[idx++]->name);
    }
    
    return F;
}

llvm::Function* CodegenVisitor::visit(FuncDecl& ast) {
    llvm::Function* TheFunction = TheModule->getFunction(ast.proto->name);

    if (!TheFunction) {
        TheFunction = this->visit(*ast.proto);
    }

    if (!TheFunction) {
        return nullptr;
    }

    if (!TheFunction->empty()) {
        return (llvm::Function*)LogErrorV("Function Cannot be Redefined");
    }

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    NamedValues.clear();
    for (auto &A : TheFunction->args()) {
        llvm::AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, A.getName());
        Builder->CreateStore(&A, Alloca);
        NamedValues[std::string(A.getName())] = Alloca;
    }

    if (llvm::Value* RetVal = this->visit(*ast.body)) {
        Builder->CreateRet(RetVal);
        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}

llvm::Function* CodegenVisitor::visit(ProcDecl& ast) {
    llvm::Function* TheFunction = TheModule->getFunction(ast.proto->name);

    if (!TheFunction) {
        TheFunction = this->visit(*ast.proto);
    }

    if (!TheFunction) {
        return nullptr;
    }

    if (!TheFunction->empty()) {
        return (llvm::Function*)LogErrorV("Procedure Cannot be Redefined");
    }

    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    NamedValues.clear();
    for (auto &A : TheFunction->args()) {
        llvm::AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, A.getName());
        Builder->CreateStore(&A, Alloca);
        NamedValues[std::string(A.getName())] = Alloca;
    }

    if (this->visit(*ast.body)) {
        Builder->CreateRetVoid(); 
        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}