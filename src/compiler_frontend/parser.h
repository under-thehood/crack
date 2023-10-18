#ifndef _CRACK_PARSER_H_
#define _CRACK_PARSER_H_

#include "lexer.h"
#include "../error.h"

typedef enum
{
    STATUS_ERROR = 0x01 << 0,
    STATUS_PANIC = 0x01 << 1,
    STATUS_WARNING = 0x01 << 2
} Status;

typedef struct
{
    Lexer *lexer;
    TokenType currTokenType;

    Status status;
} Parser;

void parser_init(Parser *parser, Lexer *lexer);

void parser_parse(Parser *parser);

#endif
