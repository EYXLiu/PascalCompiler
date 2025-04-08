#include "lexer.hpp"
#include "token.hpp"
#include "stmt.hpp"
#include "function.hpp"
#include "expr.hpp"
#include "parser.hpp"
#include "astVisitor.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

#include "llvm.hpp"

std::unique_ptr<llvm::LLVMContext> TheContext = std::make_unique<llvm::LLVMContext>();
std::unique_ptr<llvm::IRBuilder<>> Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
std::unique_ptr<llvm::Module> TheModule = std::make_unique<llvm::Module>("Module", *TheContext);
std::map<std::string, llvm::Value *> NamedValues;



int main() {
    std::ifstream file("sample.pas");
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::unique_ptr<Lexer> l = std::make_unique<Lexer>(buffer.str());

    Parser *p = new Parser(std::move(l));
    try {
        p->parse();
    } catch (std::runtime_error* &e) {
        std::cout << e->what() << std::endl;
    } catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
    }

    delete (p);
    file.close();

    return 0;

}