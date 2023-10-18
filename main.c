#include "src/compiler_frontend/parser.h"
#include "src/compiler_frontend/cli_handler.h"
#include"src/fileio.h"


//=========================Main Section======================
int main(int argc, char const *argv[])
{
    Config config = command_argument_parse(argc, argv);
    String fileContent;
    bool status = file_read(config.fileName, &fileContent);
    if (status == true)
    {
        Lexer lexer;
        lexer_init(&lexer, fileContent);
        Parser parser;
        parser_init(&parser, &lexer);
        parser_parse(&parser);
    }
    return 0;
}
