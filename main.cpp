#include "fd.h"
#include "scanner.h"
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    // Open the file
    FileDescriptor *fd;
    if (argc > 1)
    {
        fd = new FileDescriptor(argv[1]);
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << "test.txt" << std::endl;
        return 1;
    }

    if (!fd->IsOpen())
    {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    // Create a scanner object
    Scanner scanner(fd);
    TOKEN *token;

    // Scan tokens until end of file
    while ((token = scanner.scan())->type != END_OF_FILE)
    {
        std::cout << "Token: ";
        if (!token->str_ptr.empty())
        {
            std::cout << token->str_ptr;
        }
        else if (token->type == lx_integer || token->type == INTEGER_LITERAL)
        {
            std::cout << token->value;
        }
        else if (token->type == lx_float)
        {
            std::cout << token->float_value;
        }

        std::cout << " Type: " << scanner.tokenTypeToString(token->type) << std::endl;
        delete token;
    }

    // Delete the last token (END_OF_FILE token)
    delete token;
    delete fd;
    return 0;
}