#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <errno.h>

#include "../include/standard.h"

#define INT 1
#define STRING 2
#define STRINGARRAY 3
#define TWO_DSTRINGARRAY 5
#define INTARRAY 4
#define TWO_DINTARRAY 6
#define GROUP 7
#define REDEFINE 8

struct varnames {
    char name[200];
    int level;
    int type;
    char rename[200];
    int length; //Laenge derVariable 
    int x_length; // X Laenge des Arrays
    int y_length; // Y Laenge des Arrays (Nur bei 2D)
    struct varnames *nextlevel;
    struct varnames *next;
    struct varnames *prev;
};

struct status_p {
    int akt_level;
    bool foundpage_grp;
    struct varnames *cur_dataset;
};

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

char *printVarType(int type)
{
    char *returnval = malloc(17);

    switch(type)
    {
        case INT:
            strcpy(returnval, "Int");
            break;
        case STRING:
            strcpy(returnval, "String");
            break;
        case STRINGARRAY:
            strcpy(returnval, "String Array");
            break;
        case TWO_DSTRINGARRAY:
            strcpy(returnval, "String 2D Array");
            break;
        case INTARRAY:
            strcpy(returnval, "Int Array");
            break;
        case TWO_DINTARRAY:
            strcpy(returnval, "Int 2D Array");
            break;
        case GROUP:
            strcpy(returnval, "Group Leader");
            break;
        case REDEFINE:
            strcpy(returnval, "Redefine");
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

void printfork(struct varnames *ptr)
{
    struct varnames *hptr;
    
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


        if(hptr->type == STRINGARRAY || hptr->type == INTARRAY)
        {
            printTabs(hptr->level);
            printf("X Length: [%d]\n", hptr->x_length);
        }
        else if(hptr->type == TWO_DSTRINGARRAY || hptr->type == TWO_DINTARRAY)
        {
            printTabs(hptr->level);
            printf("X Length: [%d]\n", hptr->x_length);
            printTabs(hptr->level);
            printf("Y Length: [%d]\n", hptr->y_length);
        }
        else if(hptr->type == REDEFINE)
        {
            printTabs(hptr->level);
            printf("new name: [%s]\n", hptr->rename);
        }
        printf("\n");

        if(hptr->nextlevel != NULL)
            printfork(hptr->nextlevel);

        hptr = hptr->next;
    }
}

void printall(struct varnames *anker)
{
    int index = 0;
    struct varnames *ptr, *hptr;

    ptr = anker;

    while(ptr != NULL)
    {
        hptr = ptr->nextlevel;
        while(hptr != NULL)
        {

        }

        ptr = ptr->next;
    }

    
}

void remove_tailing_spaces(char *buffer)
{
    int i;
    for(i=0; i < strlen(buffer); i++)
    {
        if(buffer[i] != ' ')
        {
            break;
        }
    }

    memcpy(buffer, buffer+i, strlen(buffer));
}

void striptrailspace( char *line )
{
    int walker = strlen(line);
    walker = strspn ( line, " \t\n\v" );
    memmove ( line, line + walker, strlen ( line + walker ) + 1 );
}

bool checkifArray(char *line, char *result)
{
    int length = strlen(line);
    int i, x;
    bool found = false;

    x = 0;

    for(i=0; i < length; i++)
    {
        if(found == true)
        {
            result[x] = line[i];
            x++;
        }
        if(line[i] == '/')
        {
            line[i] = '\0';
            found = true;
        }
    }

    result[x] = '\0';

    return(found);
}

char *strtoupper(char *str)
{
    char *cpy = malloc(strlen(str));
    int i;

    strcpy(cpy, str);

    for(i=0; i < strlen(cpy); i++)
    {
        cpy[i] = toupper(cpy[i]);
    }

    return(cpy);
}

void parser(struct varnames *anker, struct status_p *status, char *line)
{
    int x, i = 0;
    int length = strlen(line);
    int x_length = 0;
    int y_length = 0;
    int varlength = 0;

    char levelbuff[4];
    char varname[100];
    char varinfos[50];
    char arrayresult[50];
    char varlengthbuff[4];
    char redefinevar [100];
    char *sptr;
    int vartype;

    struct varnames *hptr, *ptr, *hhptr;


    if(length == 0)
        return;

    if(line[length-1] == '\n')
        line[length-1] = '\0';

    while(line[i] != ' ' && i < length && i < sizeof(levelbuff)-1)
    {
        levelbuff[i] = line[i];
        i++;
    }
    levelbuff[i] = '\0';
    
    memmove(line, line+(++i), length);
    length = strlen(line);

    i = 0;
    while(line[i] != ' ' && i < length && i < sizeof(varname)-1)
    {
        varname[i] = line[i];
        i++;
    }
    varname[i] = '\0';

    memmove(line, line+(++i), length);
    length = strlen(line);

    if(cmp(strtoupper(varname), "REDEFINE"))
    {
        //redefine gefunden
        strcpy(redefinevar, line);
        vartype = REDEFINE;
        goto save;
    }

    //Ende der Variable erreicht. Keine naehere Informationen gefunden. 
    //Also ist es ein Gruppenleader
    if(strlen(line) == 0)
    {
        vartype = GROUP;
        if(cmp(varname, "page"))
            status->foundpage_grp = true;
        else if(levelbuff[0] == '1')
            status->foundpage_grp = false;

        goto save;
    }
   
    //Klammer auf am Anfang der Zeile endfernen
    memmove(line, line+1, length);
    i = 0;
    while(line[i] != ')' && i < length && i < sizeof(varinfos)-1)
    {
        varinfos[i] = line[i];
        i++;
    }

    varinfos[i] = '\0';

    x = 0;
    for(i=1; i < strlen(varinfos); i++)
    {
        if(varinfos[i] == '/')
            break;
        varlengthbuff[x] = varinfos[i];
        x++;
    }

    varlengthbuff[x] = '\0';
    varlength = atoi(varlengthbuff);

    switch(tolower(varinfos[0]))
    {
        case 'a':
            if(checkifArray(varinfos, arrayresult) == true)
            {
                vartype = STRINGARRAY;
                sptr = strtok(arrayresult, ",");
                for(i=0; i < strlen(sptr); i++)
                {
                    if(sptr[i] == ':')
                    {
                        break;
                    }
                }
                i++;

                memmove(sptr, sptr+i, strlen(sptr));
                x_length = atoi(sptr);

                sptr = strtok(NULL, ",");
                if(sptr != NULL)
                {
                    //Zweite Dimension gefunden
                    vartype = TWO_DSTRINGARRAY;
                    for(i=0; i < strlen(sptr); i++)
                    {
                        if(sptr[i] == ':')
                        {
                            break;
                        }
                    }
                    i++;

                    memmove(sptr, sptr+i, strlen(sptr));
                    y_length = atoi(sptr);
                }
            }
            else
            {
                vartype = STRING;    
            }
            break;
        case 'i':
            vartype = INT;
            break;
        case 'n':
            vartype = INT;
            break;
        case 'l':
            //Boolean
            vartype = INT;
            break;
        default:
            fprintf(stderr, "Dont know [%c] as variable type\n", varinfos[0]);
            return;
    }

save:
    if(status->foundpage_grp == false)
        return;
    hptr = malloc(sizeof(struct varnames));
    if(hptr == NULL)
    {
        fprintf(stderr, "Can not allocate memory for new entry [%s]\n", 
                strerror(errno));
        return;
    }

    hptr->level = atoi(levelbuff);
    strcpy(hptr->name, varname);
    hptr->type = vartype;
    hptr->length = varlength;
    hptr->x_length = x_length;
    hptr->y_length = y_length;

    if(vartype == REDEFINE)
    {
        strcpy(hptr->rename, redefinevar);
    }

    hptr->nextlevel = NULL;
    hptr->next = NULL;
    hptr->prev = NULL;

    if(status->cur_dataset->level == hptr->level)
    {
        hptr->prev = status->cur_dataset;
        status->cur_dataset->next = hptr;
        status->cur_dataset = hptr;
    }
    else if(status->cur_dataset->level > hptr->level)
    {
        hhptr = status->cur_dataset;
        while(hhptr != NULL)
        {
            if(hhptr->level == hptr->level)
                break;
            hhptr = hhptr->prev;
        }
        if(hhptr == NULL)
        {
            fprintf(stderr, "Level nicht gefunden\n");
            return;
        }

        while(hhptr->next != NULL)
        {
            hhptr = hhptr->next;
        }

        hptr->prev = hhptr;
        hhptr->next = hptr;
        status->cur_dataset = hptr;
    }
    else if(status->cur_dataset->level < hptr->level)
    {
        hptr->prev = status->cur_dataset;
        status->cur_dataset->nextlevel = hptr;
        status->cur_dataset = hptr;
    }
}

int main()
{
    char *ldaname = "./testlda";
    FILE *lda;
    char linebuf[2024];

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

    if((lda = fopen(ldaname, "r")) == NULL)
    {
        fprintf(stderr, "Can not open File [%s]\n", ldaname);
        fprintf(stderr, "%s\n", strerror(errno));
        exit(-1);
    }

    while(!feof(lda))
    {
        fgets(linebuf, sizeof(linebuf), lda);
        striptrailspace(linebuf);
        parser(anker, &parser_status, linebuf);
        bzero(linebuf, sizeof(linebuf));
    }

printf("------------------------------------------------------------------\n");
    printfork(anker);
printf("------------------------------------------------------------------\n");
}
