#include <stdlib.h>
#include "./symbolTable/stable.h"
#include <ctime>
#include <iostream>
#include "./parser/parser.h"
#include "./parser/ast.h"
#include "./scanner/fd.h"

using namespace std;

int main()
{
    FileDescriptor *fd = new FileDescriptor("C:/Users/baraa/OneDrive/Documents/GitHub/compiler/code.txt");
    FILE *fp;
    fp = fopen("C:/Users/baraa/OneDrive/Documents/GitHub/compiler/out.txt", "w");
    STable *table = new STable();
    Parser *p = new Parser(fd, table);
    ast_list *code = p->parseProgram();
    while (code != nullptr)
    {
        cout << "Done" << endl;
        print_ast_node(fp, code->head);
        code = code->tail;
    }
    delete[] code;
    return 0;
}
