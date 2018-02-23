#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "standard.h"
#include "rh4n_ldaparser.h"


void rh4nldaFreeList(RH4NLDAEntry_t *pldaentries) {
    rh4nldaFreeFork(pldaentries);
}

void rh4nldaFreeFork(RH4NLDAEntry_t *anker) {
    RH4NLDAEntry_t *hptr = NULL, *next = NULL;

    hptr = anker;
    while(hptr) {
        next = hptr->next;
        if(hptr->nextlvl) rh4nldaFreeFork(hptr->nextlvl);
        free(hptr);
        hptr = next;
    }
}
