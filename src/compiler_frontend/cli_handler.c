#include"cli_handler.h"


void display_usage(const char *exeName)
{
    // for starting only filename is taken
    fprintf(stderr, "Usage : %s <filename>\n\n", exeName);
}



Config command_argument_parse(int argc, char const *argv[])
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