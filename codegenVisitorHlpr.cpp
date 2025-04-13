#include "codegenVisitor.hpp"

llvm::Value* CodegenVisitor::LogErrorV(const char *str) {
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
};

llvm::Function* CodegenVisitor::getFunction(std::string name) {
    if (auto* F = TheModule->getFunction(name)) {
        return F;
    };

    auto FI = FunctionProtos.find(name);
    if (FI != FunctionProtos.end()) {
        return FI->second->accept(*this);
    }

    return nullptr;
};

llvm::AllocaInst* CodegenVisitor::CreateEntryBlockAlloca(llvm::Function* TheFunction, llvm::StringRef name) {
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*TheContext), nullptr, name);
};

int CodegenVisitor::getFieldIndex(const std::string& recordName, const std::string& fieldName) {
    auto varTypeIt = VariableTypeMap.find(recordName);
    if (varTypeIt == VariableTypeMap.end()) {
        return -1;
    }

    std::string recordType = varTypeIt->second;
    auto recIt = RecordTypes.find(recordType);
    if (recIt == RecordTypes.end()) {
        return -1;
    }

    auto fieldIt = recIt->second.fieldIndices.find(fieldName);
    if (fieldIt == recIt->second.fieldIndices.end()) {
        return -1;
    }

    return fieldIt->second;
}