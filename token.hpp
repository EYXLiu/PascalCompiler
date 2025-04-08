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
    tok_set = -24,
    tok_integer = -25,
    tok_real = -26,
    tok_char = -27,
    tok_string = -28,
    tok_boolean = -29,
    tok_write = -30,
    tok_read = -31,

    tok_identifier = -32,
    tok_number = -33,
    tok_string_literal = -34,
    tok_char_literal = -35,
    tok_boolean_literal = -36,

    tok_plus = -37,
    tok_minus = -38,
    tok_multiply = -39,
    tok_divide = -40,
    tok_assign = -41,
    tok_equals = -42,
    tok_not_equals = -43,
    tok_less_than = -44,
    tok_less_equal = -45,
    tok_greater_than = -46,
    tok_greater_equal = -47,
    tok_and = -48,
    tok_or = -49,
    tok_not = -50,
    tok_mod = -51,
    tok_div = -52,

    tok_semicolon = -53,
    tok_colon = -54,
    tok_comma = -55,
    tok_dot = -56,
    tok_open_paren = -57,
    tok_close_paren = -58,
    tok_open_bracket = -59,
    tok_close_bracket = -60,
    tok_pointer = -61,
    tok_range = -62,
    
    tok_error = -63,
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, std::string value) : type(type), value(value) {}
};

#endif
