#ifndef _CRACK_LEXER_H_
#define _CRACK_LEXER_H_

#include <stddef.h>
#include "../hashtable/hashtable.h"
#include "token.h"

typedef struct
{
    size_t currLineNo;
    size_t currLineStart;
    size_t currPos;

    String currTokenValue;
    String src;

    TokenInfo tokenInfo;

    HashTableStrInt keywords;
} Lexer;

void lexer_init(Lexer *lexer, String src);

TokenType lexer_next_token(Lexer *lexer);

#ifdef _CRACK_DEBUG_
void lexer_display_all_token(Lexer *lexer);
#endif

#endif
