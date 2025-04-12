#include "lexer.hpp"
#include "token.hpp"
#include "stmt.hpp"
#include "decl.hpp"
#include "expr.hpp"
#include "parser.hpp"
#include "astVisitor.hpp"
#include "codegenVisitor.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

#include "llvm.hpp"

std::unique_ptr<llvm::LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("main", *TheContext);
std::map<std::string, llvm::AllocaInst*> NamedValues;
std::map<std::string, std::unique_ptr<Prototype>> FunctionProtos;

int main(int argc, char* argv[]) {
    std::ifstream file(argv[1]);
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::unique_ptr<Lexer> l = std::make_unique<Lexer>(buffer.str());
    //CodegenVisitor c = CodegenVisitor();

    Parser *p = new Parser(std::move(l));
    try {
        std::unique_ptr<Program> program = p->parse();
        //program->accept(c);
    } catch (std::runtime_error* &e) {
        std::cout << e->what() << std::endl;
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }

    delete (p);
    file.close();

    return 0;

}