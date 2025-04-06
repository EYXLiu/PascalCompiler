#include "lexer.hpp"

Lexer::Lexer(std::string input) : input(std::move(input)) {};

char Lexer::peek() const {
    return position < input.length() ? input[position] : '\0';
}

char Lexer::next() {
    return position < input.length() ? input[position++] : '\0';
}

void Lexer::skipWhitespace() {
    while (isspace(peek())) {
        next();
    }
}

std::unique_ptr<Token> Lexer::number() {
    std::string value;
    while (isdigit(peek()) || peek() == '.') {
        value += next();
    }
    return std::make_unique<Token>(TokenType::tok_number, value);
}

std::unique_ptr<Token> Lexer::identifier() {
    std::string value;
    while (isalnum(peek()) || peek()== '_') {
        value += next();
    }

    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c){ return tolower(c); }
    );

    if (value == "program") return std::make_unique<Token>(TokenType::tok_program, value);
    if (value == "var") return std::make_unique<Token>(TokenType::tok_var, value);
    if (value == "begin") return std::make_unique<Token>(TokenType::tok_begin, value);
    if (value == "end") return std::make_unique<Token>(TokenType::tok_end, value);
    if (value == "procedure") return std::make_unique<Token>(TokenType::tok_procedure, value);
    if (value == "function") return std::make_unique<Token>(TokenType::tok_function, value);
    if (value == "if") return std::make_unique<Token>(TokenType::tok_if, value);
    if (value == "then") return std::make_unique<Token>(TokenType::tok_then, value);
    if (value == "else") return std::make_unique<Token>(TokenType::tok_else, value);
    if (value == "while") return std::make_unique<Token>(TokenType::tok_while, value);
    if (value == "do") return std::make_unique<Token>(TokenType::tok_do, value);
    if (value == "for") return std::make_unique<Token>(TokenType::tok_for, value);
    if (value == "to") return std::make_unique<Token>(TokenType::tok_to, value);
    if (value == "downto") return std::make_unique<Token>(TokenType::tok_downto, value);
    if (value == "repeat") return std::make_unique<Token>(TokenType::tok_repeat, value);
    if (value == "until") return std::make_unique<Token>(TokenType::tok_until, value);
    if (value == "case") return std::make_unique<Token>(TokenType::tok_case, value);
    if (value == "of") return std::make_unique<Token>(TokenType::tok_of, value);
    if (value == "const") return std::make_unique<Token>(TokenType::tok_const, value);
    if (value == "type") return std::make_unique<Token>(TokenType::tok_type, value);
    if (value == "array") return std::make_unique<Token>(TokenType::tok_array, value);
    if (value == "record") return std::make_unique<Token>(TokenType::tok_record, value);
    if (value == "set") return std::make_unique<Token>(TokenType::tok_set, value);
    if (value == "writeln") return std::make_unique<Token>(TokenType::tok_write, value);
    if (value == "readln") return std::make_unique<Token>(TokenType::tok_read, value);
    if (value == "and") return std::make_unique<Token>(TokenType::tok_and, value);
    if (value == "or") return std::make_unique<Token>(TokenType::tok_or, value);
    if (value == "not") return std::make_unique<Token>(TokenType::tok_not, value);
    if (value == "mod") return std::make_unique<Token>(TokenType::tok_mod, value);
    if (value == "div") return std::make_unique<Token>(TokenType::tok_div, value);
    if (value == "integeter") return std::make_unique<Token>(TokenType::tok_integer, value);
    if (value == "real") return std::make_unique<Token>(TokenType::tok_real, value);
    if (value == "char") return std::make_unique<Token>(TokenType::tok_char, value);
    if (value == "boolean") return std::make_unique<Token>(TokenType::tok_boolean, value);
    if (value == "string") return std::make_unique<Token>(TokenType::tok_string, value);
    if (value == "true" || value == "false") return std::make_unique<Token>(TokenType::tok_boolean_literal, value);
    

    return std::make_unique<Token>(TokenType::tok_identifier, value);
}

