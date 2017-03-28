#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "parser_errno.h"
#include "token_handling.h"
#include "command_parsing.h"
#include "if.h"
#include "exec_if.h"

typedef struct 
{
    int index_type;
    int index[3];
} index_parms;

int if_handling(token_t *anker, status_t *stat)
{
    if(stat->just_save == true)
    {
        stat->in_if++;
        return(JUSTSAVE);
    }

    stat->just_save = true;
    stat->in_if = 1;
    stat->lookfor = IFBLOCK;
    return(JUSTSAVE);
}

int parseIfHead(token_t *head, wchar_t *line)
{
    return(lineToTokens(head, line, line+wcslen(line)));
}

token_t *jumpOverCMDName(token_t *anker)
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

bool checkIndexRange(vars_t *target, index_parms index)
{
    switch(index.index_type)
    {
        case 3:
            if(target->z_length < index.index[2])
            {
                parser_errno = INDEX_Z_OUT_OF_RANGE;
                return(false);
            }
        case 2:
            if(target->y_length < index.index[1])
            {
                parser_errno = INDEX_Y_OUT_OF_RANGE;
                return(false);
            }
        case 1:
            if(target->x_length < index.index[0])
            {
                parser_errno = INDEX_X_OUT_OF_RANGE;
                return(false);
            }
    }
    return(true);
}

int getCompareType(int length, wchar_t *cmp_str)
{
        if(wcsncmp(cmp_str, IFLESSTHEN_STR, length) == 0)
            return(IFLESSTHEN);
        if(wcsncmp(cmp_str, IFLESSTHENEQUAL_STR, length) == 0)
            return(IFLESSTHENEQUAL);
        if(wcsncmp(cmp_str, IFEQUALS_STR, length) == 0)
            return(IFEQUALS);
        if(wcsncmp(cmp_str, IFUNEQUALS_STR, length) == 0)
            return(IFUNEQUALS);
        if(wcsncmp(cmp_str, IFGREATERTHEN_STR, length) == 0)
            return(IFGREATERTHEN);
        if(wcsncmp(cmp_str, IFGREATERTHENEQUALS_STR, length) == 0)
            return(IFGREATERTHENEQUALS);
        if(wcsncmp(cmp_str, IFIN_STR, length) == 0)
            return(IFIN);
        return(-1);
}

int computePtrOffset(vars_t *target, index_parms index, int size)
{
    //TODO: Add Error handling for the index
    int sizeofz = 0, sizeofy = 0, offset = 0;

    if(checkIndexRange(target, index) == false)
    {
        return(-1);
    }

    if(index.index_type == 3)
    {
        sizeofz = size*(target->z_length);
        sizeofy = sizeofz*(target->y_length);
        offset = (index.index[0]*sizeofy)+
            (index.index[1]*sizeofz)+(size*index.index[2]);
    }
    else if(index.index_type == 2)
    {
        offset = (target->y_length*size);
        offset = offset*(index.index[0])+(size*index.index[1]);
    }
    else if(index.index_type == 1)
    {

        offset = size*(index.index[0]);
    }

    //printf("offset: [%d]\n", offset);
    return(offset);

}

int getSizeofVariable(vars_t *target)
{
    if(target->type == INTEGER || target->type == ONEDINTEGER || 
        target->type == TWODINTEGER || target->type == THREEDINTEGER)
        return(sizeof(int));

    if(target->type == BOOL || target->type == ONEDBOOL || 
        target->type == TWODBOOL|| target->type == THREEDBOOL)
        return(sizeof(bool));

    if(target->type == FLOAT || target->type == ONEDFLOAT || 
        target->type == TWODFLOAT || target->type == THREEDFLOAT)
        return(sizeof(double));

    if(target->type == STRING || target->type == ONEDSTRING || 
        target->type == TWODSTRING || target->type == THREEDSTRING)
        return(target->length*sizeof(wchar_t));

    return(-1);
}

