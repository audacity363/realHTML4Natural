#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "parser_errno.h"
#include "for.h"
#include "for_startvalues.h"

int for_handling(token_t *anker, status_t *stat)
{
    if(stat->just_save == true)
    {
        stat->in_for++;
        return(JUSTSAVE);
    }

    stat->just_save = true;
    stat->in_for = 1;
    stat->lookfor = FORBLOCK;
    return(JUSTSAVE);
}

int parseForHead(token_t *head, wchar_t *line)
{
    return(lineToTokens(head, line, line+wcslen(line)));
}

token_t *jumpOverForCMDName(token_t *anker)
{
    token_t *hptr = anker->next;
    int found_cmd = false;

    while(hptr)
    {
        if(hptr->type == SPACE && found_cmd)
        {
            hptr = hptr->next;
            break;
        }
        else if(hptr->type == CHAR)
        {
            found_cmd = true;
        }
        hptr = hptr->next;
    }

    return(hptr);
}

/*
 * Gets the variable name for the for loop. The variable name is limited
 * to 200 chars
 * Returns the variable name as normal char or on error NULL
 */
char *getForVariable(token_t *cmd, token_t **end_token)
{
    wchar_t varname[200];
    char *c_varname = NULL;

    token_t *hptr = cmd;

    int i = 0, end_token_con = SPACE;

    bool found_clamp = false;

    while(hptr != NULL)
    {
        if(hptr->type == end_token_con)
        {
            break;
        }
        if(hptr->type == CLINGTO)
            end_token_con = CLAMPS;

        memcpy(varname+i, &hptr->val, sizeof(wchar_t));
        i++;
        hptr = hptr->next;
    }

    memset(varname+i, 0x00, sizeof(wchar_t));

    if((c_varname = malloc(wcslen(varname)+1)) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        return(NULL);
    }

    if(wcstombs(c_varname, varname, wcslen(varname)+1) != wcslen(varname))
    {
        parser_errno = UNICODE_IN_VARNAME;
        free(c_varname);
        return(NULL);
    }
    if(end_token_con == CLAMPS)
        memmove(c_varname, c_varname+1, strlen(c_varname));

    if(end_token_con == CLAMPS) *end_token = hptr->next;
    else *end_token = hptr;

    return(c_varname);
}

