#ifndef _CRACK_ERROR_H_
#define _CRACK_ERROR_H_
#include <stdarg.h>
#include <stdio.h>

typedef enum
{
    LOG_INFO,
    LOG_ERROR,
    LOG_WARNING,
    LOG_NONE
} LogType;

void log_msg(LogType logType, const char *errorMessage, ...);

void perror_at_line(char *src, size_t size, size_t lineStartIndex, size_t lineNo, size_t column, char *errorMsg, ...);
#endif