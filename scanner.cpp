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

        // std::cout << "Current character: " << c << std::endl;
        if (!isspace(c)) // Check if the next character is not a space
        {
            fd->UngetChar(c); // Go back one character in the token
        }

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

    static const char single_char_operators[] = {'(', ')', '+', '-', '*', '/', '=', '.', ';', '[', ']', ',', '{', '}'};
    for (const char &oper : single_char_operators)
    {
        if (oper == c)
        {
            return new TOKEN{OPERATOR, std::string(1, c), 0, 0.0};
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

    static const char single_char_operators2[] = {'(', ')', ':', '+', '-', '*', '/', '=', '<', '>', '.', ';', '[', ']', ',', '{', '}'};
    for (const char &oper : single_char_operators2)
    {
        if (oper == c)
        {
            return new TOKEN{OPERATOR, std::string(1, c), 0, 0.0};
        }
    }

    fd->ReportError((char *)"Unknown token");
    return new TOKEN{UNKNOWN, op, 0, 0.0};
}

std::string Scanner::tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case lx_identifier:
        return "Identifier";
    case lx_float:
        return "Float";
    case lx_integer:
        return "Integer";
    case lx_string:
        return "String";
    case lx_plus:
        return "Plus";
    case lx_minus:
        return "Minus";
    case lx_star:
        return "Star";
    case lx_slash:
        return "Slash";
    case lx_eq:
        return "Equals";
    case lx_neq:
        return "Not Equals";
    case lx_lt:
        return "Less Than";
    case lx_le:
        return "Less Equals";
    case lx_gt:
        return "Greater Than";
    case lx_ge:
        return "Greater Equals";
    case lx_lparen:
        return "Left Parenthesis";
    case lx_rparen:
        return "Right Parenthesis";
    case lx_lbracket:
        return "Left Bracket";
    case lx_rbracket:
        return "Right Bracket";
    case lx_colon:
        return "Colon";
    case lx_dot:
        return "Dot";
    case lx_semicolon:
        return "Semicolon";
    case lx_comma:
        return "Comma";
    case lx_colon_eq:
        return "Colon Equals";
    case kw_program:
        return "Keyword Program";
    case kw_var:
        return "Keyword Var";
    case kw_constant:
        return "Keyword Constant";
    case kw_integer:
        return "Keyword Integer";
    case kw_boolean:
        return "Keyword Boolean";
    case kw_string:
        return "Keyword String";
    case kw_float:
        return "Keyword Float";
    case kw_true:
        return "Keyword True";
    case kw_false:
        return "Keyword False";
    case kw_if:
        return "Keyword If";
    case kw_fi:
        return "Keyword Fi";
    case kw_then:
        return "Keyword Then";
    case kw_else:
        return "Keyword Else";
    case kw_while:
        return "Keyword While";
    case kw_do:
        return "Keyword Do";
    case kw_od:
        return "Keyword Od";
    case kw_and:
        return "Keyword And";
    case kw_or:
        return "Keyword Or";
    case kw_read:
        return "Keyword Read";
    case kw_write:
        return "Keyword Write";
    case kw_for:
        return "Keyword For";
    case kw_from:
        return "Keyword From";
    case kw_to:
        return "Keyword To";
    case kw_by:
        return "Keyword By";
    case kw_function:
        return "Keyword Function";
    case kw_procedure:
        return "Keyword Procedure";
    case kw_return:
        return "Keyword Return";
    case kw_not:
        return "Keyword Not";
    case kw_begin:
        return "Keyword Begin";
    case kw_end:
        return "Keyword End";
    case END_OF_FILE:
        return "End Of File";
    case UNKNOWN:
        return "Unknown";
    case COMMENT:
        return "Comment";
    case INTEGER_LITERAL:
        return "Integer Literal";
    case STRING_LITERAL:
        return "String Literal";
    case OPERATOR:
        return "Operator";
    case KEYWORD:
        return "Keyword";
    default:
        return "Undefined Token Type";
    }
}