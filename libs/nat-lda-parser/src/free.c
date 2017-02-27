#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vars.h"
#include "lda.h"

void freeFork(vars_t *start);

void freeLDAAnker(vars_t *anker)
{
        freeFork(anker);    
}

void freeFork(vars_t *start)
{
    vars_t *hptr = start, *savptr = NULL;
    //got to the end of the anker when there is an fork free this one first
    while(hptr)
    {
        if(hptr->next_lvl != NULL)
            freeFork(hptr->next_lvl);
        savptr = hptr;
        hptr = hptr->next;
    }

    hptr = savptr;

    while(hptr->level == start->level && hptr != NULL)
    {
        free(hptr->name);
        hptr->name = NULL;
        //reaced first entry
        if(hptr->prev == NULL)
        {
            free(hptr);
            break;
        }

        //reaced group head
        if(hptr == start)
        {
            hptr->prev->next_lvl = NULL;
            free(hptr);
            break;
        }

        hptr = hptr->prev;
        free(hptr->next);
        hptr->next = NULL;
    }

}
