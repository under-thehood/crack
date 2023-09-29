#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//=======================Error handling=================
typedef enum
{
    LOG_INFO,
    LOG_ERROR,
    LOG_WARNING,
    LOG_NONE
} LogType;

void log_msg(LogType logType, const char *errorMessage, ...)
{
    va_list arg;
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

//========================CLI handling=============

void displayUsage(const char *exeName)
{
    // for starting only filename is taken
    fprintf(stderr, "Usage : %s <filename>\n\n", exeName);
}

// define the configuration of current build
typedef struct
{
    char const *fileName;
} Config;

Config parseCommandArgument(int argc, char const *argv[])
{
    // TODO: handle error message correctly
    if (argc != 2)
    {
        log_msg(LOG_ERROR, "No input file is specified");
        displayUsage(argv[0]);
        exit(EXIT_FAILURE);
    }
    return (Config){.fileName = argv[1]};
}

//=======================String handling================
typedef struct
{
    char *str;
    size_t size;
} String;

String string_create(char *str)
{
    size_t strLen = strlen(str);
    return (String){.str = str, .size = strLen};
}

String string_substring(String *src, size_t startIndex, size_t length)
{
}

//======================File handling =================

//=================Lexing section=======================

int main(int argc, char const *argv[])
{
    Config config = parseCommandArgument(argc, argv);

    return 0;
}
