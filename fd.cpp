#include "fd.h"

FileDescriptor::FileDescriptor(char *FileName)
{
    if (FileName == nullptr)
    {
        fp = stdin;
    }
    else
    {
        fp = fopen(FileName, "r");
        if (!fp)
        {
            perror(FileName);
            exit(EXIT_FAILURE);
        }
    }
    line_number = 1;
    char_number = 0;
    flag = UNSET;
    buf_size = BUFFER_SIZE;
    buffer = new char[buf_size];
    file = FileName ? strdup(FileName) : nullptr;
    flag2 = UNSET;
}

FileDescriptor::~FileDescriptor()
{
    if (fp != stdin)
    {
        fclose(fp);
    }
    delete[] buffer;
    if (file)
    {
        free(file);
    }
}

FileDescriptor::FileDescriptor() : FileDescriptor(nullptr) {}

void FileDescriptor::Close()
{
    if (fp != stdin)
    {
        fclose(fp);
    }
}

char FileDescriptor::GetChar()
{
    if (flag == SET)
    {
        flag = UNSET;
        return buffer[char_number++];
    }
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

void FileDescriptor::UngetChar(char c)
{
    if (flag == SET)
    {
        flag2 = SET;
    }
    else
    {
        flag = SET;
        buffer[char_number - 1] = c;
        char_number--;
        if (c == '\n')
        {
            line_number--;
        }
    }
}

void FileDescriptor::ReportError(char *msg)
{
    printf("%s\n", buffer);
    for (int i = 0; i < char_number; i++)
    {
        printf(" ");
    }
    printf("^\n");
    printf("Error: \"%s\" on line %d\n", msg, line_number);
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
