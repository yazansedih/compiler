#include "scanner.h"
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>

using namespace std;

SCANNER::SCANNER()
{
    Fd = nullptr;
}

SCANNER::SCANNER(FileDescriptor *fd)
{
    Fd = fd;
}

TOKEN *SCANNER::get_int()
{
    char ch;
    char *str = new char[Fd->getBufferSize()];
    int index = 0;

    bool is_float = false;
    bool errorFlag = false;

    const char *error_message = "Identifier cannot start with a number";

    ch = Fd->getChar();
    if (ch == '-')
    {
        str[index++] = ch;
        if (!isdigit(Fd->getChar()))
        {
            Fd->ungetChar(ch);
            TOKEN *tok = new TOKEN;
            tok->str_ptr = new char[strlen(error_message) + 1]; // Allocate memory for str_ptr
            strcpy(tok->str_ptr, error_message);                // Copy error_message to str_ptr
            tok->type = lx_minus;
            delete[] str;
            return tok;
        }
        else
            Fd->ungetChar(ch);
    }
    else
    {
        Fd->ungetChar(ch);
    }

    while ((ch = Fd->getChar()) != EOF)
    {
        if (isdigit(ch))
        {
            str[index++] = ch;
        }
        else if (ch == '.')
        {
            is_float = true;
            str[index++] = ch;
        }
        else if (isalpha(ch))
        {
            // Skip the entire invalid variable name
            while ((ch = Fd->getChar()) != EOF && isalnum(ch))
            {
                // Keep reading until we encounter a non-alphanumeric character
            }

            // Unget the non-alphanumeric character that caused the loop to break
            Fd->ungetChar(ch);

            Fd->reportError(const_cast<char *>(error_message)); // Reporting the error
            errorFlag = true;
            break;
        }
        else if (ch == ';' || ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
        {
            Fd->ungetChar(ch);
            break;
        }
        else
        {
            Fd->ungetChar(ch);
            break;
        }
    }

    str[index] = '\0';
    TOKEN *tok = new TOKEN;
    if (errorFlag)
    {
        tok->type = lx_error;
        tok->str_ptr = new char[strlen(error_message) + 1]; // Allocate memory for str_ptr
        strcpy(tok->str_ptr, error_message);                // Copy error_message to str_ptr
        delete[] str;
        return tok;
    }

    if (is_float)
    {
        if (str[index - 1] == '.')
        {
            Fd->reportError(const_cast<char *>("Bad floating point number")); // Reporting the error
            tok->type = lx_error;
            tok->str_ptr = nullptr;
            delete[] str;
            return tok;
        }
        tok->type = lx_float;
        tok->float_value = atof(str);
    }
    else
    {
        tok->type = lx_integer;
        tok->value = atoi(str);
    }

    delete[] str;
    return tok;
}

void SCANNER::skip_comments()
{
    TOKEN *token = new TOKEN;
    char ch;
    string comment = "##";

    // Collect characters until "##" is encountered or end of line
    while ((ch = Fd->getChar()) != EOF && ch != '\n')
    {
        comment += ch;
        if (ch == '#')
        {
            char nextChar = Fd->getChar();
            if (nextChar == '#')
            {
                comment += nextChar;
                Fd->reportError(("Unterminated comment: " + comment).c_str());
                return;
            }
        }
    }

    if (ch == '\n')
    {
        Fd->reportError(("Unterminated comment: " + comment).c_str());
        // token->type = lx_comment;
        // token->str_ptr = const_cast<char *>(comment.c_str());
    }
}

bool SCANNER::check_keyword(const char *str)
{
    int low = 0;
    int high = keys - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        int cmp = strcmp(keyword[mid], str);
        if (cmp == 0)
            return true;
        if (cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return false;
}

TOKEN *SCANNER::get_id()
{
    char ch;
    char *str = new char[Fd->getBufferSize()];
    int index = 0;
    bool errorFlag = false;

    while ((ch = Fd->getChar()) != EOF)
    {
        if (isalnum(ch) || ch == '_')
        {
            str[index++] = ch;
        }
        else if (ch == ';' || ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r' ||
                 ch == '*' || ch == '/' || ch == '+' || ch == '-' || ch == ')' ||
                 ch == '(' || ch == ':' || ch == ',' || ch == '[' || ch == ']' || ch == '=')
        {
            Fd->ungetChar(ch);
            break;
        }
        else
        {
            std::cerr << "Illegal character encountered: " << ch << std::endl;
            Fd->reportError("Illegal character");
            errorFlag = true;
            break;
        }
    }

    str[index] = '\0';
    TOKEN *tok = new TOKEN;

    if (errorFlag)
    {
        tok->type = lx_error;
        tok->str_ptr = nullptr;
        delete[] str;
        return tok;
    }

    if (check_keyword(str))
    {
        int keyword_index = -1;
        for (int i = 0; i < keys; i++)
        {
            if (strcmp(keyword[i], str) == 0)
            {
                keyword_index = i;
                break;
            }
        }

        tok->type = key_type[keyword_index];
    }
    else
    {
        tok->type = lx_identifier;
        tok->str_ptr = strdup(str); // Use strdup to allocate memory for duplicated string
    }

    delete[] str;
    return tok;
}

TOKEN *SCANNER::get_string()
{
    char ch;
    char *str = new char[Fd->getBufferSize()];
    int index = 0;
    bool escaped = false;

    while ((ch = Fd->getChar()) != EOF)
    {
        if (ch == '"')
        {
            if (!escaped)
                break;
            else
            {
                str[index++] = ch;
                escaped = false;
            }
        }
        else if (ch == '\\')
        {
            if (escaped)
            {
                str[index++] = ch;
                escaped = false;
            }
            else
            {
                escaped = true;
            }
        }
        else
        {
            str[index++] = ch;
            escaped = false;
        }
    }

    str[index] = '\0';
    TOKEN *tok = new TOKEN;
    tok->type = lx_string;
    tok->str_ptr = strdup(str); // Use strdup to allocate memory for duplicated string

    delete[] str;
    return tok;
}

TOKEN *SCANNER::Scan()
{
    char ch;
    TOKEN *token = new TOKEN;
    token->str_ptr = nullptr;

    while ((ch = Fd->getChar()) != EOF)
    {
        if (isspace(ch))
        {
            continue;
        }

        if (ch == '#')
        {
            char next_char = Fd->getChar();
            if (next_char == '#')
            {
                skip_comments();
                continue;
            }
            else
            {
                Fd->ungetChar(next_char);
                Fd->reportError("Single '#' found in input (expected '##')");
                token->type = lx_error;
                return token;
            }
        }

        if (isdigit(ch) || ch == '-')
        {
            Fd->ungetChar(ch);
            token = get_int();
            return token;
        }

        if (ch == '"')
        {
            token = get_string();
            return token;
        }

        if (isalpha(ch) || ch == '_')
        {
            Fd->ungetChar(ch);
            token = get_id();
            return token;
        }

        if (ch == ':')
        {
            if ((ch = Fd->getChar()) == '=')
            {
                token->type = lx_colon_eq;
                return token;
            }
            else
            {
                token->type = lx_colon;
                Fd->ungetChar(ch);
                return token;
            }
        }

        if (ch == '!')
        {
            if ((ch = Fd->getChar()) == '=')
            {
                token->type = lx_neq;
                return token;
            }
            else
            {
                Fd->ungetChar(ch);
                continue;
            }
        }

        if (ch == '<')
        {
            if ((ch = Fd->getChar()) == '=')
            {
                token->type = lx_le;
                return token;
            }
            else
            {
                token->type = lx_lt;
                Fd->ungetChar(ch);
                return token;
            }
        }

        if (ch == '>')
        {
            if ((ch = Fd->getChar()) == '=')
            {
                token->type = lx_ge;
                return token;
            }
            else
            {
                token->type = lx_gt;
                Fd->ungetChar(ch);
                return token;
            }
        }

        switch (ch)
        {
        case '(':
            token->type = lx_lparen;
            return token;
        case ')':
            token->type = lx_rparen;
            return token;
        case '[':
            token->type = lx_lbracket;
            return token;
        case ']':
            token->type = lx_rbracket;
            return token;
        case '{':
            token->type = lx_lbrace;
            return token;
        case '}':
            token->type = lx_rbrace;
            return token;
        case '.':
            token->type = lx_dot;
            return token;
        case ';':
            token->type = lx_semicolon;
            return token;
        case ',':
            token->type = lx_comma;
            return token;
        case '+':
            token->type = lx_plus;
            return token;
        case '-':
            token->type = lx_minus;
            return token;
        case '*':
            token->type = lx_star;
            return token;
        case '/':
            token->type = lx_slash;
            return token;
        case '=':
            token->type = lx_eq;
            return token;
        case '?':
            token->type = lx_error;
            return token;
        case '$':
            token->type = lx_error;
            return token;
        case '%':
            token->type = lx_error;
            return token;
        default:
            std::cerr << "Unrecognized character encountered: " << ch << std::endl;
            Fd->reportError("Unrecognized character");
            exit(EXIT_FAILURE);
        }
    }

    if (ch == EOF)
    {
        token->type = lx_eof;
        token->value = 0;
        token->float_value = 0.0;
    }

    return token;
}