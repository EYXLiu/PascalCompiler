#ifndef LLVM_HPP
#define LLVM_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h> 
#include <map>

struct Prototype;

extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::map<std::string, llvm::AllocaInst *> NamedValues;
extern std::map<std::string, std::string> VariableTypeMap;
extern std::map<std::string, std::unique_ptr<Prototype>> FunctionProtos;
struct RecordInfo;
extern std::map<std::string, RecordInfo> RecordTypes;


#endif