//when it is not a variable parse the value and write it into data. When it is
//a variable get the pointer to the value in the variablen list. When it is a
//array: calculate the offset and save this pointer
int addParm(if_parms_t *parms, int val_len, wchar_t *val, int str, int compare, index_parms index)
{
    if_parms_t *new = NULL, *hptr = parms;
    char *c_val = NULL, *grp_delmiter = NULL,
         *var_name = NULL, *grp_name = NULL;

    vars_t *target = NULL;

    int offset = 0;

    if((new = malloc(sizeof(if_parms_t))) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        return(-1);
    }

    new->type = -1;

    if(str)
    {
        if((new->data = malloc((wcslen(val)+1)*sizeof(wchar_t))) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-2);
        }
        wcscpy(new->data, val);
        new->sizeof_data = (wcslen(val)+1)*sizeof(wchar_t);
    }
    else if(compare)
    {
        if((new->type = getCompareType(val_len, val)) == -1)
        {
            parser_errno = UNSUPPORTED_COMPARE;
            return(-2);
        }
    }
    else
    {
        //Convert the unicode string into a ascii string
        if((c_val = malloc(wcslen(val)+1)) == NULL)
        {
            parser_errno = MEM_ALLOC_ERROR;
            return(-3);
        }
        if(wcstombs(c_val, val, wcslen(val)+1) == -1)
        {
            parser_errno = UNICODE_IN_VARNAME;
            return(-4);
        }

        //Check if it is a boolean
        if(strcmp(c_val, TRUE_STR) == 0)
        {
            if((new->data = malloc(sizeof(bool))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-5);
            }
            *((bool*)new->data) = true;
            new->sizeof_data = sizeof(bool);
            goto save;
        }
        else if(strcmp(c_val, FALSE_STR) == 0)
        {
            if((new->data = malloc(sizeof(bool))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-5);
            }
            *((bool*)new->data) = false;
            new->sizeof_data = sizeof(bool);
            goto save;
        }
            
        //Check if a group is specified
        if((grp_delmiter = strchr(c_val, '.')) != NULL)
        {
            grp_delmiter[0] = '\0';
            grp_name = c_val;
            var_name = ++grp_delmiter;
        }
        else
        {
            var_name = c_val;
        }

        if((target = isDefinedGrp(vars_anker, grp_name, var_name)) == NULL)
        {
            //It is not a variable. It must be an integer, boolean, float or 
            //the variable is just not defined
            //TODO: Add integer parsing
            if((new->data = malloc(sizeof(int))) == NULL)
            {
                parser_errno = MEM_ALLOC_ERROR;
                return(-5);
            }
            char *end_ptr = NULL, *tmp = NULL;
            tmp = strchr(c_val, '\0');
            *((int*)new->data) = strtol(c_val, &end_ptr, 10);
            if(end_ptr >= c_val && end_ptr < tmp)
            {
                parser_errno = INT_CONVERSION_ERROR;
                return(NULL);
            }
            new->sizeof_data = sizeof(int);
        }
        else
        {
            if((new->sizeof_data = getSizeofVariable(target)) == -1)
            {
                return(-1);
            }
            offset = computePtrOffset(target, index, new->sizeof_data);
            new->data = target->data+offset;
        }

    }

save:
    while(hptr->next)
        hptr = hptr->next;

    new->next = NULL; 
    new->prev = hptr;

    hptr->next = new;
    
    return(0);
}

void printParms(if_parms_t *anker)
{
    if_parms_t *hptr;

    if(anker->prev)
        hptr = anker;
    else
        hptr = anker->next;

    while(hptr)
    {
        switch(hptr->type)
        {
            case IFVARIABLE:
                printf("variable\n\tval: [%S]\n", hptr->val);
                break;
            case IFSTR:
                printf("string\n\tval: [%S]\n", hptr->val);
                break;
            case IFCOMPARE:
                printf("compare\n\tval: [%S]\n", hptr->val);
                break;
            default:
                printf("unkown\n\tval: [%S]\n", hptr->val);
                
        }
        hptr = hptr->next;
    }
}

token_t *jumpOverSpaces(token_t *tokens)
{
    token_t *hptr = tokens;

    while(hptr)
    {
        if(hptr->type != SPACE)
        {
            return(hptr);
        }
        hptr = hptr->next;
    }
    return(NULL);
}

