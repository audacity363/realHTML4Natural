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
