#include <iostream>
#include <cstring>
#include <cctype>
#include "fd.h"      // Assuming this header defines FileDescriptor
#include "scanner.h" // Assuming this header defines TOKEN, lx_eof, lx_identifier, etc.

using namespace std;

class SCANNER
{
private:
    FileDescriptor *Fd;
    int keywords;
    int operators;

    // Helper function to skip comments
    void skip_comments()
    {
        char ch;
        bool in_comment = false;
        while ((ch = Fd->readChar()) != EOF)
        {
            if (!in_comment && ch == '#')
            {
                char next_char = Fd->peek();
                if (next_char == '#')
                {
                    // Skip until "##" is encountered
                    while ((ch = Fd->readChar()) != EOF && ch != '\n')
                    {
                        if (ch == '#' && Fd->peek() == '#')
                        {
                            Fd->readChar(); // Consume the second '#'
                            cout << "Comment with double ## has been skipped." << endl;
                            return; // Exit after skipping one line
                        }
                    }
                }
                else
                {
                    // Error: single '#' found
                    Fd->ReportError("Single '#' found in input");
                    return;
                }
            }
            else if (in_comment && ch == '\n')
            {
                in_comment = false;
            }
        }
    }

    // Helper function to check if a string is a keyword
    TOKEN *check_keyword(const char *str)
    {
        if (str == nullptr)
        {
            return nullptr;
        }

        const char *keyword[] = {"if", "else", "while", "for", "int", "float", "double", "char", "void", "return", "break", "continue", "switch", "case", "default", "do", "auto", "const", "enum", "extern", "long", "register", "signed", "static", "struct", "typedef", "union", "unsigned", "volatile", "sizeof"};
        TOKEN_TYPE key_type[] = {lx_if, lx_else, lx_while, lx_for, lx_int, lx_float, lx_double, lx_char, lx_void, lx_return, lx_break, lx_continue, lx_switch, lx_case, lx_default, lx_do, lx_auto, lx_const, lx_enum, lx_extern, lx_long, lx_register, lx_signed, lx_static, lx_struct, lx_typedef, lx_union, lx_unsigned, lx_volatile, lx_sizeof};
        for (int i = 0; i < keywords; ++i)
        {
            if (strcmp(str, keyword[i]) == 0)
            {
                TOKEN *newToken = new TOKEN();
                newToken->type = key_type[i];
                size_t length = strlen(str) + 1; // Include space for null terminator
                newToken->str_ptr = new char[length];
                if (strcpy_s(newToken->str_ptr, length, str) != 0)
                {
                    cerr << "Error copying string." << endl;
                    delete newToken;
                    return nullptr;
                }
                cout << "The Token Type is Keyword." << endl;
                return newToken;
            }
        }
        return nullptr;
    }

    // Helper function to get an identifier token
    TOKEN *get_id(const char *first_char)
    {
        TOKEN *newToken = new TOKEN();
        newToken->type = lx_identifier;
        size_t length = strlen(first_char) + 1; // Include space for null terminator
        newToken->str_ptr = new char[length];
        if (strcpy_s(newToken->str_ptr, length, first_char) != 0)
        {
            cerr << "Error copying string." << endl;
            delete newToken;
            return nullptr;
        }
        cout << "The Token Type is Identifier." << endl;
        return newToken;
    }

    // Helper function to get an operator token
    TOKEN *get_op(const char *op)
    {
        const char *operator_list[] = {"(", ")", "+", "-", "*", "/", "=", ".", ";", "[", "]", ",", "{", "}", ":=", "!=", "<=", ">="};
        TOKEN_TYPE operator_type[] = {lx_left_paren, lx_right_paren, lx_plus, lx_minus, lx_times, lx_divide, lx_assign, lx_dot, lx_semicolon, lx_left_bracket, lx_right_bracket, lx_comma, lx_left_brace, lx_right_brace, lx_assign, lx_not_equal, lx_less_equal, lx_greater_equal};

        for (int i = 0; i < operators; ++i)
        {
            if (strcmp(op, operator_list[i]) == 0)
            {
                TOKEN *newToken = new TOKEN();
                newToken->type = operator_type[i];
                size_t length = strlen(op) + 1; // Include space for null terminator
                newToken->str_ptr = new char[length];
                if (strcpy_s(newToken->str_ptr, length, op) != 0)
                {
                    cerr << "Error copying string." << endl;
                    delete newToken;
                    return nullptr;
                }
                cout << "The Token Type is Operator." << endl;
                return newToken;
            }
        }
        return nullptr;
    }

    // Helper function to get an integer token
    TOKEN *get_int(const char *first_char)
    {
        TOKEN *newToken = new TOKEN();
        newToken->type = lx_integer;
        newToken->value = atoi(first_char); // convert string to int
        cout << "The Token Type is Integer." << endl;
        return newToken;
    }