token_t *findNextParm(if_parms_t *parms, token_t *tokens)
{
    token_t *hptr = tokens;
    int in_str = false,
        in_index = 0,
        length = 1,
        found_str = false,
        index_i = 0,
        found_compare = false;
    wchar_t *buff = malloc(sizeof(wchar_t));
    wchar_t index_buff[4] = {0, 0, 0, 0};
    index_parms index = {0, {-1, -1, -1}};
  
    hptr = jumpOverSpaces(hptr);

    while(hptr)
    {
        if(hptr->type == SPACE && in_str)
        {
            memcpy(buff+(length-1), &hptr->val, sizeof(wchar_t));
            buff = realloc(buff, ++length*sizeof(wchar_t));
        }
        else if(hptr->type == SPACE)
            break;
        else if(hptr->type == STR && in_str)
        {
            in_str = false;
            break;
        }
        else if(hptr->type == INDEXOPEN && !in_str && !in_index)
        {
            in_index = true;
        }
        else if(hptr->type == INDEXCLOSE && !in_str && in_index)
        {
            index.index[index.index_type++] = wcstol(index_buff, NULL, 10);    
            memset(index_buff, 0x00, sizeof(index_buff));
            index_i = 0;
            in_index = false;
        }
        else if(hptr->type == CHAR && in_index)
        {
            memcpy(index_buff+(index_i++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == STR && !in_str)
        {
            in_str = true;
            found_str = true;
        }
        else if(hptr->type == CHAR)
        {
            memcpy(buff+(length-1), &hptr->val, sizeof(wchar_t));
            buff = realloc(buff, ++length*sizeof(wchar_t));
        }
        else if(hptr->type == EQUALS || hptr->type == GREATERTHEN || hptr->type == LESSTHEN)
        {
            found_compare = true;
            memcpy(buff+(length-1), &hptr->val, sizeof(wchar_t));
            buff = realloc(buff, ++length*sizeof(wchar_t));
        }
        else if(hptr->type == BLOCKEND)
            break;
        hptr = hptr->next;
    }

    //Found just a index declaration
    if(index.index_type != 0 && length == 1)
    {
        parser_errno = FOUND_SINGLE_INDEX;
        return(NULL);
    }

    if(!hptr || length == 1)
    {
        free(buff);
        return(NULL);
    }
    hptr = hptr->next;

    memset(buff+(length-1), 0x00, sizeof(wchar_t));

    printf("Index_type :[%d]\n", index.index_type);

    if(addParm(parms, length, buff, found_str, found_compare, index) != 0)
    {
        free(buff);
        return(NULL);
    }

    free(buff);

    return(hptr);
}

token_t *findParms(if_parms_t *parms, token_t *head)
{
    token_t *hptr;

    //Jump over command name
    hptr = jumpOverCMDName(head);
    
    return(hptr);
}


int end_if_handling(token_t *anker, status_t *stat)
{
    wchar_t *if_head;
    int ret = 0;

    token_t head = {' ', -1, NULL, NULL},
            *parm_start,
            *last_parm;
    if_parms_t parms = {NULL, -1, NULL, -1, -1, -1, -1, -1, -1, -1, NULL, NULL};

    if(stat->in_if == 0)
    {
        parser_errno = END_IF_WITHOUT_START;
        return(-1);
    }
    if(--stat->in_if != 0)
    {
        return(JUSTSAVE);
    }
    if(stat->lookfor != IFBLOCK)
    {
        return(JUSTSAVE);
    }

    if_head = stat->save_buff[0];
    parseIfHead(&head, if_head);

    parm_start = findParms(&parms, &head);

    last_parm = findNextParm(&parms, parm_start);
    while(last_parm)
    {
        last_parm = findNextParm(&parms, last_parm);
    }
    if(parser_errno != 0)
    {
        return(-2);
    }

    //printParms(&parms);

    stat->just_save = false;
    stat->lookfor = 0;

    ret = exec_if(&parms, stat->save_buff, stat->sizeof_sav_buff);

    freeLineBuff(stat);
    return(ret);
}
