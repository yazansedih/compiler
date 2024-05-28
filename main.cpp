#include "scanner.cpp"
#include "fd.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <String>

using namespace std;

int main(int argc, char **argv)
{
    FileDescriptor *fd;
    if (argc > 1)
    {
        fd = new FileDescriptor(argv[1]);
        if (!fd->IsOpen())
        {
            fprintf(stderr, "Failed to open file: %s\n", argv[1]);
            return 1;
        }
    }
    else
    {
        fd = new FileDescriptor();
    }

    SCANNER scanner(fd);
    TOKEN *token;
    while ((token = scanner.Scan())->type != lx_eof)
    {
        // Process the token
        printf("Token type: %d\n", token->type);
        delete token;
    }

    delete fd;
    return 0;
}