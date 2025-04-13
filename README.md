# PascalCompiler
Tech Stack: C++, LLVM, Object Oriented Programming Principles, Visitor Design Pattern

# About
* Lexer and Parser both for the Pascal Programming Language, allows for var, type, arrays, records (struct), functions, and procedures
* Follows modern C++ best practices, including the use of std::unique_ptr for memory management and clean separation via .hpp/.cpp files
* Global scope for LLVM infrastructure (Context, Builder, Module), as well as Values, Variables, Functions, and Records
* Implemented the Visitor Design Pattern to traverse and operate on the Abstract Syntax Tree (AST) during compilation stages such as semantic analysis and code generation
* Wrote Visitor Code for Expressions, so value access points, will continue to work on declare and value allocation (Decl and Stmt)

# Usage
* To us, compile the compiler using make, which will use g++ and LLVM configuration
* Run `./cpascal` and then the file name, eg. `./cpascal sample.pas` on the command line
* The above will compile and run the code
