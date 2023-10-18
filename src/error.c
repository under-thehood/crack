#include "error.h"

void log_msg(LogType logType, const char *errorMessage, ...)
{
    va_list arg;
    va_start(arg, errorMessage);

    switch (logType)
    {
    case LOG_INFO:
        fprintf(stderr, "[INFO] ");
        break;
    case LOG_WARNING:
        fprintf(stderr, "[WARNING] ");
        break;
    case LOG_ERROR:
        fprintf(stderr, "[ERROR] ");
        break;
    default:
        break;
    }
    vfprintf(stderr, errorMessage, arg);

    fprintf(stderr, "\n");
}

void perror_at_line(char *src, size_t size, size_t lineStartIndex, size_t lineNo, size_t column, char *errorMsg, ...)
{
    va_list arg;
    va_start(arg, errorMsg);

    char *startPtr = src + lineStartIndex;
    fprintf(stderr, "[ERROR %d:%d] ", lineNo, column);
    vfprintf(stderr, errorMsg, arg);
    putc('\n', stderr);
    char curr = *startPtr;

    while (curr != '\n' && lineStartIndex < size)
    {

        putc(curr, stderr);
        lineStartIndex++;
        startPtr++;
        curr = *startPtr;
    }
    putchar('\n');
    for (size_t i = 0; i < column; i++)
    {
        putc(' ', stderr);
    }
    putc('^', stderr);
    putc('\n', stderr);
}