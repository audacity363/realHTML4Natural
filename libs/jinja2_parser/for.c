#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/standard.h"
#include "jinja2_parser.h"
#include "utils.h"

struct for_infos {
    int index;
    int first;
    int second;
};

int forhandle(struct variables *anker, char *data)
{
//    printf("\n-----------------------------------------------------------------\n");
    char *forcmd, *first, *vergleich, *second, *tmp;
    char *datacpy;
    void *entrys;
    int i, type, length, index, x, y;
    int x_length, y_length;
    struct for_infos status;

    datacpy = malloc(sizeof(char)*strlen(data));
    strcpy(datacpy, data);

    forcmd = strtok(data, "\n");
    //Anfang des Befehls suchen
    for(i=0; i < strlen(forcmd); i++)
    {
        if(forcmd[i] == 'f')
            break;
    }
    memmove(forcmd, forcmd+i, strlen(forcmd));

    //Ende suchen
    for(i=0; i < strlen(forcmd); i++)
    {
        if(forcmd[i] == '%')
            forcmd[i] = '\0';
    }

    //Erstes for herausfiltern
    first = strtok(forcmd, " ");
    first = strtok(NULL, " ");
    vergleich = strtok(NULL, " ");
    second = strtok(NULL, " ");

    if(first == NULL || vergleich == NULL || second == NULL)
    {
        fprintf(stderr, "FOR: Syntax Error\n");
        return(-1);
    }

    if(strcmp(vergleich, "in") != 0)
    {
        fprintf(stderr, "For: Kenne ich nicht\n");
        return(-1);
    }

    type = getVarType(anker, second);
    if(type != STRINGARRAY && type != INTARRAY &&
       type != TWO_DSTRINGARRAY && type != TWO_DINTARRAY)
    {
        fprintf(stderr, "For: Wrong Type\n");
        return(-1);
    }

    length = getArrayLength(anker, second, &x_length, &y_length);
    if(length == -1)
    {
        fprintf(stderr, "Variable not found\n");
        return(-1);
    }

    entrys = malloc(sizeof(char*));
    tmp = strtok(datacpy, "\n");
    tmp = strtok(NULL, "\n");
    if(tmp == NULL)
    {
        fprintf(stderr, "For: Empty for block\n");
        return(-1);
    }

    index = 1;
    while(tmp != NULL)
    {
        ((char**)entrys)[index-1] = malloc(strlen(tmp));
        strcpy(((char**)entrys)[index-1], tmp);
        strcat(((char**)entrys)[index-1], "\n");
        entrys = realloc(entrys, sizeof(char*)*index+1);
        index++;
        tmp = strtok(NULL, "\n");
    }

    newIntVar(anker, "loop.i", 0);
    switch(type)
    {
        case STRINGARRAY:
            newStringVar(anker, first, " ");
            break;
        case INTARRAY:
            newIntVar(anker, first, 0);
            break;
        case TWO_DSTRINGARRAY:
            newStringArray(anker, first, " ");
            for(y=0; y < y_length; y++)
            {
                appendStringArray(anker, first,
                 getStringValuefrom2DArray(anker, second, 0, y));
            }
            length = x_length;
            break;
        case TWO_DINTARRAY:
            newIntArray(anker, first, 0);
            for(y=0; y < y_length; y++)
            {
                appendIntArray(anker, first,
                    getIntValuefrom2DArray(anker, second, 0, y));
            }
            length = x_length;
            break;
        default:
            fprintf(stderr, "For: Wrong Type\n");
            return(-1);
    }


//    printf("value [%s]\n", getStringValuefromArray(anker, second, 0));
    for(status.index=0; status.index < length; status.index++)
    {

        editIntVar(anker, "loop.i", status.index);
        switch(type)
        {
            case STRINGARRAY:
                editStringVar(anker, first, 
                    getStringValuefromArray(anker, second, status.index));
                break;
            case INTARRAY:
                editIntVar(anker, first, 
                    getIntValuefromArray(anker, second, status.index));
                break;
            case TWO_DSTRINGARRAY:
                for(y=0; y < y_length; y++)
                {
                    editStringVarArray(anker, first, 
                     getStringValuefrom2DArray(anker, second, status.index, y),
                    y);
                }       
                break;
            case TWO_DINTARRAY:
                for(y=0; y < y_length; y++)
                {
                    editIntVarArray(anker, first, 
                      getIntValuefrom2DArray(anker, second, status.index, y),
                      y);
                }
                break;
            default:
                fprintf(stderr, "For: Unknown Type\n");
                break;
        }

        parser_info parser_status = {false, false, false, false, false, -1, 
                                     "", 0, 0, malloc(sizeof(char))};
        for(x=0; x < index; x++)
        {
            if(((char**)entrys)[x] == 0x0)
                break;
            int linelen  = strlen(((char**)entrys)[x]);
            char *curline = ((char**)entrys)[x];
            local_jinja_parser(&parser_status, anker, curline, linelen);
        }
    }

//    deleteVar(anker, second);
//    deleteVar(anker, "loop.i");

//    printf("\n-----------------------------------------------------------------\n");
    free(datacpy);
}
