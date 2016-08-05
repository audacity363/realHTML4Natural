#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"
#include "jinja_parser.h"
#include "command_handling.h"

/*Sucht nacht einem Befehl.
 * Return:
 * for       = 1
 * end-for   = 2
 * if        = 3
 * end-if    = 4
 * printVars = 5
 *
 * kein bekannter = -1
 */
int searchCommand(char *line, macros *anker)
{
    char *found_cmd;
    struct macro_definition *hptr;

    if((found_cmd = strstr(line, "end-for")) != NULL)
    {
        return(ENDFOR_CMD);
    }
    else if((found_cmd = strstr(line, "for")) != NULL)
    {
        return(FOR_CMD);
    }
    else if((found_cmd = strstr(line, "end-if")) != NULL)
    {
        return(ENDIF_CMD);
    }
    else if((found_cmd = strstr(line, "if")) != NULL)
    {
        return(IF_CMD);
    }
    else if((found_cmd = strstr(line, "printVars")) != NULL)
    {
        return(PRINTVARS_CMD);
    }
    else if((found_cmd = strstr(line, "else")) != NULL)
    {
        return(ELSE_CMD);
    }
    else if((found_cmd = strstr(line, "continue")) != NULL)
    {
        return(CONTINUE_CMD);
    }
    else if((found_cmd = strstr(line, "break")) != NULL)
    {
        return(BREAK_CMD);
    }
    else if((found_cmd = strstr(line, "import")) != NULL)
    {
        return(IMPORT_CMD);
    }
    else if((found_cmd = strstr(line, "end-macro")) != NULL)
    {
        return(ENDMACRO_CMD);
    }
    else if((found_cmd = strstr(line, "macro")) != NULL)
    {
        return(NEWMACRO_CMD);
    }
    else
    {
        hptr = anker->anker;
        while(hptr != NULL)
        {
            if(strstr(line, hptr->name) != NULL)
            {
                return(MACRO_CMD);
            }
            hptr = hptr->next;
        }
        return(-1);
    }

}

int start_for(struct variables *anker, char *cmd_buff, FILE *p_output,
               macros *macro_anker, char *error_str)
{    
    return(for_handle(anker, cmd_buff, p_output, macro_anker, error_str));
}

int start_if(struct variables *anker, char *cmd_buff, FILE *p_output,
            macros *macro_anker, char *error_str)
{
    return(if_handle(anker, cmd_buff, p_output, macro_anker, error_str));
}

int start_import(struct variables *anker, char *line, FILE *p_output,
                 macros *macro_anker, char *error_str)
{
    return(import_handle(anker, line, p_output, macro_anker, error_str));
}
