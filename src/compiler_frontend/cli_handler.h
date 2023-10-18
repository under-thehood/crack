#ifndef _CRACK_CLI_HANDLER_H_
#define _CRACK_CLI_HANDLER_H_

#include <stdio.h>
#include <stdlib.h>
#include "../error.h"

// define the configuration of current build
typedef struct
{
    char const *fileName;
} Config;

void display_usage(const char *exeName);

Config command_argument_parse(int argc, char const *argv[]);

#endif
