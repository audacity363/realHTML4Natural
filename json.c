#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"

int exec_json(token_t *anker, status_t *stat)
{
    char **varnames = NULL;
    wchar_t name_buff[200];
    memset(name_buff, 0x00, sizeof(name_buff));

    int index_name = 0,
        var_count = 0, i = 0;
    
    token_t *hptr = NULL;

    if(!(varnames = malloc(sizeof(char*))))
    {
        return(-1);
    }

    hptr = anker;

    while(hptr)
    {
        if(hptr->type == COMMA || hptr->type == CLAMPS)
        {
            memset(name_buff+index_name, 0x00, sizeof(wchar_t));
            if(!(varnames[var_count] = malloc(sizeof(char)*wcslen(name_buff))))
            {
                for(i=0; i < var_count; i++)
                    free(varnames[i]);
                free(varnames);
                return(-2);
            }

            wcstombs(varnames[var_count], name_buff, wcslen(name_buff)+1);

            if(!(varnames = realloc(varnames, sizeof(char*)*((++var_count)+1))))
            {
                return(-3);
            }

            index_name = 0;
        }
        else if(hptr->type == CHAR)
        {
            memcpy(name_buff+index_name, &hptr->val, sizeof(wchar_t));
            index_name++;
        }
        hptr = hptr->next;
    }

    if(printVarsToFileJSON(vars_anker, varnames, var_count, stdout) != 0)
    {
        //variable not known
        for(i=0; i < var_count; i++)
            free(varnames[i]);
        free(varnames);
        return(-4);
    }

    for(i=0; i < var_count; i++)
        free(varnames[i]);
    free(varnames);

    return(0);
}
