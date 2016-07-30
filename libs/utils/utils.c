#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    // Write new null terminator
    *(end+1) = 0;

    return str;
}

void TrimSpaces(char *str)
{
    int length = strlen(str), i;
    for(i=0; i < length; i++)
    {
        if(!isspace(str[i]))
            break;
    }

    memmove(str, str+i, length);

}

char* getLineFromFile(char *filename, int line)
{
    FILE *file;
    char *linebuff = malloc(1024);
    int count = 1;

    if((file = fopen(filename, "r")) == NULL)
    {
        return(NULL);
    }

    while(fgets(linebuff, 1024, file) != NULL)
    {
        if(count >= line)
            break;
        count++;
    }
    fclose(file);

    linebuff[strlen(linebuff)-1] = '\0';

    return(linebuff);
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

void char_strcat(char *str, char character)
{
    char tmp_str[2];
    tmp_str[0] = character;
    tmp_str[1] = '\0';

    strcat(str, tmp_str);
}
