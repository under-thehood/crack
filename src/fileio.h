#ifndef _CRACK_FILEIO_H_
#define _CRACK_FILEIO_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "error.h"
#include "string.h"

typedef struct
{
    const char *fileName;
    size_t fileSize;
    FILE *fp;
} FileInfo;

FileInfo file_status(const char *fileName);

bool file_read(const char *fileName, String *string);

#endif