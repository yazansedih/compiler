#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifndef FD_H
#define FD_H

#define SET 1
#define UNSET 0
#define BUFFER_SIZE 256

class FileDescriptor
{
private:
    FILE *fp;
    int line_number;
    int char_number;
    int flag;
    int buf_size;
    char *buffer;
    char *file;

    void doubleBufferSize();

public:
    FileDescriptor(char *FileName);
    ~FileDescriptor();
    FileDescriptor();
    void Close();
    char *GetFileName();
    bool IsOpen();
    char *GetCurrLine();
    int GetLineNum();
    int GetCharNum();
    char GetChar();
    void ReportError(const char *msg);
    void UngetChar(char c);
};

#endif // FD_H
