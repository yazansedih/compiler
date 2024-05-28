#include "scanner.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

int keys = 32; /* number of keywords */
const char *keyword[] = {
    "and", "begin", "boolean", "by", "constant", "do", "else", "end", "false", "fi",
    "float", "for", "from", "function", "if", "integer", "not", "od", "or", "procedure",
    "program", "read", "return", "string", "then", "to", "true", "var", "while", "write"};

const LEXEME_TYPE key_type[] = {
    kw_and, kw_begin, kw_boolean, kw_by, kw_constant, kw_do, kw_else, kw_end, kw_false, kw_fi,
    kw_float, kw_for, kw_from, kw_function, kw_if, kw_integer, kw_not, kw_od, kw_or, kw_procedure,
    kw_program, kw_read, kw_return, kw_string, kw_then, kw_to, kw_true, kw_var, kw_while, kw_write};

SCANNER::SCANNER()
{
    Fd = nullptr;
}

void SCANNER::skip_comments()
{
    char c;
    while ((c = Fd->GetChar()) != EOF)
    {
        if (c == '#' && Fd->GetChar() == '#')
        {
            while ((c = Fd->GetChar()) != EOF && c != '\n')
            {
            }
            if (c == '\n')
            {
                Fd->UngetChar(c);
            }
            return;
        }
    }
}

bool SCANNER::check_keyword(const char *str, TOKEN *token)
{
    for (int i = 0; i < keys; ++i)
    {
        if (strcmp(str, keyword[i]) == 0)
        {
            token->type = key_type[i];
            return true;
        }
    }
    return false;
}

TOKEN *SCANNER::get_id()
{
    TOKEN *token = new TOKEN();
    char buffer[256];
    int length = 0;

    char c = Fd->GetChar();
    while (isalnum(c) || c == '_')
    {
        buffer[length++] = c;
        c = Fd->GetChar();
    }
    buffer[length] = '\0';
    Fd->UngetChar(c);

    if (check_keyword(buffer, token))
    {
        return token;
    }

    token->type = lx_identifier;
    token->str_ptr = strdup(buffer);
    return token;
}

TOKEN *SCANNER::get_string()
{
    TOKEN *token = new TOKEN();
    char buffer[1024];
    int length = 0;

    char c;
    while ((c = Fd->GetChar()) != '"' && c != EOF)
    {
        if (c == '\n')
        {
            ReportError("Unterminated string literal");
            delete token;
            return nullptr;
        }
        buffer[length++] = c;
    }
    buffer[length] = '\0';

    token->type = lx_string;
    token->str_ptr = strdup(buffer);
    return token;
}

TOKEN *SCANNER::get_int()
{
    TOKEN *token = new TOKEN();
    char buffer[256];
    int length = 0;
    bool is_float = false;

    char c = Fd->GetChar();
    while (isdigit(c) || c == '.')
    {
        if (c == '.')
        {
            is_float = true;
        }
        buffer[length++] = c;
        c = Fd->GetChar();
    }
    buffer[length] = '\0';
    Fd->UngetChar(c);

    if (is_float)
    {
        token->type = lx_float;
        token->float_value = atof(buffer);
    }
    else
    {
        token->type = lx_integer;
        token->value = atoi(buffer);
    }
    return token;
}

