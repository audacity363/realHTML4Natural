#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "parser.h"
#include "if.h"

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

int end_for_handling(token_t *anker, status_t *stat)
{
    int i;

    if(stat->in_for == 0)
    {
        fprintf(stderr, "Found end-for without starting for\n");
        return(-1);
    }
    if(--stat->in_for != 0)
    {
        return(JUSTSAVE);
    }
    if(stat->lookfor != FORBLOCK)
    {
        return(JUSTSAVE);
    }
    printf("---------for body-------\n");
    for(i=1; i < stat->sizeof_sav_buff; i++)
    {
        printf("%S\n", stat->save_buff[i]);
    }
    printf("-----------------------\n");

    stat->just_save = false;
    stat->lookfor = 0;

    freeLineBuff(stat);
    return(0);
}
