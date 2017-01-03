#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vars.h"
#include "lda.h"

void start(char *filename, vars_t *anker)
{
    FILE *lda = NULL;
    char line[2048];

    if((lda = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }

    anker->level = 1;


    while(!feof(lda))
    {
        fgets(line, sizeof(line), lda);
        unix_lda(line, anker);
    }

    printfork(anker);
    fclose(lda);
}

void printTabs(int level)
{
    int i;
    for(i=0; i < level-1; i++)
    {
        printf("\t");
    }

}

void printVarType(int type)
{
    switch(type)
    {
        case STRING:
            printf("Type:     [String]\n");
            break;
        case ONEDSTRING:
            printf("Type:     [1D String]\n");
            break;
        case TWODSTRING:
            printf("Type:     [2D String]\n");
            break;
        case THREEDSTRING:
            printf("Type:     [3D String]\n");
            break;
        case INTEGER:
            printf("Type:     [int]\n");
            break;
        case ONEDINTEGER:
            printf("Type:     [1D int]\n");
            break;
        case TWODINTEGER:
            printf("Type:     [2D int]\n");
            break;
        case THREEDINTEGER:
            printf("Type:     [3D int]\n");
            break;
        case FLOAT:
            printf("Type:     [float]\n");
            break;
        case ONEDFLOAT:
            printf("Type:     [1D float]\n");
            break;
        case TWODFLOAT:
            printf("Type:     [2D float]\n");
            break;
        case THREEDFLOAT:
            printf("Type:     [2D float]\n");
            break;
        case BOOL:
            printf("Type:     [bool]\n");
            break;
        case ONEDBOOL:
            printf("Type:     [1D bool]\n");
            break;
        case TWODBOOL:
            printf("Type:     [2D bool]\n");
            break;
        case THREEDBOOL:
            printf("Type:     [3D bool]\n");
            break;
        case GROUP:
            printf("Type:     [Group leader]\n");
            break;
    }
}

void printfork(vars_t *anker)
{
    vars_t *hptr;
    int is_array = 0;
    
    hptr = anker;
    while(hptr != NULL)
    {
        printTabs(hptr->level);
        printf("Level:    [%d]\n", hptr->level);
        printTabs(hptr->level);
        printf("Name:     [%s]\n", hptr->name);
        printTabs(hptr->level);
        printVarType(hptr->type);
        if(hptr->x_length != -1)
        {
            printTabs(hptr->level);
            printf("X Length:    [%d]\n", hptr->x_length);
        }
        if(hptr->y_length != -1)
        {
            printTabs(hptr->level);
            printf("Y Length:    [%d]\n", hptr->y_length);
        }
        if(hptr->z_length != -1)
        {
            printTabs(hptr->level);
            printf("Z Length:    [%d]\n", hptr->z_length);
        }


        if(hptr->next_lvl != NULL)
            printfork(hptr->next_lvl);

        printf("\n");
        hptr = hptr->next;
    }
}
