#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "rh4n_utils.h"

void RemoveSpaces(char* source)
{
    char* i = source;
    char* j = source;
    while(*j != 0)
    {
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = 0;
}

char *StripTrailingSpaces(char *str)
{
    char *end;

    printf("Strip Spaces:\n");
    printf("Working with [%s]\n", str);

    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}

int str_isdigit(char *str)
{
    int length = strlen(str);
    int i;

    for(i=0; i < length; i++)
    {
        if(isdigit(str[i]) == 0)
            return(-1);
    }
    return(0);
}

int strn_isdigit(char *str, int length)
{
    int i;

    for(i=0; i < length; i++)
    {
        if(isdigit(str[i]) == 0)
            return(-1);
    }
    return(0);
}

void char_strcat(char *str, char character)
{
    char tmp_str[2];
    tmp_str[0] = character;
    tmp_str[1] = '\0';

    strcat(str, tmp_str);
}

void trimSpaces(char *input)
{
    int length = strlen(input);

    for(; length >= -1; length--)
    {
        if(input[length] != 0x20 && input[length] != 0x00)
        {
            input[length+1] = '\0';
            break;
        }
    }
}

void trimleadingSpaces(char *input)
{
    int length = strlen(input), i;

    for(i=0; i < length; i++)
    {
        if(input[i] != 0x20)
            break;
    }

    length++;

    memmove(input, input+i, length-i);
}

void trimSpaces4B(wchar_t *input)
{
    int byte_length = wcslen(input)*sizeof(wchar_t);

    void *start = (void*)input,
         *end = ((void*)input)+byte_length;
    char c_null = 0x00,
         c_space = 0x20;

    while(end != start)
    {
        if(memcmp(end, &c_null, 1) != 0 && memcmp(end, &c_space, 1) != 0)
            break;
        memset(end, 0x00, 1);
        end--;
    }
}

char *str_toUpper(char *input)
{
    static char output[1024];
    int i = 0;

    memset(output, 0x00, sizeof(output));

    if(strlen(input)+1 > sizeof(output))
    {
        return(NULL);
    }

    for(i=0; i < strlen(input); i++)
    {
        output[i] = toupper(input[i]);
    }
    output[i+1] = '\0';

    return(output);
}

char *str_toLower(char *input)
{
    static char output[1024];
    int i = 0;

    memset(output, 0x00, sizeof(output));

    if(strlen(input)+1 > sizeof(output))
    {
        return(NULL);
    }

    for(i=0; i < strlen(input); i++)
    {
        output[i] = tolower(input[i]);
    }
    output[i+1] = '\0';

    return(output);
  
}

