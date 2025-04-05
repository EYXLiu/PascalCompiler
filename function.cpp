#include <string>
#include <vector>
#include "stmt.cpp"

struct Prototype {
    std::string name;
    std::vector<std::string> args;
    
    Prototype(std::string name, std::vector<std::string> args) : name(name), args(std::move(args)) {};
};

struct Function {
    std::unique_ptr<Prototype> proto;
    std::unique_ptr<Stmt> body;

    Function(std::unique_ptr<Prototype> proto, std::unique_ptr<Stmt> body) : proto(std::move(proto)), body(std::move(body)) {};
};