// function to scan tokens from the input file.
TOKEN *SCANNER::Scan()
{
    char c;
    while ((c = Fd->GetChar()) != EOF)
    {
        if (isspace(c))
        {
            continue;
        }
        if (c == '#')
        {
            char next = Fd->GetChar();
            if (next == '#')
            {
                skip_comments();
                continue;
            }
            else
            {
                Fd->UngetChar(next);
                continue;
            }
        }
        if (isalpha(c) || c == '_')
        {
            Fd->UngetChar(c);
            return get_id();
        }
        if (isdigit(c) || c == '.')
        {
            Fd->UngetChar(c);
            return get_int();
        }
        if (c == '"')
        {
            return get_string();
        }
        switch (c)
        {
        case '+':
            return new TOKEN{lx_plus};
        case '-':
            return new TOKEN{lx_minus};
        case '*':
            return new TOKEN{lx_star};
        case '/':
            return new TOKEN{lx_slash};
        case '=':
            return new TOKEN{lx_eq};
        case '!':
            if (Fd->GetChar() == '=')
                return new TOKEN{lx_neq};
            break;
        case '<':
            if (Fd->GetChar() == '=')
                return new TOKEN{lx_le};
            return new TOKEN{lx_lt};
        case '>':
            if (Fd->GetChar() == '=')
                return new TOKEN{lx_ge};
            return new TOKEN{lx_gt};
        case ':':
            if (Fd->GetChar() == '=')
                return new TOKEN{lx_colon_eq};
            return new TOKEN{lx_colon};
        case '.':
            return new TOKEN{lx_dot};
        case ';':
            return new TOKEN{lx_semicolon};
        case ',':
            return new TOKEN{lx_comma};
        case '(':
            return new TOKEN{lx_lparen};
        case ')':
            return new TOKEN{lx_rparen};
        case '[':
            return new TOKEN{lx_lbracket};
        case ']':
            return new TOKEN{lx_rbracket};
        case '{':
            return new TOKEN{lx_lbracket};
        case '}':
            return new TOKEN{lx_rbracket};
        default:
            ReportError("Unrecognized character");
            return new TOKEN{lx_eof};
        }
    }
    return new TOKEN{lx_eof};
}

void SCANNER::ReportError(const char *msg)
{
    printf("Error: \"%s\" on line %d of the input file\n", msg, Fd->GetLineNum());
}

int main(int argc, char **argv)
{
    FileDescriptor *fd;
    if (argc > 1)
    {
        fd = new FileDescriptor(argv[1]);
    }
    else
    {
        fd = new FileDescriptor();
    }

    SCANNER scanner(fd);
    TOKEN *token;
    while ((token = scanner.Scan()) && token->type != lx_eof)
    {
        switch (token->type)
        {
        case lx_identifier:
            printf("Identifier: %s\n", token->str_ptr);
            free(token->str_ptr);
            break;
        case lx_integer:
            printf("Integer: %d\n", token->value);
            break;
        case lx_float:
            printf("Float: %f\n", token->float_value);
            break;
        case lx_string:
            printf("String: %s\n", token->str_ptr);
            free(token->str_ptr);
            break;
        case lx_plus:
            printf("Operator: +\n");
            break;
        case lx_minus:
            printf("Operator: -\n");
            break;
        case lx_star:
            printf("Operator: *\n");
            break;
        case lx_slash:
            printf("Operator: /\n");
            break;
        case lx_eq:
            printf("Operator: =\n");
            break;
        case lx_neq:
            printf("Operator: !=\n");
            break;
        case lx_lt:
            printf("Operator: <\n");
            break;
        case lx_le:
            printf("Operator: <=\n");
            break;
        case lx_gt:
            printf("Operator: >\n");
            break;
        case lx_ge:
            printf("Operator: >=\n");
            break;
        case lx_colon:
            printf("Operator: :\n");
            break;
        case lx_dot:
            printf("Operator: .\n");
            break;
        case lx_semicolon:
            printf("Operator: ;\n");
            break;
        case lx_comma:
            printf("Operator: ,\n");
            break;
        case lx_colon_eq:
            printf("Operator: :=\n");
            break;
        case lx_lparen:
            printf("Operator: (\n");
            break;
        case lx_rparen:
            printf("Operator: )\n");
            break;
        case lx_lbracket:
            printf("Operator: [\n");
            break;
        case lx_rbracket:
            printf("Operator: ]\n");
            break;
        case lx_eof:
            printf("End of File\n");
            break;
        default:
            if (token->type >= kw_program && token->type <= kw_write)
            {
                printf("Keyword: %s\n", keyword[token->type - kw_program]);
            }
            else
            {
                printf("Unknown token type: %d\n", token->type);
            }
            break;
        }
        delete token;
    }

    delete fd;
    return 0;
}
