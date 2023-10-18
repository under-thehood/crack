#include "../src/compiler_frontend/cli_handler.h"
#include "../src/compiler_frontend/parser.h"

void test_parser(Config *config)
{
    String fileContent;
    bool status = file_read(config->fileName, &fileContent);
    if (status == true)
    {
        Lexer lexer;
        lexer_init(&lexer, fileContent);
        Parser parser;
        parser_init(&parser, &lexer);
        parser_parse(&parser);
    }
}

int main(int argc, char const *argv[])
{
    Config config = command_argument_parse(argc, argv);
    test_parser(&config);
    return 0;
}
