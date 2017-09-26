#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vars.h"
#include "parser.h"
#include "parser_errno.h"
#include "for.h"
#include "rh4n_utils.h"

int parse_rangeCMD(char *cmd, for_status *stat);
int parse_ForVariable(char *cmd, for_status *stat);

int parseForCMD(token_t *start, for_status *stat)
{
    token_t *hptr = start;

    wchar_t cmd[200];
    int i = 0, ret = 0;
    char *c_cmd = NULL;

    //Jump over "in" keyword
    while(hptr)
    {
        if(hptr->type == SPACE)
            break;
        hptr = hptr->next;
    }

    if(hptr == NULL)
    {
        parser_errno = FOR_SYNTAX_ERROR;
        return(-1);
    }
    if(hptr->next == NULL)
    {
        parser_errno = FOR_SYNTAX_ERROR;
        return(-1);
    }

    hptr = hptr->next;

    while(hptr)
    {
        if(hptr->type == CMDSTARTEND)
            break;

        memcpy(cmd+(i++), &hptr->val, sizeof(wchar_t));
        hptr = hptr->next;
    }

    if(i == 0)
    {
        parser_errno = FOR_SYNTAX_ERROR;
        return(-2);
    }

    memset(cmd+i, 0x00, sizeof(wchar_t));

    if((c_cmd = malloc(wcslen(cmd)+1)) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        return(-2);
    }

    if(wcstombs(c_cmd, cmd, wcslen(cmd)+1) != wcslen(cmd))
    {
        parser_errno = UNICODE_IN_VARNAME;
        free(c_cmd);
        return(-3);
    }

    trimleadingSpaces(c_cmd);

    //printf("cmd: [%s]\n", c_cmd);

    if(memcmp(c_cmd, RANGE_STR, strlen(RANGE_STR)) == 0)
    {
        //It is the range() command
        //printf("found the range() command\n");
        stat->for_type = RANGE;   
        if((ret = parse_rangeCMD(c_cmd, stat)) != 0)
        {
            free(c_cmd);
            return(ret-3);
        }
    }
    else
    {
        //It is a variable
        //printf("Found a variable\n");
        stat->for_type = ARRAY;   
        if((ret = parse_ForVariable(c_cmd, stat)) != 0)
        {
            free(c_cmd);
            return(ret-10);
        }
    }
    free(c_cmd);
    return(0);
}

int parse_rangeCMD(char *cmd, for_status *stat)
{
    int i = 0, x = 0,
        cmd_length = strlen(cmd),
        start = -1, end = -2;

    char tmp_buff[200], *endptr = NULL;


    //Jump over the "range" keyword
    cmd = cmd+strlen(RANGE_STR);

    printf("start: [%s]\n", cmd);
    if(cmd[0] != '(')
    {
        parser_errno = MISSING_OPEN_BRACKET;
        return(-1);
    }

    cmd++;
    for(i=0; i < strlen(cmd) && cmd[i] != ')'; i++)
    {
        if(cmd[i] == ',')
        {
            tmp_buff[x] = 0x00;
            start = (int)strtol(tmp_buff, &endptr, 10);
            if(tmp_buff == endptr)
            {
                parser_errno = RANGE_INVALID_VALUE;
                return(-2);
            }
            x=0;
            continue;
        }
        tmp_buff[x++] = cmd[i];
    }
    tmp_buff[x] = 0x00;
    end = (int)strtol(tmp_buff, &endptr, 10);
    if(tmp_buff == endptr)
    {
        parser_errno = RANGE_INVALID_VALUE;
        return(-2);
    }

    stat->for_type = RANGE;
    stat->range.start = start;
    stat->range.end = end;

    return(0);
}

