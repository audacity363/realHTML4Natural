#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "token_handling.h"

int addToken(token_t *anker, wchar_t *token, int type)
{
    token_t *hptr = anker,
            *new;

    while(hptr->next)
    {
        hptr = hptr->next;
    }
    
    if((new = malloc(sizeof(token_t))) == NULL)
    {
        return(false);
    }

    hptr->next = new;
    new->prev = hptr;
    new->next = NULL;

    new->type = type;
    memcpy(&new->val, token, sizeof(wchar_t));

    return(true);
}

void printTokens(token_t *anker)
{
    token_t *hptr;

    if(anker->prev)
        hptr = anker;
    else
        hptr = anker->next;

    while(hptr)
    {
        switch(hptr->type)
        {
            case VARIABLEBEGIN:
                printf("Begin Var\n");
                break;
            case VARIABLEEND:
                printf("End Var\n");
                break;
            case INDEXOPEN:
                printf("Index open\n");
                break;
            case INDEXCLOSE:
                printf("Index close\n");
                break;
            case CHAR:
                printf("Alphanumeric Char\n");
                break;
            case STR:
                printf("String start/ende\n");
                break;
            case EQUALS:
                printf("gleich\n");
                break;
            case GREATERTHEN:
                printf("groesser als");
                break;
            case LESSTHEN:
                printf("kleiner als");
                break;
            case SPACE:
                printf("Space\n");
                break;
            case CLINGTO:
                printf("Clings to\n");
                break;
            case CLAMPS:
                printf("Clamps\n");
                break;
            case BLOCKSTART:
                printf("Block open\n");
                break;
            case BLOCKEND:
                printf("Block closed\n");
                break;
            case CMDSTARTEND:
                printf("Command block start/end\n");
                break;
            default:
                printf("unkown Token\n");
                break;
        }
        hptr = hptr->next;
    }
}

void deleteTokens(token_t *anker)
{
    token_t *hptr = anker,
            *prev;

    //Zum ende der Liste gehen
    while(hptr->next)
        hptr = hptr->next;

    prev = hptr->prev;
    while(prev)
    {
        free(hptr);
        hptr = prev;
        prev = hptr->prev;
    }
}

token_t *jumpToOffset(token_t *anker, int offset)
{
    token_t *hptr = anker->next;
    int i;

    for(i=1; i <= offset; i++)
    {
        if(!hptr)
            break;
        hptr = hptr->next;
    }

    return(hptr);
}
