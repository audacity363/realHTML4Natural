#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "token_handling.h"

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

//Sucht den Index raus. 
//Returnwerte: false = kein Index gefunden
//                 1 = 1 Index gefunden
//                 2 = 2 Index gefunden
//                 3 = 3 Index gefunden
//Die gefundenn werden in das Array index_num geschrieben
int getIndex(token_t *anker, int *index_num)
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
            fprintf(stderr, "Syntax Error. Missing \"]\"\n");
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
            fprintf(stderr, "Syntax Error. Missing \"[\"\n");
            return(-2);
        }
        else if(hptr->type == INDEXCLOSE && in_brackets == true)
        {
            in_brackets = false;
            c_index[i] = '\0';
            if((index_num[0] = atoi(c_index)) == 0 && c_index[0] != '0')
            {
                fprintf(stderr, "Conv Error\n");
                return(-3);
            }
        } 
        hptr = hptr->next;
    }
    return(index_type);
}

int parseVariable(wchar_t *begin, wchar_t *end)
{
    wchar_t *curpos = begin,
            *variablename;
    token_t anker;
    int varname_length, 
        i = 0;

    anker.next = NULL;
    anker.prev = NULL;

    while(curpos+i <= end)
    {
        if(wcsncmp(curpos+i, L"{", 1) == 0)
            addToken(&anker, curpos+i, VARIABLEBEGIN);
        else if(wcsncmp(curpos+i, L"[", 1) == 0)
            addToken(&anker, curpos+i, INDEXOPEN);
        else if(wcsncmp(curpos+i, L"[", 1) == 0)
            addToken(&anker, curpos+i, INDEXCLOSE);
        else if(wcsncmp(curpos+i, L"}", 1) == 0)
            addToken(&anker, curpos+i, VARIABLEEND);
        else if(wcsncmp(curpos+i, L" ", 1) == 0)
            addToken(&anker, curpos+i, SPACE);
        else
            addToken(&anker, curpos+i, CHAR);
        i++;
    }
    printTokens(&anker);
    if((varname_length = getVarnameLength(&anker)) == 0) 
    {
        fprintf(stderr, "Keine Variable gefunden\n");
        return(-1);
    }
    varname_length++;
    if((variablename = malloc(varname_length*sizeof(wchar_t))) == NULL)
    {
        return(-2);
    }

    getVarname(&anker, variablename);
    free(variablename);

    deleteTokens(&anker);
    printf("------------------------------------------------------\n");
    return(0);
}
