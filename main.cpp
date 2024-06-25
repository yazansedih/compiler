#include "fd.h"
#include "scanner.h"
#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
    const char fileName[] = "test_scanner.txt";
    FileDescriptor fd(fileName);

    if (!fd.isOpen())
    {
        printf("Failed to open file: %s\n", fileName);
        return 1;
    }

    SCANNER scanner(&fd); // Pass FileDescriptor instance to SCANNER

    TOKEN *token;
    while ((token = scanner.Scan())->type != lx_eof)
    {
        switch (token->type)
        {
        case lx_identifier:
            printf("Identifier: %s\n", token->str_ptr);
            break;
        case lx_integer:
            printf("Integer: %d\n", token->value);
            break;
        case lx_string:
            printf("String: %s\n", token->str_ptr);
            break;
        case lx_float:
            printf("Float: %f\n", token->float_value);
            break;
        default:
            printf("Token type: %s\n", TOKEN_NAMES[token->type]);
            break;
        }
    }

    fd.close();
    return 0;
}
