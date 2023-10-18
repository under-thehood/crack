#ifndef _CRACK_TOKEN_H_
#define _CRACK_TOKEN_H_
#include <stddef.h>

typedef enum
{
    TOK_IDENTIFIER,
    TOK_NUMBER,

    // Keywords
    TOK_LET,
    TOK_FN,
    TOK_IMPORT,
    TOK_STRUCT,
    TOK_MATCH,
    TOK_FOR,
    TOK_IF,
    TOK_ELSE,

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
    TOK_COMMA,
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

#ifdef _CRACK_DEBUG_
// FOR DEBUG ONLY

static char *tokenTypeValue[] = {

    "TOK_IDENTIFIER",
    "TOK_NUMBER",

    // Keywords
    "TOK_LET",
    "TOK_FN",
    "TOK_IMPORT",
    "TOK_STRUCT",
    "TOK_MATCH",
    "TOK_FOR",
    "TOK_IF",
    "TOK_ELSE",

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
    "TOK_COMMA",
    "TOK_EQUAL",

    // Status Tok
    "TOK_EOF",
    "TOK_UNKNOWN",
    "TOK_NEWLINE"};
#endif

#endif