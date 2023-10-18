#include"parser.h"

void parser_init(Parser *parser, Lexer *lexer)
{
    parser->lexer = lexer;
}

void parser_error_at_current(Parser *parser, char *errorMessage, ...)
{
    va_list arg;
    va_start(arg, errorMessage);

    perror_at_line(parser->lexer->src.str, parser->lexer->src.size, parser->lexer->currLineStart, parser->lexer->currLineNo, parser->lexer->tokenInfo.column, errorMessage, arg);
}

bool parser_match(Parser *parser, TokenType tokType)
{
    return parser->currTokenType == tokType;
}

bool parser_expect(Parser *parser, TokenType tokType, char *msg)
{
    if (!parser_match(parser, tokType))
    {
        // TODO: Handle the error properly
        parser_error_at_current(parser, "Expected %s but found %s", msg, "Other thing");
        parser->status = STATUS_ERROR;
        return false;
    }
    return true;
}

void parser_advance(Parser *parser)
{
    parser->currTokenType = lexer_next_token(parser->lexer);
}

void parser_ignore_newline(Parser *parser)
{
    while (parser_match(parser, TOK_NEWLINE))
        parser_advance(parser);
}

bool parser_expect_ignoring_newline(Parser *parser, TokenType tokType, char *msg)
{
    parser_ignore_newline(parser);
    return parser_expect(parser, tokType, msg);
}

void parser_parse_variable_prototype(Parser *parser)
{
    if (!parser_expect(parser, TOK_IDENTIFIER, "variable name"))
        return;
    parser_advance(parser);
    if (!parser_expect(parser, TOK_COLON, ": after the variable name"))
        return;
    parser_advance(parser);
    if (!parser_expect(parser, TOK_IDENTIFIER, "variable data type name"))
        return;
    parser_advance(parser);
}

void parser_parse_import(Parser *parser)
{
    log_msg(LOG_INFO, "Parsing import");

    parser_advance(parser);

    if (!parser_expect(parser, TOK_IDENTIFIER, "module name"))
        return;

    parser_advance(parser);

    if (!parser_expect(parser, TOK_NEWLINE, "newline"))
        return;
}
void parser_parse_function_body(Parser *parser)
{
    parser_ignore_newline(parser);
}

void parser_parse_function_parameter(Parser *parser)
{
    while (1)
    {
        parser_ignore_newline(parser);

        if (parser_match(parser, TOK_PAREN_CLOSE))
            break;

        parser_parse_variable_prototype(parser);

        parser_ignore_newline(parser);

        if (!parser_match(parser, TOK_COMMA))
            break;
        parser_advance(parser);
    }
}

void parser_parse_function_declaraction(Parser *parser)
{
    log_msg(LOG_INFO, "Parsing Function Declaration");

    parser_advance(parser);

    if (!parser_expect(parser, TOK_IDENTIFIER, "function name"))
        return;

    parser_advance(parser);

    if (!parser_expect(parser, TOK_PAREN_OPEN, "("))
        return;

    parser_advance(parser);

    parser_parse_function_parameter(parser);

    if (!parser_expect(parser, TOK_PAREN_CLOSE, ")"))
        return;
    parser_advance(parser);

    parser_ignore_newline(parser);

    if (!parser_expect(parser, TOK_CURLY_OPEN, "{"))
        return;

    parser_advance(parser);

    parser_parse_function_body(parser);

    if (!parser_expect(parser, TOK_CURLY_CLOSE, "}"))
        return;
}

void parser_parse_struct_body(Parser *parser)
{
    parser_ignore_newline(parser);

    if (parser_match(parser, TOK_CURLY_CLOSE))
        return;

    while (1)
    {
        parser_parse_variable_prototype(parser);

        parser_ignore_newline(parser);

        if (!parser_match(parser, TOK_COMMA))
            break;
        parser_advance(parser);

        parser_ignore_newline(parser);
    }

    if (!parser_expect(parser, TOK_CURLY_CLOSE, "} or ,"))
        return;
}

void parser_parse_struct_defination(Parser *parser)
{
    log_msg(LOG_INFO, "Parsing Struct Declaration");

    parser_advance(parser);

    if (!parser_expect(parser, TOK_IDENTIFIER, "struct name"))
        return;

    parser_advance(parser);
    parser_expect_ignoring_newline(parser, TOK_CURLY_OPEN, "{");

    parser_advance(parser);

    parser_parse_struct_body(parser);
}

void parser_parse_expression(Parser *parser)
{
    parser_advance(parser);

    switch (parser->currTokenType)
    {
    case TOK_NUMBER:
        break;
    default:
        log_msg(LOG_ERROR, "For now only number expression is supported");
        parser->status = parser->status | STATUS_ERROR;
        break;
    }
    parser_advance(parser);
}

void parser_parse_variable_declaration(Parser *parser)
{
    parser_advance(parser);

    parser_parse_variable_prototype(parser);

    if (parser_match(parser, TOK_NEWLINE))
    {
        log_msg(LOG_ERROR, "Initial Value is required while declaring variable");
        parser->status = parser->status | STATUS_ERROR;
        return;
    }
    if (!parser_expect(parser, TOK_EQUAL, "="))
        return;

    parser_parse_expression(parser);

    if (!parser_expect(parser, TOK_NEWLINE, "newline after the expression"))
        return;
}

void parser_parse(Parser *parser)
{
    parser_advance(parser);
    while (!parser_match(parser, TOK_EOF))
    {
        switch (parser->currTokenType)
        {
        case TOK_IMPORT:
            parser_parse_import(parser);
            break;
        case TOK_FN:
            parser_parse_function_declaraction(parser);
            break;
        case TOK_STRUCT:
            parser_parse_struct_defination(parser);
            break;
        case TOK_LET:
            log_msg(LOG_ERROR, "Sorry we don't support global variable!");
            parser_parse_variable_declaration(parser);
            break;
        case TOK_NEWLINE:
            // Ignore the newline
            break;
        case TOK_EOF:
            break;
        default:
            // log_msg(LOG_ERROR, "Unknown Error! Syntax error");
            parser_error_at_current(parser, "Unknown Error! Syntax error");
        }
        parser_advance(parser);
    }
}