int end_for_handling(token_t *anker, status_t *stat)
{
    int i, ret = 0,
        for_end = 0, x = 0;

    token_t head = {' ', -1, NULL, NULL},
            *cmd_start = NULL;

    wchar_t *head_str = NULL;
    char *varname = NULL;
    for_status status = {-1, {-1, -1}, {0, NULL, NULL, NULL, NULL, NULL, NULL},
                         0, 0, NULL};

    vars_t *for_vars = NULL;
    wchar_t **for_body = NULL;

    status_t for_parser_stat;
    for_parser_stat.in_for = 0;
    for_parser_stat.in_if = 0;
    for_parser_stat.save_buff= NULL;
    for_parser_stat.just_save = 0;
    for_parser_stat.sizeof_sav_buff = 0;

    if(stat->in_for == 0)
    {
        return(-1);
    }
    if(--stat->in_for != 0)
    {
        return(JUSTSAVE);
    }
    if(stat->lookfor != FORBLOCK)
    {
        //return(-3);
        return(JUSTSAVE);
    }

    parseForHead(&head, stat->save_buff[0]);

    cmd_start = jumpOverForCMDName(&head);

    if((varname = getForVariable(cmd_start, &cmd_start)) == NULL)
    {
        return(-1);
    }

    if(checkIfMultipleVarnames(varname, &status) < 0)
    {
        return(-3);
    }

    if(cmd_start->next == NULL)
    {
        parser_errno = FOR_TOO_FEW_ARGS;
        freeLineBuff(stat);
        cleanUpTokenList(&head);
        free(varname);
        return(-2);
    }
    cmd_start = cmd_start->next;

    if((ret = parseForCMD(cmd_start, &status)) < 0)
    {
        goto end;
    }

    /*for(i=0; i < status.array.var_count; i++)
    {
        printf("name: [%s]\n", status.array.name[i]);
        printf("\tvar_type: [%d]\n", status.array.type[i]);
        printf("\tlength: [%d] [%d] [%d]\n", status.array.array_length[i][0],
status.array.array_length[i][1],status.array.array_length[i][2]);
        printf("\tindex_type: [%d]\n", status.array.index_type[i]);
        printf("\tx_index: [%d]\n", status.array.x_index[i]);
        printf("\ty_index: [%d]\n", status.array.y_index[i]);
    }*/

    calculateForEnd(status, &for_end);

   //printf("For end: [%d]\n", for_end);
   
    //To get commands behind the for cmd to work
    if((for_body = malloc(sizeof(wchar_t*)*stat->sizeof_sav_buff)) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        return(-3);
    }

    wchar_t *behind_formcd;
    int offset = 0;

    if((behind_formcd = wcsstr(stat->save_buff[0], L"%}")) != NULL)
    {
        if(wcslen(behind_formcd) > 2)
        {
            for_body[0] = behind_formcd+2;
            offset = 1;
        }
    }

    for(i=offset;i < stat->sizeof_sav_buff;i++)
    {
        for_body[i] = stat->save_buff[i];
    }
   

    //Execute the for loop
    for(status.index=0; status.index < for_end; status.index++)
    {
    //    setLoopValue(varname, status, vars_anker, status.index);
        setStartValue(varname, status, vars_anker);

        for(i=0; i < stat->sizeof_sav_buff; i++)
        {
            if(parseLine(for_body[i], &for_parser_stat) == EXIT)
            {
                return(EXIT);
            }
        }

        freeForVars(status, vars_anker);

    }

    //TODO: Add a new function "removeGrp" for removing a full group
    removeVar(vars_anker, varname); 
end:

    for(i=0; i < status.var_count; i++)
    {
        free(status.varnames[i]);
        if(status.for_type == ARRAY)
        {
            free(status.array.array_length[i]);
            free(status.array.name[i]);
            free(status.array.grp[i]);
        } 
    }

    free(status.varnames);

    if(status.for_type == ARRAY)
    {
        free(status.array.type);
        free(status.array.y_index);
        free(status.array.x_index);
        free(status.array.length);
        free(status.array.index_type);
        free(status.array.array_length);
        free(status.array.name);
        free(status.array.grp);
    }

    stat->just_save = false;
    stat->lookfor = 0;

    freeLineBuff(stat);
    cleanUpTokenList(&head);
    free(varname);
    return(ret);
}

int freeForVars(for_status stat, vars_t *anker)
{
    //When it is just one Integer Variable we dont delete it
    if(stat.for_type == RANGE)
        return(0);

    int i = 0;

    for(; i < stat.var_count; i++)
        removeVar(anker, stat.varnames[i]);
}

//Calculate the real length of the for loop when start and end param from
//the range cmd is given so we can count from zero
int calculateForEnd(for_status stat, int *for_end)
{
    int i = 0, lowest_length = -1, cur_length = -1;

    if(stat.for_type == ARRAY)
    {
        if(stat.var_count != stat.array.var_count)
        {
            parser_errno = FOR_ARGS_MISSMATCH;
            return(-3);
        }

        for(; i < stat.var_count; i++)
        {
            cur_length = stat.array.array_length[i][stat.array.index_type[i]];
            if(cur_length < lowest_length || lowest_length == -1)
           {
               lowest_length = stat.array.array_length[i][stat.array.index_type[i]];
           }
        }

        *for_end = lowest_length;
        return(0);

    }

    if(stat.for_type == RANGE)
    {
        if(stat.range.start != -1)
        {
            *for_end = stat.range.end - stat.range.start;
        }
        else
        {
            *for_end = stat.range.end;
        }
    }
    return(0);
}

int setLoopValue(char *tmp_var, for_status stat, vars_t *anker, int index)
{
    editInteger(anker, "loop", "i", index);

    if(stat.for_type == RANGE)
    {
        if(stat.range.start != -1) editInteger(anker, NULL, tmp_var, stat.range.start+index);
        else editInteger(anker, NULL, tmp_var, index);
    }
}
