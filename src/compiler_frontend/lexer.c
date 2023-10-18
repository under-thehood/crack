
#include "lexer.h"

void lexer_init(Lexer *lexer, String src)
{
    lexer->currLineNo = 0;
    lexer->currLineStart = 0;
    lexer->currPos = 0;
    lexer->src = src;

    hash_table_strint_init(&lexer->keywords, 25);

    hash_table_strint_add(&lexer->keywords, string_create("fun"), TOK_FN);
    hash_table_strint_add(&lexer->keywords, string_create("import"), TOK_IMPORT);
    hash_table_strint_add(&lexer->keywords, string_create("let"), TOK_LET);
    hash_table_strint_add(&lexer->keywords, string_create("struct"), TOK_STRUCT);
    hash_table_strint_add(&lexer->keywords, string_create("match"), TOK_MATCH);
    hash_table_strint_add(&lexer->keywords, string_create("for"), TOK_FOR);
    hash_table_strint_add(&lexer->keywords, string_create("if"), TOK_IF);
    hash_table_strint_add(&lexer->keywords, string_create("else"), TOK_ELSE);
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

    EntryStrInt entryValue;
    if (hash_table_strint_get_value(&lexer->keywords, (String){.str = lexer->currTokenValue.str, .size = lexer->currTokenValue.size}, &entryValue))
        return (TokenType)entryValue.value;

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
    case ',':
        tokenType = TOK_COMMA;
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

#ifdef _CRACK_DEBUG_
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
