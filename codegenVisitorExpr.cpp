#include "codegenVisitor.hpp"

llvm::Value* CodegenVisitor::visit(NumberExpr& ast) {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(ast.value));
};

llvm::Value* CodegenVisitor::visit(StringExpr& ast) {
    llvm::Constant* str = llvm::ConstantDataArray::getString(*TheContext, ast.value, true);
    llvm::GlobalVariable* gStr = new llvm::GlobalVariable(
        *TheModule, 
        str->getType(),
        true,
        llvm::GlobalValue::PrivateLinkage,
        str,
        ".str"
    );

    llvm::Constant* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0);
    std::vector<llvm::Constant*> indices = { zero, zero };
    llvm::Constant* strPtr = llvm::ConstantExpr::getGetElementPtr(
        str->getType(),
        gStr,
        indices
    );

    return strPtr;
};

llvm::Value* CodegenVisitor::visit(CharExpr& ast) {
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(8, static_cast<unsigned char>(ast.value)));
};

llvm::Value* CodegenVisitor::visit(BoolExpr& ast) {
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(1, ast.value ? 1 : 0));
};

llvm::Value* CodegenVisitor::visit(VarExpr& ast) {
    llvm::AllocaInst* A = NamedValues[ast.name];
    if (!A) {
        std::string msg = "Unknown variable name: " + ast.name;
        return LogErrorV(msg.c_str());
    }

    return Builder->CreateLoad(A->getAllocatedType(), A, ast.name.c_str());
};

llvm::Value* CodegenVisitor::visit(UnaryExpr& ast) {
    llvm::Value* OpV = ast.rhs->accept(*this);
    if (!OpV) {
        return nullptr;
    }

    if (ast.op == "not") {
        OpV = Builder->CreateFCmpONE(OpV, llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), "cond");
        OpV = Builder->CreateNot(OpV, "nottmp");
        return Builder->CreateUIToFP(OpV, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == "-") {
        llvm::Value *Zero = llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0));
        return Builder->CreateFSub(Zero, OpV, "negtmp");
    }

    llvm::Function* f = getFunction(std::string("unary") + ast.op);
    if (!f) {
        return LogErrorV("Unknown Unary Operator");
    }

    return Builder->CreateCall(f, OpV, "unop");
}

llvm::Value* CodegenVisitor::visit(BinaryExpr& ast) {
    llvm::Value* L = ast.lhs->accept(*this);
    llvm::Value* R = ast.rhs->accept(*this);

    if (ast.op == "+") {
        return Builder->CreateFAdd(L, R, "addtmp");
    } else if (ast.op == "-") {
        return Builder->CreateFSub(L, R, "subtmp");
    } else if (ast.op == "*") {
        return Builder->CreateFMul(L, R, "multmp");
    } else if (ast.op == "/") {
        return Builder->CreateFDiv(L, R, "divtmp");
    } else if (ast.op == "<") {
        L = Builder->CreateFCmpULT(L, R, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == "<=") {
        L = Builder->CreateFCmpULE(L, R, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == ">") {
        L = Builder->CreateFCmpULT(R, L, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == ">=") {
        L = Builder->CreateFCmpULE(R, L, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == "<>") {
        L = Builder->CreateFCmpUNE(L, R, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == "==") {
        L = Builder->CreateFCmpUEQ(L, R, "cmptmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == "and") {
        L = Builder->CreateFCmpONE(L, llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), "lcond");
        R = Builder->CreateFCmpONE(R, llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), "rcond");
        L = Builder->CreateAnd(L, R, "andtmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == "or") {
        L = Builder->CreateFCmpONE(L, llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), "lcond");
        R = Builder->CreateFCmpONE(R, llvm::ConstantFP::get(*TheContext, llvm::APFloat(0.0)), "rcond");
        L = Builder->CreateOr(L, R, "ortmp");
        return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
    } else if (ast.op == "mod") {
        L = Builder->CreateFPToSI(L, llvm::Type::getInt32Ty(*TheContext), "lint");
        R = Builder->CreateFPToSI(R, llvm::Type::getInt32Ty(*TheContext), "rint");
        L = Builder->CreateSRem(L, R, "modtmp");
        return Builder->CreateSIToFP(L, llvm::Type::getDoubleTy(*TheContext), "inttmp");
    } else if (ast.op == "div") {
        L = Builder->CreateFPToSI(L, llvm::Type::getInt32Ty(*TheContext), "lint");
        R = Builder->CreateFPToSI(R, llvm::Type::getInt32Ty(*TheContext), "rint");
        L = Builder->CreateSDiv(L, R, "divtmp");
        return Builder->CreateSIToFP(L, llvm::Type::getDoubleTy(*TheContext), "inttmp");
    }
}

llvm::Value* CodegenVisitor::visit(CallExpr& ast) {
    llvm::Function* Callee = TheModule->getFunction(ast.callee);
    if (!Callee) {
        return LogErrorV("Unknown Function Referenced");
    }

    if (Callee->arg_size()!= ast.args.size()) {
        return LogErrorV("Incorrect Number of Arguments");
    }

    std::vector<llvm::Value*> Args;

    for (unsigned i = 0; i != ast.args.size(); i++) {
        Args.push_back(ast.args[i]->accept(*this));
        if (!Args.back()) {
            return nullptr;
        }
    }

    return Builder->CreateCall(Callee, Args, "calltmp");
}

llvm::Value* CodegenVisitor::visit(ArrayExpr& ast) {
    llvm::AllocaInst* A = NamedValues[ast.arr->name];
    if (!A) {
        std::string m = "Uknown Array Name" + ast.arr->name;
        return LogErrorV(m.c_str());
    }

    llvm::Value* I = ast.index->accept(*this);
    if (!I) {
        return nullptr;
    }

    I = Builder->CreateFPToSI(I, llvm::Type::getInt32Ty(*TheContext), "idx");

    llvm::Value* P = Builder->CreateGEP(
        A->getAllocatedType(),
        A, 
        { llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0), I },
        "arrayelement"
    );

    return Builder->CreateLoad(llvm::Type::getDoubleTy(*TheContext), P, "arrayload");
}
    
llvm::Value* CodegenVisitor::visit(RecordExpr& ast) {
    llvm::AllocaInst* A = NamedValues[ast.record->name];
    if (!A) {
        std::string m = "Uknown Record Name" + ast.record->name;
        return LogErrorV(m.c_str());
    }

    int fieldIndex = getFieldIndex(ast.record->name, ast.field);
    if (fieldIndex == -1) {
        return LogErrorV("Unknown Record Field");
    }

    llvm::Value* P = Builder->CreateGEP(
        A->getAllocatedType(),
        A, 
        {
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0),
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), fieldIndex)
        },
        "recordfield"
    );

    return Builder->CreateLoad(llvm::Type::getDoubleTy(*TheContext), P, "recordload");
}
