#include "scanner.h"
#include <cctype>
#include <cstring>
#include <iostream>

Scanner::Scanner(FileDescriptor *fd) : fd(fd)
{
    // initKeywords();
}

const char *keyword[] = {
    "and", "begin", "boolean", "by", "constant",
    "do", "else", "end", "false", "fi", "float", "for", "from",
    "function", "if", "integer", "not", "od", "or", "procedure",
    "program", "read", "return", "string", "then", "to", "true",
    "var", "while", "write"};

LEXEME_TYPE key_type[] = {
    kw_and, kw_begin, kw_boolean, kw_by, kw_constant,
    kw_do, kw_else, kw_end, kw_false, kw_fi, kw_float,
    kw_for, kw_from, kw_function, kw_if, kw_integer, kw_not,
    kw_od, kw_or, kw_procedure, kw_program, kw_read, kw_return,
    kw_string, kw_then, kw_to, kw_true, kw_var, kw_while, kw_write};

const char *operator_list[] = {
    "(", ")", "[", "]",
    ":", ".", ";", ",", ":=",
    "+", "-", "*", "/",
    "=", "!=", "<", "<=", ">", ">=", ""};

LEXEME_TYPE operator_type[] = {
    lx_lparen, lx_rparen, lx_lbracket, lx_rbracket,
    lx_colon, lx_dot, lx_semicolon, lx_comma, lx_colon_eq,
    lx_plus, lx_minus, lx_star, lx_slash,
    lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge, lx_eof};

int keys = 30;
int operators = 20;

TOKEN *Scanner::scan()
{
    return getNextToken();
}

TOKEN *Scanner::getNextToken()
{
    char c = fd->GetChar();
    while (isspace(c))
    {
        c = fd->GetChar();
    }

    if (c == EOF)
    {
        return new TOKEN{END_OF_FILE, "", 0, 0.0};
    }

    if (isalpha(c) || c == '_')
    {
        std::string identifier(1, c);
        c = fd->GetChar();
        while (isalnum(c) || c == '_')
        {
            identifier += c;
            c = fd->GetChar();
        }
        fd->UngetChar(c);

        if (keywords.find(identifier) != keywords.end())
        {
            return new TOKEN{keywords[identifier], identifier, 0, 0.0};
        }
        else
        {
            return new TOKEN{lx_identifier, identifier, 0, 0.0};
        }
    }

    if (isdigit(c))
    {
        std::string number(1, c);
        c = fd->GetChar();
        while (isdigit(c))
        {
            number += c;
            c = fd->GetChar();
        }
        if (c == '.')
        {
            number += c;
            c = fd->GetChar();
            while (isdigit(c))
            {
                number += c;
                c = fd->GetChar();
            }
            fd->UngetChar(c);
            return new TOKEN{lx_float, number, 0, std::stod(number)};
        }
        fd->UngetChar(c);
        return new TOKEN{lx_integer, number, std::stoi(number), 0.0};
    }

    if (c == '"')
    {
        std::string str;
        c = fd->GetChar();
        while (c != '"' && c != EOF)
        {
            str += c;
            c = fd->GetChar();
        }
        if (c != '"')
        {
            fd->ReportError("Unterminated string literal");
            return new TOKEN{UNKNOWN, "", 0, 0.0};
        }
        return new TOKEN{lx_string, str, 0, 0.0};
    }

    if (c == '#')
    {
        c = fd->GetChar();
        if (c == '#')
        {
            std::string comment;
            c = fd->GetChar();
            while (c != '\n' && c != EOF)
            {
                comment += c;
                c = fd->GetChar();
            }
            return new TOKEN{COMMENT, comment, 0, 0.0};
        }
        else
        {
            fd->ReportError("Invalid comment format");
            return new TOKEN{UNKNOWN, "", 0, 0.0};
        }
    }

    std::string op(1, c);
    char next_char = fd->GetChar();
    op += next_char;

    static const std::string operators[] = {":=", "!=", "<=", ">="};
    for (const std::string &oper : operators)
    {
        if (oper == op)
        {
            return new TOKEN{OPERATOR, op, 0, 0.0};
        }
    }

    fd->UngetChar(next_char);
    op.pop_back(); // remove next_char from op

    static const std::string single_char_operators[] = {"(", ")", ":", "+", "-", "*", "/", "=", "<", ">", ".", ";", "[", "]", ",", "{", "}"};
    for (const std::string &oper : single_char_operators)
    {
        if (oper == op)
        {
            return new TOKEN{OPERATOR, op, 0, 0.0};
        }
    }

    fd->ReportError("Unknown token");
    return new TOKEN{UNKNOWN, op, 0, 0.0};
}
