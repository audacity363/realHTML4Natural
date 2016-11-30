#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "if.h"

int for_handling(token_t *anker, status_t *stat)
{
    printf("Hallo ich bin ein if\n");
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
        printf("Ende eines for blocks erreicht\n");
        return(JUSTSAVE);
    }
    if(stat->lookfor != FORBLOCK)
    {
        printf("Ende eines for blocks erreicht\n");
        return(JUSTSAVE);
    }
    printf("OK fuere nun for aus\n");
    printf("---------for body-------\n");
    for(i=1; i < stat->sizeof_sav_buff; i++)
    {
        printf("%s\n", stat->save_buff[i]);
    }
    printf("-----------------------\n");

    stat->just_save = false;
    stat->lookfor = 0;

    freeLineBuff(stat);
    return(0);
}
