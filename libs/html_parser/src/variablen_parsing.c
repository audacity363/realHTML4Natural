#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "token_handling.h"

#include "parser_errno.h"

int printVariable(char *name, char *grpname, bool has_grp, int index_type, int *index_num);
int handleONEDINTEGER(vars_t *target, int index_type, int *index);

int getVarnameLength(token_t *anker)
{
    token_t *hptr = anker->next;
    int length = 0;

    while(hptr)
    {
        if(hptr->type == INDEXOPEN)
        {
            break;
        }
        if(hptr->type == CHAR)
        {
            length++;
        }
        hptr = hptr->next;
    }

    return(length);
}

void getVarname(token_t *anker, wchar_t *buff)
{
    token_t *hptr = anker->next;
    int length = 0;

    while(hptr)
    {
        if(hptr->type == INDEXOPEN)
        {
            break;
        }
        if(hptr->type == CHAR)
        {
            memcpy(buff+(length++), &hptr->val, sizeof(wchar_t));
        }
        hptr = hptr->next;
    }
    memset(buff+length, 0x00, sizeof(wchar_t));
}

/*
 * checks if there is a groupname in the variable name.
 * Returns: > 0 length of the group name
 *          = 0 no group found
 */
int checkIfGroup(char *varname)
{
    char *point = NULL;
    int length = 0;

    if((point = strchr(varname, '.')) == NULL)
        return(0);

    length = point-varname;

    return(length);
    
}

/*
 * copy the group with the length of grplength
 * into the grpname buffer
 * Returns: Pointer to the begin of the variable name
 */
char *getGroup(char *varname, int grplength, char *grpname)
{
    strncpy(grpname, varname, grplength);

    return(varname+(grplength+1));
}

//Sucht den Index raus. 
//Returnwerte: false = kein Index gefunden
//                 1 = 1 Index gefunden
//                 2 = 2 Index gefunden
//                 3 = 3 Index gefunden
//Die gefundenn werden in das Array index_num geschrieben
int getIndexChar(token_t *anker, int *index_num)
{
    int index_type = 0,
        in_brackets = false,
        i = 0;

    token_t *hptr = anker->next;
    char c_index[4];

    while(hptr)
    {
        if(hptr->type == INDEXOPEN && in_brackets == true)
        {
            parser_errno = MISSING_CLOSE_INDEX;
            return(-1);
        }
        else if(hptr->type == INDEXOPEN && in_brackets == false)
        {
            in_brackets = true;
            index_type++;
        }
        else if(hptr->type == CHAR && in_brackets == true)
        {
            c_index[i++] = hptr->val; 
        }
        else if(hptr->type == INDEXCLOSE && in_brackets == false)
        {
            parser_errno = MISSING_OPEN_INDEX;
            return(-2);
        }
        else if(hptr->type == INDEXCLOSE && in_brackets == true)
        {
            in_brackets = false;
            c_index[i] = '\0';
            if((index_num[index_type-1] = atoi(c_index)) == 0 && c_index[0] != '0')
            {
                parser_errno = INT_CONVERSION_ERROR;
                return(-3);
            }
            i=0;
        } 
        hptr = hptr->next;
    }
    return(index_type);
}

