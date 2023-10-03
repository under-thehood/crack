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
char string_iter_current(StringIter *iter)
{
    return *iter->curr;
}
void string_iter_advance(StringIter *iter)
{
    if (iter->curr + 1 <= iter->end)
        iter->curr++;
}
// give the curr char and advance
// char string_iter_curr_advance(StringIter *iter)
// {
//     char curr = string_iter_current(iter);
//     string_iter_advance(iter);
//     return curr;
// }

String string_create(char *str)
{
    size_t strLen = strlen(str);
    return (String){.str = str, .size = strLen};
}

String string_substring(String *src, size_t startIndex, size_t length)
{
    return (String){.str = src->str + (startIndex % src->size), .size = startIndex + length < src->size ? length : src->size - startIndex};
}
void string_display(String *string)
{
    char *start = string->str;
    for (size_t i = 0; i < string->size; i++)
    {
        putchar(start[i]);
    }
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
    TOK_COLON,
    TOK_EQUAL,

    // Status Tok
    TOK_EOF,
    TOK_UNKNOWN,
    TOK_NEWLINE

} TokenType;

typedef struct
{
    // start position is with respect to currentline(i.e. column)
    size_t column;
    size_t length;
} TokenInfo;

typedef struct
{
    size_t currLineNo;
    size_t currLineStart;
    size_t currPos;

    String currTokenValue;
    String src;

    TokenInfo tokenInfo;
} Lexer;

void lexer_init(Lexer *lexer, String src)
{
    lexer->currLineNo = 0;
    lexer->currLineStart = 0;
    lexer->currPos = 0;
    lexer->src = src;
}

TokenType lexer_parse_number(Lexer *lexer, StringIter *iter)
{
    size_t tokenStart = iter->curr - lexer->src.str;

    // TODO: Handle the float value
    while (is_digit(string_iter_current(iter)))
        string_iter_advance(iter);

    lexer->currPos = iter->curr - lexer->src.str;

    lexer->currTokenValue = string_substring(&lexer->src, tokenStart, lexer->currPos - tokenStart);
    lexer->tokenInfo = (TokenInfo){.column = tokenStart - lexer->currLineStart, .length = lexer->currTokenValue.size};
    return TOK_NUMBER;
}

TokenType lexer_parse_identifier(Lexer *lexer, StringIter *iter)
{
    size_t tokenStart = iter->curr - lexer->src.str;
    while (is_alpha_digit(string_iter_current(iter)) || string_iter_current(iter) == '_')
        string_iter_advance(iter);

    lexer->currPos = iter->curr - lexer->src.str;

    lexer->currTokenValue = string_substring(&lexer->src, tokenStart, lexer->currPos - tokenStart);
    lexer->tokenInfo = (TokenInfo){.column = tokenStart - lexer->currLineStart, .length = lexer->currTokenValue.size};
    return TOK_IDENTIFIER;
}

TokenType lexer_parse_symbol(Lexer *lexer, StringIter *iter)
{
    lexer->tokenInfo.column = iter->curr - (lexer->src.str + lexer->currLineStart);

    TokenType tokenType;
    size_t tokenStart = iter->curr - lexer->src.str;

    switch (string_iter_current(iter))
    {
    case '+':
        tokenType = TOK_PLUS;
        break;
    case '-':
        tokenType = TOK_MINUS;
        break;
    case '*':
        tokenType = TOK_STAR;
        break;
    case '/':
        tokenType = TOK_FORWARD_SLASH;
        break;
    case '(':
        tokenType = TOK_PAREN_OPEN;
        break;
    case ')':
        tokenType = TOK_PAREN_CLOSE;
        break;
    case '{':
        tokenType = TOK_CURLY_OPEN;
        break;
    case '}':
        tokenType = TOK_CURLY_CLOSE;
        break;
    case ':':
        tokenType = TOK_COLON;
        break;
    case '=':
        tokenType = TOK_EQUAL;
        break;
    case '\n':
        tokenType = TOK_NEWLINE;
        lexer->currLineNo++;
        break;
    default:
        tokenType = TOK_UNKNOWN;
        break;
    }
    string_iter_advance(iter);

    lexer->currPos = iter->curr - lexer->src.str;
    lexer->currTokenValue = string_substring(&lexer->src, tokenStart, lexer->currPos - tokenStart);
    lexer->tokenInfo.length = lexer->currTokenValue.size;

    return tokenType;
}

TokenType lexer_next_token(Lexer *lexer)
{
    if (lexer->currPos + 1 > lexer->src.size)
        return TOK_EOF;

    char currentCharacter = *(lexer->src.str + lexer->currPos);

    if (currentCharacter == '\n')
    {
        lexer->currLineStart = ++lexer->currPos;
        lexer->currLineNo++;
        return TOK_NEWLINE;
    }
    StringIter iter = string_iter_create(&lexer->src, lexer->currPos);
    while (string_iter_current(&iter) == ' ')
        string_iter_advance(&iter);

    currentCharacter = string_iter_current(&iter);
    // parse identifier
    if (is_alpha(currentCharacter) || currentCharacter == '_')
    {
        return lexer_parse_identifier(lexer, &iter);
    }

    // parse number
    if (is_digit(currentCharacter))
    {
        return lexer_parse_number(lexer, &iter);
    }

    return lexer_parse_symbol(lexer, &iter);
}

#define _LEXER_DEBUG_

#ifdef _LEXER_DEBUG_
// FOR DEBUG ONLY

static char *tokenTypeValue[] = {

    "TOK_IDENTIFIER",
    "TOK_NUMBER",

    // Keywords
    "TOK_LET",
    "TOK_FN",
    "TOK_IMPORT",

    // Operators
    "TOK_PLUS",
    "TOK_MINUS",
    "TOK_STAR",
    "TOK_FORWARD_SLASH",

    // Symbols
    "TOK_PAREN_OPEN",
    "TOK_PAREN_CLOSE",
    "TOK_CURLY_OPEN",
    "TOK_CURLY_CLOSE",
    "TOK_COLON",
    "TOK_EQUAL",

    // Status Tok
    "TOK_EOF",
    "TOK_UNKNOWN",
    "TOK_NEWLINE"};

void lexer_display_all_token(Lexer *lexer)
{
    TokenType tokenType = lexer_next_token(lexer);
    while (tokenType != TOK_EOF)
    {
        printf("{ TokenType: %s TokenValue: ", tokenTypeValue[tokenType]);
        string_display(&lexer->currTokenValue);
        printf("  LineNo: %d ColumnNo: %d", lexer->currLineNo, lexer->tokenInfo.column);
        printf(" } \n");
        tokenType = lexer_next_token(lexer);
    }
}
#endif

//=========================Test Section=======================
void test_lexer(Config *config)
{
    String fileContent;
    bool status = file_read(config->fileName, &fileContent);
    if (status == true)
    {
        Lexer lexer;
        lexer_init(&lexer, fileContent);
        lexer_display_all_token(&lexer);
    }
    else
    {
        log_msg(LOG_ERROR, "Cannot read the file!");
    }
}

//=========================Main Section======================
int main(int argc, char const *argv[])
{
    Config config = parse_command_argument(argc, argv);
    test_lexer(&config);
    return 0;
}
