#include "fd.h"

void FileDescriptor::doubleBufferSize()
{
    buf_size *= 2;
    buffer = (char *)realloc(buffer, buf_size);
    if (!buffer)
    {
        perror("Failed to reallocate buffer");
        exit(EXIT_FAILURE);
    }
}

FileDescriptor::FileDescriptor(char *FileName) : line_number(0), char_number(0), flag(UNSET), buf_size(BUFFER_SIZE)
{
    buffer = (char *)malloc(buf_size);
    if (!buffer)
    {
        perror("Failed to allocate buffer");
        exit(EXIT_FAILURE);
    }

    if (FileName)
    {
        file = strdup(FileName);
        fp = fopen(FileName, "r");
        if (!fp)
        {
            perror("Failed to open file");
            free(buffer);
            free(file);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        file = nullptr;
        fp = stdin;
    }
}

FileDescriptor::~FileDescriptor()
{
    if (fp && fp != stdin)
    {
        fclose(fp);
    }
    free(buffer);
    free(file);
}

FileDescriptor::FileDescriptor() : FileDescriptor(nullptr) {}

void FileDescriptor::Close()
{
    if (fp && fp != stdin)
    {
        fclose(fp);
        fp = nullptr;
    }
}

char *FileDescriptor::GetFileName()
{
    return file;
}

bool FileDescriptor::IsOpen()
{
    return fp != nullptr;
}

char *FileDescriptor::GetCurrLine()
{
    if (!fp || feof(fp))
        return nullptr;

    if (fgets(buffer, buf_size, fp) == nullptr)
    {
        return nullptr;
    }

    while (buffer[strlen(buffer) - 1] != '\n' && !feof(fp))
    {
        doubleBufferSize();
        if (fgets(buffer + strlen(buffer), buf_size - strlen(buffer), fp) == nullptr)
        {
            return buffer;
        }
    }

    line_number++;
    char_number = 0;
    return buffer;
}

int FileDescriptor::GetLineNum()
{
    return line_number;
}

int FileDescriptor::GetCharNum()
{
    return char_number;
}

char FileDescriptor::GetChar()
{
    if (fp == nullptr || feof(fp))
        return EOF;

    char c = fgetc(fp);
    if (c == '\n')
    {
        line_number++;
        char_number = 0;
    }
    else
    {
        char_number++;
    }

    return c;
}

void FileDescriptor::ReportError(const char *msg)
{
    std::cerr << "Error: " << msg << " at line " << line_number << ", char " << char_number << std::endl;
}

void FileDescriptor::UngetChar(char c)
{
    if (flag == SET)
        return;
    if (c != EOF && ungetc(c, fp) != EOF)
    {
        if (c == '\n')
        {
            line_number--;
        }
        else
        {
            char_number--;
        }
        flag = SET;
    }
}