int parse_ForVariable(char *cmd, for_status *stat)
{
    char *clamps = NULL,
         *comma = NULL;


    if(cmd[0] != '(')
    {
        //Just one variablen
        if((stat->array.name = malloc(sizeof(char*))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        if((stat->array.grp = malloc(sizeof(char*))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        stat->array.grp[stat->array.var_count] = NULL;
        if((stat->array.array_length = malloc(sizeof(int*))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        if((stat->array.type = malloc(sizeof(int))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        if((stat->array.index_type = malloc(sizeof(int))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        if((stat->array.x_index = malloc(sizeof(int))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        if((stat->array.y_index = malloc(sizeof(int))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }

        stat->array.var_count = 1;
        RemoveSpaces(cmd);
        if((stat->array.name[0] = malloc(strlen(cmd)+1)) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        strcpy(stat->array.name[0], cmd);
        if(getVariableProperties(stat, 0) < 0)
        {
            return(-2);
        }
        return(0);
    }

    cmd++;

    if((clamps = strchr(cmd, ')')) == NULL)
    {
        parser_errno = MISSING_CLOSE_BRACKET;
        return(-1);
    }

    clamps[0] = '\0';

    comma = strtok(cmd, ",");
    while(comma)
    {
        if(stat->array.var_count == 0)
        {
            if((stat->array.name = malloc(sizeof(char*))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-1);
            }
            if((stat->array.grp = malloc(sizeof(char*))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-1);
            }
            stat->array.grp[stat->array.var_count] = NULL;
            if((stat->array.array_length = malloc(sizeof(int*))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-1);
            }
            if((stat->array.type = malloc(sizeof(int))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-1);
            }
            if((stat->array.index_type = malloc(sizeof(int))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-1);
            }
            if((stat->array.x_index = malloc(sizeof(int))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-1);
            }
            if((stat->array.y_index = malloc(sizeof(int))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-1);
            }
            
        }
        else
        {
            if((stat->array.name = realloc(stat->array.name,
                sizeof(char*)*(stat->array.var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-1);
            }
            if((stat->array.grp = realloc(stat->array.grp,
                sizeof(char*)*(stat->array.var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-1);
            }
            stat->array.grp[stat->array.var_count] = NULL;
            if((stat->array.array_length = realloc(stat->array.array_length,
                sizeof(int*)*(stat->array.var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-1);
            }
            if((stat->array.type= realloc(stat->array.type,
                sizeof(int)*(stat->array.var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-1);
            }
            if((stat->array.index_type = realloc(stat->array.index_type,
                sizeof(int)*(stat->array.var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-1);
            }
            if((stat->array.x_index = realloc(stat->array.x_index,
                sizeof(int)*(stat->array.var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-1);
            }
            if((stat->array.y_index = realloc(stat->array.y_index,
                sizeof(int)*(stat->array.var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-1);
            }
        }

        RemoveSpaces(comma);
        
        if((stat->array.name[stat->array.var_count] = malloc(strlen(comma)+1)) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        strcpy(stat->array.name[stat->array.var_count++], comma);
        if(getVariableProperties(stat, stat->array.var_count-1) < 0)
        {
            return(-2);
        }
        comma = strtok(NULL, ",");
    }

    return(0);
}

/*
 * Checks if mupltiple varnames were given.
 * If yes:
 *        they get split and saved in a two dim char array in the stat struct
 * If no:
 *        they only variable get saved in the two dim array in the stat struct
 */
int checkIfMultipleVarnames(char *found_names, for_status *stat)
{
    char *comma = NULL;
    if((comma = strchr(found_names, ','))  == NULL)
    {
        //Only one variable
        if((stat->varnames = malloc(sizeof(char*))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        if((stat->varnames[0] = malloc(strlen(found_names)+1)) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-2);
        }

        strcpy(stat->varnames[0], found_names);
        stat->var_count = 1;
        return(0);
    }

    comma = strtok(found_names, ",");

    //printf("Multiple varname:\n");
    while(comma)
    {
        if(stat->var_count == 0)
        {
            if((stat->varnames = malloc(sizeof(char*))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-3);
            }
        }
        else
        {
            if((stat->varnames = realloc(stat->varnames, sizeof(char*)*(stat->var_count+1))) == NULL)
            {
                parser_errno = MEM_REALLOC_ERROR;
                return(-3);
            }
        }
        if((stat->varnames[stat->var_count] = malloc(strlen(comma)+1)) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-3);
        }

        RemoveSpaces(comma);
        strcpy(stat->varnames[stat->var_count++], comma);
        //printf("var: [%s]\n", comma);
        comma = strtok(NULL, ",");
    }

    return(0);
}

//get the properties of the variable wich name is stored in the stat.array.name
//variable in the "index" index.
//the properrtes consist out of the variable type and the index which get found
int getVariableProperties(for_status *stat, int index)
{
    int type = -1, index_type = -1, x_index = -1, y_index = -1, z_index = -1,
        array_length[3], tmp = 0;

    char *point = NULL;

    char *grp = NULL,
         *var = NULL;

    stat->array.index_type[index] = getIndex(stat->array.name[index], &x_index, &y_index, &z_index);
    stat->array.x_index[index] = x_index;
    stat->array.y_index[index] = y_index;

    if((point = strchr(stat->array.name[index], '.')) != NULL)
    {
        //a group was found
        point[0] = '\0';
        if((stat->array.grp[index] = malloc((strlen(stat->array.name[index])+1)*sizeof(char))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-1);
        }
        strcpy(stat->array.grp[index], stat->array.name[index]);
        grp = stat->array.grp[index];

        tmp = strlen(stat->array.name[index])+1;
        memmove(stat->array.name[index], 
            stat->array.name[index]+(tmp), 
            strlen(point+1)+1);
        var = stat->array.name[index];
    }
    else
        var = stat->array.name[index];

    if(isDefinedGrpBool(vars_anker, grp, var) == false)
    {
        parser_errno = NOT_DEFINED_VAR;
        return(-1);
    }

    if((stat->array.array_length[index] = malloc(sizeof(int)*3)) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        return(-2);
    }

    if(getArrayLength(vars_anker, grp, var, &stat->array.array_length[index][0],
        &stat->array.array_length[index][1], &stat->array.array_length[index][2]) < 0)
    {
        parser_errno = VAR_HANDLE_ERROR;
        return(-3);
    }
    
    stat->array.type[index] = getVarType(vars_anker, grp, var);
} 
//Tries to parse the x,y,z index from variable string
//if a index was found the it get written in the corisponding parm
//Returns 0 if not index was found
//       -1 error
//       1 x index found
//       2 y index found
//       3 z index found
int getIndex(char *variable, int *x, int *y, int *z)
{
    char *index_buff = NULL;
    char *open_bracked = NULL, *close_bracked = NULL;

    if((open_bracked = strchr(variable, '[')) == NULL)
    {
        //Kein Index angeben
        return(0);
    }
    if((index_buff = malloc(strlen(open_bracked)+1)) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;    
        return(-1);
    }
    strcpy(index_buff, open_bracked);
    open_bracked[0] = '\0';

    memmove(index_buff, index_buff+1, strlen(index_buff));

    if((close_bracked = strchr(index_buff, ']')) == NULL)
    {
        parser_errno = MISSING_CLOSE_INDEX;
        free(index_buff);
        return(-2);
    }
    close_bracked[0] = '\0';

    if(str_isdigit(index_buff) < 0)
    {
        parser_errno = INT_CONVERSION_ERROR;
        free(index_buff);
        return(-4);
    }
    *x = atoi(index_buff);

    memmove(index_buff, index_buff+(strlen(index_buff)+1), strlen(close_bracked+1)+1);

    //Search for Y Index
    if(strlen(index_buff) == 0)
    {
        //Kein Y Index vorhanden
        free(index_buff);
        return(1);
    }

    if(index_buff[0] != '[')
    {
        parser_errno = UNEXPECTED_TOKEN;
        free(index_buff);
        return(-5);
    }
    memmove(index_buff, index_buff+1, strlen(index_buff));

    if((close_bracked = strchr(index_buff, ']')) == NULL)
    {
        parser_errno = MISSING_CLOSE_INDEX;
        free(index_buff);
        return(-6);
    }
    close_bracked[0] = '\0';

    if(str_isdigit(index_buff) < 0)
    {
        parser_errno = INT_CONVERSION_ERROR;
        free(index_buff);
        return(-8);
    }
    *y = atoi(index_buff);
    free(index_buff);
    return(2);
}
