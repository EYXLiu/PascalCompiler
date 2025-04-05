#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum TokenType {
    tok_eof = -1,

    tok_program = -2, 
    tok_var = -3,
    tok_begin = -4,
    tok_end = -5,
    tok_procedure = -6,
    tok_function = -7,
    tok_if = -8,
    tok_then = -9,
    tok_else = -10,
    tok_while = -11,
    tok_do = -12,
    tok_for = -13,
    tok_to = -14,
    tok_downto = -15,
    tok_repeat = -16,
    tok_until = -17,
    tok_case = -18,
    tok_of = -19,
    tok_const = -20,
    tok_type = -21,
    tok_array = -22,
    tok_record = -23,
    tok_integer = -24,
    tok_real = -25,
    tok_char = -26,
    tok_string = -27,
    tok_boolean = -28,
    tok_write = -29,
    tok_read = -30,

    tok_identifier = -31,
    tok_number = -32,
    tok_string_literal = -33,

    tok_plus = -34,
    tok_minus = -35,
    tok_multiply = -36,
    tok_divide = -37,
    tok_assign = -38,
    tok_equals = -39,
    tok_not_equals = -40,
    tok_less_than = -41,
    tok_less_equal = -42,
    tok_greater_than = -43,
    tok_greater_equal = -44,
    tok_and = -45,
    tok_or = -46,
    tok_not = -47,
    tok_mod = -48,
    tok_div = -49,

    tok_semicolon = -50,
    tok_colon = -51,
    tok_comma = -52,
    tok_dot = -53, 
    tok_open_paren = -54, 
    tok_close_paren = -55,
    tok_open_bracket = -56,
    tok_close_bracket = -57,
    
    tok_error = -58,
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, std::string value) : type(type), value(value) {}
};

#endif