int parseVariable(wchar_t *begin, wchar_t *end)
{
    wchar_t *curpos = begin,
            *wc_variablename = NULL;
    char *variablename = NULL, *varname_sav = NULL, *grpname = NULL;
    token_t anker;
    int varname_length = 0,
        i = 0,
        index_type = 0,
        index[3] = {-1, -1, -1},
        grplength = 0, ret = 0;

    bool found_grp = false;

    anker.next = NULL;
    anker.prev = NULL;

    memset(&anker, 0x00, sizeof(token_t));

    //Create a tokenlist out of the string
    while(curpos+i <= end)
    {
        if(wcsncmp(curpos+i, L"{", 1) == 0)
            addToken(&anker, curpos+i, VARIABLEBEGIN);
        else if(wcsncmp(curpos+i, L"[", 1) == 0)
            addToken(&anker, curpos+i, INDEXOPEN);
        else if(wcsncmp(curpos+i, L"]", 1) == 0)
            addToken(&anker, curpos+i, INDEXCLOSE);
        else if(wcsncmp(curpos+i, L"}", 1) == 0)
            addToken(&anker, curpos+i, VARIABLEEND);
        else if(wcsncmp(curpos+i, L" ", 1) == 0)
            addToken(&anker, curpos+i, SPACE);
        else
            addToken(&anker, curpos+i, CHAR);
        i++;
    }
    //printTokens(&anker);

    if((varname_length = getVarnameLength(&anker)) == 0) 
    {
        deleteTokens(&anker);
        parser_errno = EMPTY_VAR_BLOCK;
        return(-1);
    }
    varname_length++;
    if((wc_variablename = malloc(varname_length*sizeof(wchar_t))) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        deleteTokens(&anker);
        return(-2);
    }

    if((variablename = malloc(varname_length)) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        free(wc_variablename);
        deleteTokens(&anker);
        return(-3);
    }

    getVarname(&anker, wc_variablename);

    //varname_length includes the '\0' char and wcstombs returns the length without
    //the null char
    if(wcstombs(variablename, wc_variablename, varname_length) != varname_length-1)
    {
        free(variablename);
        free(wc_variablename);
        deleteTokens(&anker);
        parser_errno = UNICODE_IN_VARNAME;
        return(-4);
    }
    varname_sav = variablename;
    index_type = getIndexChar(&anker, index);

    if((grplength = checkIfGroup(variablename)) != 0)
    {
        if((grpname = calloc(1, grplength+1)) == NULL)
        {
            free(variablename);
            free(wc_variablename);
            deleteTokens(&anker);
            parser_errno = MEM_ALLOC_ERROR;
            return(-5);
        }

        variablename = getGroup(variablename, grplength, grpname);
        found_grp = true;
    }

    if((ret = printVariable(variablename, grpname, found_grp, index_type, index)) < 0)
    {
        free(varname_sav);
        free(wc_variablename);
        free(grpname);
        deleteTokens(&anker);
        return(ret-5);
    }
    
    free(varname_sav);
    free(wc_variablename);
    free(grpname);

    deleteTokens(&anker);
    //printf("\n------------------------------------------------------\n");
    return(0);
}

int printVariable(char *name, char *grpname, bool has_grp, int index_type, int *index_num)
{
    vars_t *target = NULL;

    if(has_grp)
    {
        //printf("Grpname: [%s]\n", grpname);
        if((target = isDefinedGrp(vars_anker, grpname, name)) == NULL)
        {
            parser_errno = NOT_DEFINED_VAR;
            return(-1);
        }
    }
    else
    {
        //printf("Found name: [%s] with index = %d\n", name, index_type);
        
        if((target = isDefined(vars_anker, name)) == NULL)
        {
            parser_errno = NOT_DEFINED_VAR;
            return(-2);
        }
    }

    switch(target->type)
    {
        case INTEGER:
            printRawInteger(target, f_output);
            break;
        case ONEDINTEGER:
            handleONEDINTEGER(target, index_type, index_num);
            break;
        case TWODINTEGER:
            handleTWODINTEGER(target, index_type, index_num);
            break;
        case THREEDINTEGER:
            handleTHREEDINTEGER(target, index_type, index_num);
            break;
        case FLOAT:
            printRawFloat(target, f_output);
            break;
        case ONEDFLOAT:
            handleONEDFLOAT(target, index_type, index_num);
            break;
        case TWODFLOAT:
            handleTWODFLOAT(target, index_type, index_num);
            break;
        case THREEDFLOAT:
            handleTHREEDFLOAT(target, index_type, index_num);
            break;
        case BOOL:
            printRawBoolean(target, f_output);
            break;
        case ONEDBOOL:
            handleONEDBOOL(target, index_type, index_num);
            break;
        case TWODBOOL:
            handleTWODBOOL(target, index_type, index_num);
            break;
        case THREEDBOOL:
            handleTHREEDSTRING(target, index_type, index_num);
            break;
        case STRING:
            printRawString(target, f_output);
            break;
        case ONEDSTRING:
            handleONEDSTRING(target, index_type, index_num);
            break;
        case TWODSTRING:
            handleTWODSTRING(target, index_type, index_num);
            break;
        case THREEDSTRING:
            handleTHREEDSTRING(target, index_type, index_num);
            break;
    }
    return(0);
}
