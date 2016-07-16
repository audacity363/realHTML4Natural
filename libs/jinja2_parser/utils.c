#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/standard.h"
#include "parser.h"
#include "utils.h"
#include "varhandle.h"

int hasFunction(char *word)
{
    char commandbuf[100];
    int i, x =0;
    int length = strlen(word);
    bool found = false;

    if(ncmp(word, "loop"))
        return(false);

    for(i=0;i<length; i++)
    {
        if(word[i] == '.')
            return(true);
    }
    return(false);
}

void getFunction(char *word, char *functionname)
{
    char commandbuf[100];
    int i, x = 0;
    int length = strlen(word);
    bool found = false;

    x=0;

    for(i=0;i<length; i++)
    {
        if(found == true)
        {
            functionname[x] = word[i];
            x++;
        }

        if(word[i] == '.')
        {
            word[i] = '\0';
            found = true;
        }
    }

    functionname[x] = '\0';
    return;
}

void getlength(struct args *word, struct variables *anker)
{
    if(word->type == STRINGARRAY || word->type == INTARRAY)
    {
        word->iargs = getArrayLength(anker, word->cargs, NULL, NULL);
        if(word->iargs == -1)
        {
            fprintf(stderr, "GetLength: Unkown Variable [%s]\n", word->cargs);
        }
    }
    else
    {    
        word->iargs = strlen(word->cargs);
    }
    word->type = INT;
}

void toUpper(struct args *word, struct variables *anker)
{
    int i;
    int length = strlen(word->cargs);
    for(i=0; i<length; i++)
        word->cargs[i] = toupper(word->cargs[i]);
}

void toLower(struct args *word, struct variables *anker)
{
    int i;
    int length = strlen(word->cargs);
    for(i=0; i<length; i++)
        word->cargs[i] = tolower(word->cargs[i]);
}

bool checkInt(char *cmd)
{
    int length = strlen(cmd);
    int i;
    for(i=0; i < length; i++)
    {
        if(cmd[i] < 48 || cmd[i] > 57)
        {
            return(false);
        }
    }
    return(true);
}
