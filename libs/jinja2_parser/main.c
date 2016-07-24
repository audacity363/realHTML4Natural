#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "jinja2_parser.h"
#include "jinja_utils.h"
#include "if.h"

char error_str[2048];

void callFunction(struct args *word, struct variables *anker, char *functionname, int vartype)
{
    struct functions lookuptable[] = {{"len()", {STRING, STRINGARRAY, INTARRAY}, &getlength},
                                      {"toupper()", {STRING}, &toUpper},
                                      {"tolower()", {STRING}, &toLower}};
    struct functions *tlp;
    const int tablelength = sizeof(lookuptable)/sizeof(*lookuptable);
    int i;

    for(i=0; i <tablelength; i++)
    {
        tlp = &lookuptable[i];
        if(cmp(tlp->name, functionname))
        {
            if(tlp->vartypes[0] == vartype ||
               tlp->vartypes[1] == vartype ||
               tlp->vartypes[2] == vartype ||
               tlp->vartypes[3] == vartype)
            {
                    tlp->fncPnt(word, anker);
                    return;
            }
            else
                fprintf(stderr, "Call Function: Not Supported [%s]\n", functionname);
        }
    }
}

int importHandler(struct variables *anker, char *statement)
{
    char *filename;
    int ret;
    FILE *importfile;
    char linebuf[2048];
    char *statecpy = malloc(strlen(statement));

    strcpy(statecpy, statement);

    filename = strtok(statement, " ");
    filename = strtok(NULL, " ");
    if(filename == NULL)
        return(-1);

    if((filename[0] == '"' && filename[strlen(filename)-1] != '"') ||
       (filename[0] != '"' && filename[strlen(filename)-1] == '"') ||
       (filename[0] != '"' && filename[strlen(filename)-1] != '"'))
    {
        fprintf(stderr, "Import Syntax Error [%s]\n", statecpy);
        return(-1);
    }

    filename[strlen(filename)-1] = '\0';

    memmove(filename, filename+1, strlen(filename));

    if((ret = access(filename, R_OK)) != 0)
    {
        switch(ret)
        {
            default:
                fprintf(stderr, "Import: Unknown Error\n");
                return(-1);
        }
    }
    
    if((importfile = fopen(filename, "r")) == NULL)
    {
            fprintf(stderr, "Import: Error while opening file\n");
            return(-1);
    }

    while(!feof(importfile))
    {
        fgets(linebuf, sizeof(linebuf), importfile);
        printf("%s", linebuf);
        bzero(linebuf, sizeof(linebuf));
    }
    fclose(importfile);
}

void fillTestValues(struct variables *anker)
{
    int x, y, i;
    char teststr[MAXSTRINGLENGTH];

    anker = malloc(sizeof(struct variables));
    strcpy(anker->name, "page_test");
    anker->data = malloc(sizeof(int));
    *(int*)anker->data = 15;
    anker->type = INT;
    anker->next = NULL;

    newStringArray(anker, "arraytest", "123");
    appendStringArray(anker, "arraytest", "jojojo");
    appendStringArray(anker, "arraytest", "52");

    newIntArray(anker, "Intarray", 52);
    appendIntArray(anker, "Intarray", 878);

    newStringVar(anker, "lentest", "hello world");

    new2DStringArray(anker, "2dtest", 5, 5);
    for(x=0; x < 5; x++)
    {
        for(y=0; y < 5; y++)
        {
            sprintf(teststr, "Hello World x[%d] y[%d]", x, y);
            editStringVar2DArray(anker, "2dtest", teststr, x, y);
        }
    }


    struct variables *testvar;
    testvar = createTmpArrayOut2DArray(anker, "2dtest", 1);
    printArrayfromPtr(testvar);
    free(testvar->data);
    free(testvar);

    new2DIntArray(anker, "int2dtest", 5, 5);
    i=0;
    for(x=0; x < 5; x++)
    {
        for(y=0; y < 5; y++)
        {
            editIntVar2DArray(anker, "int2dtest", i, x, y);
            i++;
        }
    }
    printVars(anker);
}



int parser_start(char *templatefile, struct variables *anker, char *deliverfile,
                char *l_error_str, int *error_zeile)
{
    FILE *p_template, *p_outputfile;
    char linebuf[1024];
    bool function = false;
    int i, length, line = 0;
    parser_info status = {false, false, false, false, false, -1, "", 0, 0, malloc(sizeof(char))};
    char *forbuffer = malloc(1);
    int old_stdout;
    bool error = false;

    printf("---Starting Parser---\n");
    printf("Working with Vars:\n");
    printVars(anker);

    if((p_template = fopen(templatefile, "r")) == NULL)
    {
        sprintf(error_str, "Can not open templatefile: [%s] [%s]", templatefile,
                strerror(errno));
        strcpy(l_error_str, error_str);
        return(-1);
    }

    if((p_outputfile = fopen(deliverfile, "w")) == NULL)
    {
        fclose(p_template);
        sprintf(error_str, "Can not open Outputfile [%s] [%s]", deliverfile,
                strerror(errno));
        strcpy(l_error_str, error_str);
        return(-2);
    }

    old_stdout = dup(1);
    dup2(fileno(p_outputfile), 1);
    close(fileno(p_outputfile));

    do
    {
        line++;
        fgets(linebuf, sizeof(linebuf), p_template);  
        length = strlen(linebuf);
        if(local_jinja_parser(&status, anker, linebuf, length) != 0)
        {
            error = true;
            break;
        }
        bzero(linebuf, sizeof(linebuf));
    } while(!feof(p_template));
    fclose(p_template);

    fflush(stdout);

    dup2(old_stdout, 1);
    close(old_stdout);

    if(error == true)
    {
        *error_zeile = line;
        strcpy(l_error_str, error_str);
        return(-3);
    }

    return(0);
}
