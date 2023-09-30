#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <stdbool.h>

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

void display_usage(const char *exeName)
{
    // for starting only filename is taken
    fprintf(stderr, "Usage : %s <filename>\n\n", exeName);
}

// define the configuration of current build
typedef struct
{
    char const *fileName;
} Config;

Config parse_command_argument(int argc, char const *argv[])
{
    // TODO: handle error message correctly
    if (argc != 2)
    {
        log_msg(LOG_ERROR, "No input file is specified");
        display_usage(argv[0]);
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

typedef struct
{
    char *curr;
    char *end;
} StringIter;

StringIter string_iter_create(String *string, size_t index)
{
    return (StringIter){.curr = string->str + (index % string->size), .end = string->str + string->size};
}

char string_iter_advance(StringIter *iter)
{
    return iter->curr + 1 < iter->end ? *iter->curr++ : 0;
}

char string_iter_current(StringIter *iter)
{
    return *iter->curr;
}
String string_create(char *str)
{
    size_t strLen = strlen(str);
    return (String){.str = str, .size = strLen};
}

String string_substring(String *src, size_t startIndex, size_t length)
{
    return (String){.str = src->str + (startIndex % src->size), .size = startIndex + length < src->size ? length : src->size - startIndex};
}

bool is_alpha(char character)
{
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}
bool is_digit(char character)
{
    return (character >= '0' && character <= '9');
}

bool is_alpha_digit(char character)
{
    return is_alpha(character) || is_digit(character);
}

//======================File handling =================
typedef struct
{
    const char *fileName;
    size_t fileSize;
    FILE *fp;
} FileInfo;

FileInfo file_status(const char *fileName)
{
    FileInfo fInfo = (FileInfo){.fileName = fileName, .fileSize = 0};
    fInfo.fp = fopen(fileName, "r");
    if (fInfo.fp == NULL)
        return fInfo;

    // store previous position
    size_t prevPos = ftell(fInfo.fp);

    fseek(fInfo.fp, 0, SEEK_END);
    fInfo.fileSize = ftell(fInfo.fp);
    // restore previous state
    fseek(fInfo.fp, prevPos, SEEK_SET);

    return fInfo;
}
bool file_read(const char *fileName, String *string)
{
    FileInfo fileInfo = file_status(fileName);
    if (fileInfo.fp == NULL)
    {
        log_msg(LOG_ERROR, strerror(errno));
        return false;
    }

    char *buffer = (char *)malloc(sizeof(char) * fileInfo.fileSize);

    if (buffer == NULL)
    {
        log_msg(LOG_ERROR, "Cannot allocate the enough memory for file");
        return false;
    }
    // TODE: Handle file read error
    fread(buffer, sizeof(char), fileInfo.fileSize, fileInfo.fp);

    string->size = fileInfo.fileSize;
    string->str = buffer;
}
//=================Lexing section=======================
typedef enum
{
    TOK_IDENTIFIER,
    TOK_NUMBER,

    // Keywords
    TOK_LET,
    TOK_FN,
    TOK_IMPORT,

    // Operators
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_FORWARD_SLASH,

    // Symbols
    TOK_PAREN_OPEN,
    TOK_PAREN_CLOSE,
    TOK_CURLY_OPEN,
    TOK_CURLY_CLOSE,

    // Status Tok
    TOK_EOF,
    TOK_NEWLINE

} TokenType;

typedef struct
{
    size_t currLineNo;
    size_t currLineStart;
    size_t currPos;

    String currTokenValue;
    String src;
} Lexer;

void lexer_init(Lexer *lexer, String src)
{
    lexer->currLineNo = 0;
    lexer->currLineStart = 0;
    lexer->currPos = 0;
    lexer->src = src;
}

void lexer_parse_number(StringIter *iter)
{
    
}
void lexer_parse_identifier(StringIter *iter)
{

}

TokenType lexer_next_token(Lexer *lexer)
{
    if (lexer->currPos + 1 > lexer->src.size)
        return TOK_EOF;

    char currentCharacter = lexer->src.str + lexer->currPos;

    if (currentCharacter == '\n')
    {
        lexer->currLineStart = ++lexer->currPos;
        lexer->currLineNo++;
        return TOK_NEWLINE;
    }
    StringIter iter = string_iter_create(&lexer->src, lexer->currPos);
    while (string_iter_advance(&iter) == ' ')
        continue;

    currentCharacter = string_iter_current(&iter);
    // parse identifier
    if (is_alpha(currentCharacter) || currentCharacter == '_')
    {
    }

    // parse number
    if (is_digit(currentCharacter))
    {
    }
}

//=========================Main Section======================
int main(int argc, char const *argv[])
{
    Config config = parse_command_argument(argc, argv);

    String fileContent;
    bool status = file_read(config.fileName, &fileContent);

    return 0;
}
