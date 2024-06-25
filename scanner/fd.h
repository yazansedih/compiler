#ifndef __FD__H
#define __FD__H

#include <stdio.h>
#include <string.h>

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
    int bufSize;
    char *buffer;
    char *file;
    int flag2;
    char *error;

public:
    char *getFileName();
    bool isOpen();
    char *getCurrLine();
    int getLineNum();
    int getCharNum();
    FileDescriptor(const char *fileName); // Updated to const char*
    ~FileDescriptor();
    FileDescriptor(); // Default constructor
    void close();
    char getChar();
    void reportError(const char *msg); // Updated to const char*
    void ungetChar(char c);
    int getBufferSize();
    FILE *getFilePointer()
    {
        return fp;
    }
};

#endif