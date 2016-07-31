#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"

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
int searchCommand(char *line)
{
    char *found_cmd;

    if((found_cmd = strstr(line, "end-for")) != NULL)
    {
        return(2);
    }
    else if((found_cmd = strstr(line, "for")) != NULL)
    {
        return(1);
    }
    else if((found_cmd = strstr(line, "end-if")) != NULL)
    {
        return(4);
    }
    else if((found_cmd = strstr(line, "if")) != NULL)
    {
        return(3);
    }
    else if((found_cmd = strstr(line, "printVars")) != NULL)
    {
        return(5);
    }
    else if((found_cmd = strstr(line, "else")) != NULL)
    {
        return(6);
    }
    else
    {
        return(-1);
    }

}

int start_for(struct variables *anker, char *cmd_buff, FILE *p_output,
               char *error_str)
{    
    return(for_handle(anker, cmd_buff, p_output, error_str));
}

int start_if(struct variables *anker, char *cmd_buff, FILE *p_output,
            char *error_str)
{
    return(if_handle(anker, cmd_buff, p_output, error_str));
}
