#include "fd.h"
#include <string.h>

char *FileDescriptor::getFileName()
{
    if (isOpen())
        return file;
    else
    {
        strcpy(error, "Please open the file first");
        return error;
    }
}

bool FileDescriptor::isOpen()
{
    return (fp != NULL);
}

char *FileDescriptor::getCurrLine()
{
    return buffer;
}

int FileDescriptor::getLineNum()
{
    return line_number;
}

int FileDescriptor::getCharNum()
{
    return char_number;
}

FileDescriptor::FileDescriptor(const char *fileName)
{
    bufSize = BUFFER_SIZE;
    buffer = new char[bufSize];
    file = new char[strlen(fileName) + 1]; // Allocate memory for file name
    strcpy(file, fileName);                // Copy the file name
    error = new char[bufSize];
    char_number = 0;
    line_number = 0;
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        sprintf(error, "Failed to open file: %s", fileName);
        reportError(error);
    }
}

FileDescriptor::~FileDescriptor()
{
    delete[] buffer;
    delete[] file;
    delete[] error;
    if (fp != nullptr)
    {
        fclose(fp);
    }
}

FileDescriptor::FileDescriptor()
{
    bufSize = BUFFER_SIZE;
    buffer = new char[bufSize];
    file = new char[FILENAME_MAX]; // Use FILENAME_MAX for default file name
    strcpy(file, "main.txt");      // Default file name
    error = new char[bufSize];
    char_number = 0;
    line_number = 0;
    fp = fopen(file, "r");
    if (fp == NULL)
    {
        sprintf(error, "Failed to open default file: %s", file);
        reportError(error);
    }
}

void FileDescriptor::close()
{
    if (fp != nullptr)
        fclose(fp);
}

char FileDescriptor::getChar()
{
    char currentChar = fgetc(fp);
    if (currentChar == EOF)
        return EOF;
    buffer[char_number] = currentChar;
    if (currentChar == '\n')
    {
        char_number = 0;
        line_number++;
    }
    else
    {
        char_number++;
    }
    buffer[char_number] = '\0'; // Ensure buffer is null-terminated
    return currentChar;
}

void FileDescriptor::reportError(const char *msg)
{
    printf("Error: %s\n", msg);
}

void FileDescriptor::ungetChar(char c)
{
    if (char_number > 0)
    {
        char_number--;
        if (c == '\n')
        {
            line_number--;
        }
        fseek(fp, -1, SEEK_CUR);
    }
}

int FileDescriptor::getBufferSize()
{
    return bufSize;
}
