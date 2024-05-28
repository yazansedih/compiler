#ifndef FD_H
#define FD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstring> 

class FileDescriptor {
private:
    FILE *fp;
    int line_number;
    int char_number;
    int flag;
    int buf_size;
    char *buffer;
    char *file;
    int flag2;

public:
    FileDescriptor(char *FileName);
    ~FileDescriptor();
    FileDescriptor();
    void Close();
    char GetChar();
    void UngetChar(char c);
    void ReportError(const char *msg);
    char *GetFileName();
    bool IsOpen();
    char *GetCurrLine();
    int GetLineNum();
    int GetCharNum();
};

#endif // FD_H
