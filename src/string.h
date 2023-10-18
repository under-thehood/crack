#ifndef _CRACK_STRING_H_
#define _CRACK_STRING_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char *str;
    size_t size;
} String;

typedef struct
{
    char *curr;
    char *end;
} StringIter;

StringIter string_iter_create(String *string, size_t index);

char string_iter_current(StringIter *iter);

void string_iter_advance(StringIter *iter);

String string_create(char *str);



String string_substring(String *src, size_t startIndex, size_t length);

void string_display(String *string);

bool is_alpha(char character);

bool is_digit(char character);

bool is_alpha_digit(char character);

#endif