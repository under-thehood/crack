#include "fileio.h"


FileInfo file_status(const char *fileName)
{
    FileInfo fInfo = (FileInfo){.fileName = fileName, .fileSize = 0};
    fInfo.fp = fopen(fileName, "r");
    if (fInfo.fp == NULL)
        return fInfo;

    // store previous position
    size_t prevPos = ftell(fInfo.fp);

    fseek(fInfo.fp, 0, SEEK_END);
    fInfo.fileSize = ftell(fInfo.fp);
    // restore previous state
    fseek(fInfo.fp, prevPos, SEEK_SET);

    return fInfo;
}
bool file_read(const char *fileName, String *string)
{
    FileInfo fileInfo = file_status(fileName);
    if (fileInfo.fp == NULL)
    {
        log_msg(LOG_ERROR, strerror(errno));
        return false;
    }

    char *buffer = (char *)malloc(sizeof(char) * fileInfo.fileSize);

    if (buffer == NULL)
    {
        log_msg(LOG_ERROR, "Cannot allocate the enough memory for file");
        return false;
    }
    // TODE: Handle file read error
    fread(buffer, sizeof(char), fileInfo.fileSize, fileInfo.fp);

    string->size = fileInfo.fileSize;
    string->str = buffer;
}
