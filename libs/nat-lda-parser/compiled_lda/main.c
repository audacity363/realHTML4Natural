#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "lda_parser.h"

#define SAG_NATONE "/*"
#define SAG_UNIX   "**"

void striptrailspace( char *line )
{
    int walker = strlen(line);
    walker = strspn ( line, " \t\n\v" );
    memmove ( line, line + walker, strlen ( line + walker ) + 1 );
}

void removespaces(char *line)
{
    char *space = index(line, ' ');
    if(space != NULL)
        space[0] = '\0';
}

char *printVarType(int type)
{
    char *returnval = malloc(17);

    switch(type)
    {
        case INTEGER:
            strcpy(returnval, "Int");
            break;
        case ALPHA:
            strcpy(returnval, "String");
            break;
        case GROUP_LEADER:
            strcpy(returnval, "Group Leader");
            break;
        case REDEFINE:
            strcpy(returnval, "Redefine");
            break;
        case NUMERIC:
            strcpy(returnval, "Numeric");
            break;
    }
    return(returnval);
}

void printTabs(int level)
{
    int i;
    for(i=0; i < level-1; i++)
    {
        printf("\t");
    }
}

void printSingleVar(struct varnames *hptr)
{
    int is_array = 0;

    printTabs(hptr->level);
    printf("Level:    [%d]\n", hptr->level);
    printTabs(hptr->level);
    printf("Type:     [%s]\n", printVarType(hptr->type));


    printTabs(hptr->level);
    printf("Name:     [%s]\n", hptr->name);
    printTabs(hptr->level);
    printf("Length:   [%d]\n", hptr->length);
    printTabs(hptr->level);
    printf("comma:    [%d]\n", hptr->length_comma);


    if(hptr->array == ARRAY_1D)
    {
        printTabs(hptr->level);
        printf("Array Dim:[1]\n");
        printTabs(hptr->level);
        printf("X Length: [%d]\n", hptr->x_length);
        is_array = 1;
    }
    else if(hptr->array == ARRAY_2D)
    {
        is_array = 1;
        printTabs(hptr->level);
        printf("Array Dim:[2]\n");
        printTabs(hptr->level);
        printf("X Length: [%d]\n", hptr->x_length);
        printTabs(hptr->level);
        printf("Y Length: [%d]\n", hptr->y_length);
    }
    else
    {
        printTabs(hptr->level);
        printf("Array Dim:[0]\n");
    }

    printf("\n");
}

void printfork(struct varnames *ptr)
{
    struct varnames *hptr;
    int is_array = 0;
    
    hptr = ptr;
    while(hptr != NULL)
    {
        printTabs(hptr->level);
        printf("Level:    [%d]\n", hptr->level);
        printTabs(hptr->level);
        printf("Type:     [%s]\n", printVarType(hptr->type));


        printTabs(hptr->level);
        printf("Name:     [%s]\n", hptr->name);
        printTabs(hptr->level);
        printf("Length:   [%d]\n", hptr->length);
        printTabs(hptr->level);
        printf("comma:    [%d]\n", hptr->length_comma);


        if(hptr->array == ARRAY_1D)
        {
            printTabs(hptr->level);
            printf("Array Dim:[1]\n");
            printTabs(hptr->level);
            printf("X Length: [%d]\n", hptr->x_length);
            is_array = 1;
        }
        else if(hptr->array == ARRAY_2D)
        {
            is_array = 1;
            printTabs(hptr->level);
            printf("Array Dim:[2]\n");
            printTabs(hptr->level);
            printf("X Length: [%d]\n", hptr->x_length);
            printTabs(hptr->level);
            printf("Y Length: [%d]\n", hptr->y_length);
        }
        else
        {
            printTabs(hptr->level);
            printf("Array Dim:[0]\n");
        }

        printf("\n");

        if(hptr->nextlevel != NULL)
            printfork(hptr->nextlevel);

        hptr = hptr->next;
    }
}

int initAnker(struct varnames **anker)
{
    *anker = malloc(sizeof(struct varnames));
    if(*anker == NULL)
    {
        fprintf(stderr, "Can not allocate Memory for anker [%s]\n",
            strerror(errno));
        return(-1);
    }

    return(0);
}

struct varnames* getVarNames(char *ldaname, char *error_str)
{
    char linebuf[1024];
    FILE *ldafile;
    void (*parser)(struct varnames*, struct status_p*, char*);

    struct varnames *anker;
    struct status_p parser_status;

    parser_status.foundpage_grp = false;
    if(initAnker(&anker) < 0)
    {
        exit(-1);
    }

    strcpy(anker->name, "anker");
    anker->level = 1;
    anker->nextlevel = NULL;
    anker->next = NULL;
    anker->prev = NULL;

    parser_status.cur_dataset = anker;


    if((ldafile = fopen(ldaname, "r")) == NULL)
    {
        sprintf(error_str, "Error reading LDA [%s]: [%s]\n", ldaname,
            strerror(errno));
        return(NULL);
    }

    fgets(linebuf, sizeof(linebuf), ldafile);

    if(strncmp(linebuf+4, SAG_NATONE, 2) == 0)
    {
        parser = &natone_parser;      
    }
    else if(strncmp(linebuf+4, SAG_UNIX, 2) == 0)
    {
        parser = &unix_parser;      
    }
    else
    {
        sprintf(error_str, "Error parsing LDA [%s]: Unkown LDA Type", ldaname);
        fclose(ldafile);
        return(NULL);
    }

    rewind(ldafile);

    while(!feof(ldafile))
    {
        fgets(linebuf, sizeof(linebuf), ldafile);
        striptrailspace(linebuf);
        parser(anker, &parser_status, linebuf);
        bzero(linebuf, sizeof(linebuf));
    }

    return(anker);
}