    // Helper function to get a floating-point token
    TOKEN *get_float(const char *first_char)
    {
        TOKEN *newToken = new TOKEN();
        newToken->type = lx_float;
        newToken->float_value = atof(first_char); // convert string to float
        cout << "The Token Type is Float." << endl;
        return newToken;
    }

    // Helper function to get a string token
    TOKEN *get_string(const char *str)
    {
        TOKEN *newToken = new TOKEN();
        newToken->type = lx_string;
        size_t length = strlen(str) + 1; // Include space for null terminator
        newToken->str_ptr = new char[length];
        if (strcpy_s(newToken->str_ptr, length, str) != 0)
        {
            cerr << "Error copying string." << endl;
            delete newToken;
            return nullptr;
        }
        cout << "The Token Type is String." << endl;
        return newToken;
    }

public:
    SCANNER()
    {
        Fd = nullptr;
        keywords = 30;  // Number of keywords
        operators = 18; // Number of operators
    }

    SCANNER(FileDescriptor *fd)
    {
        Fd = fd;
        keywords = 30;  // Number of keywords
        operators = 18; // Number of operators
    }

    TOKEN *Scan()
    {
        if (Fd == nullptr)
        {
            cout << "Error: File descriptor is null." << endl;
            return nullptr;
        }

        char c;
        int line_number = 1;

        while ((c = Fd->readChar()) != EOF)
        {
            if (c == '\n')
            {
                line_number++;
            }

            if (c == '#')
            {
                char next_char = Fd->peek();
                if (next_char == '#')
                {
                    while ((c = Fd->readChar()) != '\n' && c != EOF)
                    {
                        if (c == '#' && Fd->peek() == '#')
                        {
                            Fd->readChar(); // Consume the second '#'
                            cout << "Comment with double ## has been skipped." << endl;
                            break;
                        }
                    }
                    line_number++;
                }
                else
                {
                    while ((c = Fd->readChar()) != '\n' && c != EOF)
                        ;
                    line_number++;
                }
            }
            else if (c == '\"')
            {
                string str = "";
                while ((c = Fd->readChar()) != '\"')
                {
                    if (c == EOF || c == '\n')
                    {
                        Fd->ReportError("Unexpected EOF or newline in string");
                        return nullptr;
                    }
                    str += c;
                }
                return get_string(str.c_str());
            }
            else if (isdigit(c) || (c == '-' && isdigit(Fd->peek())))
            {
                bool errorFlag = false;
                string num_string = "";
                num_string += c;
                while (isdigit(c = Fd->readChar()) || c == '.')
                {
                    if (isalpha(Fd->peek()))
                    {
                        while ((c = Fd->readChar()) != '\n')
                            ;
                        errorFlag = true;
                        break;
                    }
                    if (c == '.')
                    {
                        num_string += c;
                        while (isdigit(c = Fd->readChar()))
                        {
                            num_string += c;
                        }
                        Fd->ungett();
                        return get_float(num_string.c_str());
                    }
                    num_string += c;
                }
                if (!errorFlag)
                {
                    Fd->ungett();
                    return get_int(num_string.c_str());
                }
                Fd->ReportError("Error: Illegal token encountered");
            }
            else if (isalpha(c) || c == '_')
            {
                string word = "";
                word += c;
                while (isalnum(c = Fd->readChar()) || c == '_')
                {
                    word += c;
                }
                Fd->ungett();
                TOKEN *keyword_token = check_keyword(word.c_str());
                if (keyword_token != nullptr)
                {
                    return keyword_token;
                }
                else
                {
                    return get_id(word.c_str());
                }
            }
            else if (c == ':')
            {
                char next_char = Fd->peek();
                if (next_char == '=')
                {
                    Fd->readChar();
                    return get_op(":=");
                }
                else
                {
                    Fd->ReportError("Error: Illegal character encountered");
                    continue;
                }
            }
            else if (c == '!')
            {
                char next_char = Fd->peek();
                if (next_char == '=')
                {
                    Fd->readChar();
                    return get_op("!=");
                }
                else
                {
                    Fd->ReportError("Error: Illegal character encountered");
                    continue;
                }
            }
            else if (strchr("<>=+-*/;(),[]{}", c) != NULL)
            {
                string symbol = "";
                symbol += c;
                return get_op(symbol.c_str());
            }
            else if (isspace(c))
            {
                while (isspace(c = Fd->readChar()))
                {
                    if (c == '\n')
                    {
                        line_number++;
                    }
                }
                Fd->ungett();
            }
            else
            {
                string illegal_token = "";
                illegal_token += c;
                while (!isspace(c = Fd->readChar()) && c != EOF && c != '\n')
                {
                    illegal_token += c;
                }
                Fd->ReportError("Error: Illegal token encountered");
                continue;
            }
        }

        return new TOKEN{lx_eof, nullptr, 0, 0.0};
    }
};
