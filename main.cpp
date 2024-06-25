#include <stdlib.h>
#include "./symbolTable/stable.h"
#include <ctime>
#include <iostream>
#include "./parser/parser.h"
#include "./parser/ast.h"

int main()
{
    FileDescriptor *fd = new FileDescriptor("./code.txt");
    FILE *fp;
    fp = fopen("./out.txt", "w");
    STable *table = new STable();
    Parser *p = new Parser(fd, table);
    ast_list *code = p->parseProgram();
    while (code != nullptr)
    {
        print_ast_node(fp, code->head);
        code = code->tail;
    }
    delete[] code;
    return 0;
}