std::unique_ptr<Token> Lexer::special() {
    switch(peek()) {
        case ':':
            next();
            if (peek() == '=') {
                next(); 
                return std::make_unique<Token>(TokenType::tok_assign, ":=");
            }
            return std::make_unique<Token>(TokenType::tok_colon, ":");
        case '<':
            next();
            if (peek() == '=') {
                next();
                return std::make_unique<Token>(TokenType::tok_less_equal, "<=");
            }
            if (peek() == '>') {
                next();
                return std::make_unique<Token>(TokenType::tok_not_equals, "<>");
            }
            return std::make_unique<Token>(TokenType::tok_less_than, "<");
        case '>':
            next();
            if (peek() == '=') {
                next();
                return std::make_unique<Token>(TokenType::tok_greater_equal, ">=");
            }
            return std::make_unique<Token>(TokenType::tok_greater_than, ">");
        case '=':
            next();
            if (peek() == '=') {
                next();
                return std::make_unique<Token>(TokenType::tok_equals, "==");
            }
            return std::make_unique<Token>(TokenType::tok_assign, "=");
        case '!':
            next();
            if (peek() != '=') {
                return std::make_unique<Token>(TokenType::tok_error, "");
            }
            next();
            return std::make_unique<Token>(TokenType::tok_not_equals, "!=");
        case '+':
            next();
            return std::make_unique<Token>(TokenType::tok_plus, ";");
        case '-':
            next();
            return std::make_unique<Token>(TokenType::tok_minus, ";");
        case 'x':
            next();
            return std::make_unique<Token>(TokenType::tok_multiply, ";");
        case '/':
            next();
            return std::make_unique<Token>(TokenType::tok_divide, ";");    
        case ';':
            next();
            return std::make_unique<Token>(TokenType::tok_semicolon, ";");
        case '.':
            next();
            if (peek() == '.') {
                next();
                return std::make_unique<Token>(TokenType::tok_range, "..");
            }
            return std::make_unique<Token>(TokenType::tok_dot, ".");
        case ',':
            next();
            return std::make_unique<Token>(TokenType::tok_comma, ",");
        case '(':
            next();
            return std::make_unique<Token>(TokenType::tok_open_paren, "(");
        case ')':
            next();
            return std::make_unique<Token>(TokenType::tok_close_paren, ")");
        case '[':
            next();
            return std::make_unique<Token>(TokenType::tok_open_bracket, "[");
        case ']':
            next();
            return std::make_unique<Token>(TokenType::tok_close_bracket, "]");
        case '^':
            next();
            return std::make_unique<Token>(TokenType::tok_pointer, "^");
    }

    return std::make_unique<Token>(TokenType::tok_identifier, std::string(1, next()));
}

std::unique_ptr<Token> Lexer::stringLiteral() {
    next();
    std::string value;
    while (peek() != '"' && peek() != '\0') {
        value += next();
    }

    if (peek() == '"') {
        next();
        return std::make_unique<Token>(TokenType::tok_string_literal, value);
    }

    return std::make_unique<Token>(TokenType::tok_error, value);
}

std::unique_ptr<Token> Lexer::charLiteral() {
    next();
    std::string value;
    value += next();
    if (peek() == '\'') {
        next();
        return std::make_unique<Token>(TokenType::tok_char_literal, value);
    }
    return std::make_unique<Token>(TokenType::tok_error, value);
}

std::unique_ptr<Token> Lexer::nextToken() {
    skipWhitespace();

    char current = peek();

    if (current == '\0') {
        return std::make_unique<Token>(TokenType::tok_eof, "");
    }

    if (isdigit(current)) {
        return number();
    }

    if (isalpha(current) || current == '_') {
        return identifier();
    }

    if (current == '"') {
        return stringLiteral();
    }

    if (current == '\'') {
        return charLiteral();
    }

    return special();
}
