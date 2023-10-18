#include "../src/compiler_frontend/cli_handler.h"
#include "../src/compiler_frontend/lexer.h"

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

int main(int argc, char const *argv[])
{
    Config config = command_argument_parse(argc, argv);
    test_lexer(&config);
    return 0;
}
