#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"
#include "file_handling.h"
#include "jinja_parser.h"

int getMacroName(macros *macro_anker, char *line, char *error_str)
{
    char *begin, *end;
    char *import_line, *cmd_buff = NULL;

    if((begin = strchr(line, '"')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"");
        return(-1);
    }

    if((end = strchr(begin+1, '"')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"");
        return(-2);
    }
    end[0] = '\0';

    if(strlen(begin+1) == 0)
    {
        strcpy(error_str, "Syntax Error: Empty String");
        return(-3);
    }

    strcpy(macro_anker->cur_name, begin+1);

    return(0);
}

int saveMacro(macros *macros_anker)
{
    struct macro_definition *hptr, *new_value, *anker;
    
    hptr = macros_anker->anker;

    while(hptr->next != NULL)
    {
        hptr = hptr->next;
    }

    new_value = malloc(sizeof(struct macro_definition));
    strcpy(new_value->name, macros_anker->cur_name);
    new_value->value = malloc(strlen(macros_anker->macro_buff)+1);
    strcpy(new_value->value, macros_anker->macro_buff);
    new_value->next = NULL;
    hptr->next = new_value;

    free(macros_anker->macro_buff);
    bzero(macros_anker->cur_name, 1024);
    return(0);
}

int printMacro(macros *macro_anker, char *line, FILE *p_output, 
               char *error_str)
{
    struct macro_definition *hptr, *new_value, *anker;

    TrimSpaces(line);
    line = StripTrailingSpaces(line);
   
    hptr = macro_anker->anker->next;

    while(hptr != NULL)
    {
        if(strcmp(hptr->name, line) == 0)
        {
            break;
        }
        hptr = hptr->next;
    }

    if(hptr == NULL)
    {
        sprintf(error_str, "Name Error: Macro [%s] not found", line);
        return(-1);
    }

    fprintf(p_output, "%s", hptr->value);
    return(0);
}
