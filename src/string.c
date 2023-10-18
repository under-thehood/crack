#include "string.h"

StringIter string_iter_create(String *string, size_t index)
{
    return (StringIter){.curr = string->str + (index % string->size), .end = string->str + string->size};
}
char string_iter_current(StringIter *iter)
{
    return *iter->curr;
}
void string_iter_advance(StringIter *iter)
{
    if (iter->curr + 1 <= iter->end)
        iter->curr++;
}

String string_create(char *str)
{
    size_t strLen = strlen(str);
    return (String){.str = str, .size = strLen};
}

String string_substring(String *src, size_t startIndex, size_t length)
{
    return (String){.str = src->str + (startIndex % src->size), .size = startIndex + length < src->size ? length : src->size - startIndex};
}
void string_display(String *string)
{
    char *start = string->str;
    for (size_t i = 0; i < string->size; i++)
    {
        putchar(start[i]);
    }
}

bool is_alpha(char character)
{
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');
}
bool is_digit(char character)
{
    return (character >= '0' && character <= '9');
}

bool is_alpha_digit(char character)
{
    return is_alpha(character) || is_digit(character);
}
