#include "scanner.h"
#include <cctype>
#include <cstring>
#include <iostream>

Scanner::Scanner(FileDescriptor *fd) : fd(fd)
{
    initKeywords();
}

void Scanner::initKeywords()
{
    keywords["program"] = kw_program;
    keywords["var"] = kw_var;
    keywords["constant"] = kw_constant;
    keywords["integer"] = kw_integer;
    keywords["boolean"] = kw_boolean;
    keywords["string"] = kw_string;
    keywords["float"] = kw_float;
    keywords["true"] = kw_true;
    keywords["false"] = kw_false;
    keywords["if"] = kw_if;
    keywords["fi"] = kw_fi;
    keywords["then"] = kw_then;
    keywords["else"] = kw_else;
    keywords["while"] = kw_while;
    keywords["do"] = kw_do;
    keywords["od"] = kw_od;
    keywords["and"] = kw_and;
    keywords["or"] = kw_or;
    keywords["read"] = kw_read;
    keywords["write"] = kw_write;
    keywords["for"] = kw_for;
    keywords["from"] = kw_from;
    keywords["to"] = kw_to;
    keywords["by"] = kw_by;
    keywords["function"] = kw_function;
    keywords["procedure"] = kw_procedure;
    keywords["return"] = kw_return;
    keywords["not"] = kw_not;
    keywords["begin"] = kw_begin;
    keywords["end"] = kw_end;
}

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
            fd->ReportError((char *)"Unterminated string literal");
            return new TOKEN{UNKNOWN, "", 0, 0.0};
        }
        return new TOKEN{lx_string, str, 0, 0.0};
    }

    if (c == '#' && fd->GetChar() == '#')
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

    fd->ReportError((char *)"Unknown token");
    return new TOKEN{UNKNOWN, op, 0, 0.0};
}
