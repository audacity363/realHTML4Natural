#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vars.h"
#include "lda.h"

int startLDAParser(char *filename, vars_t *anker, bool debug, FILE *logfile)
{
    FILE *lda = NULL;
    char line[2048];
    int ret = 0;

    if((lda = fopen(filename, "r")) == NULL)
    {
        if(debug == true)
            fprintf(logfile, "%s\n", strerror(errno));
        return(LDA_FILE_ERROR);
    }

    anker->level = 1;

    while(!feof(lda))
    {
        fgets(line, sizeof(line), lda);
        if(strlen(line) == 0)
            break;
        if((ret = unix_lda(line, anker, debug, logfile)) != LDA_OK && ret != CONTINUE)
            return(ret);
        memset(line, 0x00, sizeof(line));
    }

    fclose(lda);
    return(LDA_OK);
}

void printTabs(int level, FILE *logfile)
{
    int i;
    for(i=0; i < level-1; i++)
    {
        fprintf(logfile, "\t");
    }

}

void printVarType(int type, FILE *logfile)
{
    switch(type)
    {
        case STRING:
            fprintf(logfile, "Type:     [String]\n");
            break;
        case ONEDSTRING:
            fprintf(logfile, "Type:     [1D String]\n");
            break;
        case TWODSTRING:
            fprintf(logfile, "Type:     [2D String]\n");
            break;
        case THREEDSTRING:
            fprintf(logfile, "Type:     [3D String]\n");
            break;
        case INTEGER:
            fprintf(logfile, "Type:     [int]\n");
            break;
        case ONEDINTEGER:
            fprintf(logfile, "Type:     [1D int]\n");
            break;
        case TWODINTEGER:
            fprintf(logfile, "Type:     [2D int]\n");
            break;
        case THREEDINTEGER:
            fprintf(logfile, "Type:     [3D int]\n");
            break;
        case FLOAT:
            fprintf(logfile, "Type:     [float]\n");
            break;
        case ONEDFLOAT:
            fprintf(logfile, "Type:     [1D float]\n");
            break;
        case TWODFLOAT:
            fprintf(logfile, "Type:     [2D float]\n");
            break;
        case THREEDFLOAT:
            fprintf(logfile, "Type:     [2D float]\n");
            break;
        case BOOL:
            fprintf(logfile, "Type:     [bool]\n");
            break;
        case ONEDBOOL:
            fprintf(logfile, "Type:     [1D bool]\n");
            break;
        case TWODBOOL:
            fprintf(logfile, "Type:     [2D bool]\n");
            break;
        case THREEDBOOL:
            fprintf(logfile, "Type:     [3D bool]\n");
            break;
        case GROUP:
            fprintf(logfile, "Type:     [Group leader]\n");
            break;
        case REDEFINE:
            fprintf(logfile, "Type:     [Redefine]\n");
            break;
        case UNKOWN:
            fprintf(logfile, "Type:     [Unkown]\n");
            break;
    }
}

void printfork(vars_t *anker, FILE *logfile)
{
    vars_t *hptr;
    int is_array = 0;
    
    hptr = anker;
    while(hptr != NULL)
    {
        printTabs(hptr->level, logfile);
        fprintf(logfile, "Level:    [%d]\n", hptr->level);
        printTabs(hptr->level, logfile);
        fprintf(logfile, "Name:     [%s]\n", hptr->name);
        printTabs(hptr->level, logfile);
        printVarType(hptr->type, logfile);
        if(hptr->x_length != -1)
        {
            printTabs(hptr->level, logfile);
            fprintf(logfile, "X Length:    [%d]\n", hptr->x_length);
        }
        if(hptr->y_length != -1)
        {
            printTabs(hptr->level, logfile);
            fprintf(logfile, "Y Length:    [%d]\n", hptr->y_length);
        }
        if(hptr->z_length != -1)
        {
            printTabs(hptr->level, logfile);
            fprintf(logfile, "Z Length:    [%d]\n", hptr->z_length);
        }


        if(hptr->next_lvl != NULL)
            printfork(hptr->next_lvl, logfile);

        fprintf(logfile, "\n");
        hptr = hptr->next;
    }
}
