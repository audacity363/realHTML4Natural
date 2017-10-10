#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vars.h"
#include "gen_json.h"

int startJSONgenerator(vars_t *var_anker, char *output_path, FILE *logfile)
{
    FILE *outputfile = NULL;
    vars_t *hptr = NULL;

    if((outputfile = fopen(output_path, "w")) == NULL)
    {
        fprintf(logfile, "Error opening outputfile [%s] [%s]\n", output_path, strerror(errno));
        return(-1);
    }

    hptr = var_anker->next;

    fputc('{', outputfile);

    while(hptr)
    {
        fprintf(outputfile, "\"%s\":", hptr->name);
        if(hptr->type == GROUP)
        {
            if(checkGroupArray(hptr) == true)
            {
                DEV_PRINT(("Generate Array of JSON objects from [%s]\n", hptr->name));
                printGroupArray(hptr, outputfile);
            }
            else
            {
                DEV_PRINT(("Generate JSON objects with entries from [%s]\n", hptr->name));
                printGroupEntries(hptr, outputfile);
            }
        }
        else
        {
            DEV_PRINT(("Generate JSON for: [%s]", hptr->name));
            printEntry(hptr, -1, -1, -1, outputfile);
        }

        if(hptr->next)
            fputc(',', outputfile);

        hptr = hptr->next;
    }

    fputc('}', outputfile);

    fclose(outputfile);
    return(0);
}

int printGroupArray(vars_t *grp, FILE *output)
{
    int array_length = 0, i=0;
    vars_t *hptr = NULL;

    fputc('[', output);

    array_length = getGroupArrayLength(grp);

    for(i=0; i < array_length; i++)
    {
        fputc('{', output);
        hptr = grp->next_lvl;
        while(hptr)
        {
            fprintf(output, "\"%s\":", hptr->name);
            printEntry(hptr, i, -1, -1, output);
            if(hptr->next)
                fputc(',', output);
            hptr = hptr->next;
        }
         
        fputc('}', output);
        if(i+1 < array_length)
            fputc(',', output);
    }

    fprintf(output, "]");
}

int printGroupEntries(vars_t *grp, FILE *output)
{

    vars_t *hptr = grp->next_lvl;

    fputc('{', output);

    while(hptr)
    {
        fprintf(output, "\"%s\":", hptr->name);
        printEntry(hptr, -1, -1, -1, output);

        if(hptr->next)
            fputc(',', output);

        hptr = hptr->next;
    }
    fputc('}', output);
}

bool checkGroupArray(vars_t *grp)
{
    DEV_PRINT(("Check Group [%s] for arrays\n", grp->name));
    
    int array_length = 0;
    vars_t *hptr = NULL;

    if(!grp->next_lvl)
        return(false);

    hptr = grp->next_lvl;

    /*if(grp->next_lvl->y_length > 0)
    {
        DEV_PRINT(("Var [%s] is a two dimensional array\n", grp->next_lvl->name));
        return(false);
    }*/
    
    array_length = hptr->x_length;

    // It is not and array
    if(array_length == -1)
    {
        return(false);
    }

    while(hptr)
    {
        /*if(hptr->y_length > 0)
        {
            DEV_PRINT(("Var [%s] is a two dimensional array\n", hptr->name));
            return(false);
        }*/
        if(hptr->x_length != array_length)
        {
            DEV_PRINT(("Var [%s] is different [%d/%d]\n", hptr->name, hptr->x_length, array_length));
            return(false);
        }
        hptr = hptr->next;
    }

    return(true);
}

int getGroupArrayLength(vars_t *grp)
{
    return(grp->next_lvl->x_length);
}

