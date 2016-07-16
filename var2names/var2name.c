#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "parser.h"
#include "varhandle.h"
#include "var2name.h"

void handleRedefine(struct variables *var_anker, int valueoffset, struct varnames *redefanker)
{
    void *tmpbuffer;
    struct variables *hptr = var_anker->next; 
    struct varnames *ptr = redefanker->nextlevel;
    int i,
        dataoffset = 0;
    

    for(i=0; i != valueoffset; i++)
    {
        hptr = hptr->next;       
    }

    while(ptr != NULL)
    {
        tmpbuffer = malloc(ptr->length);
        memcpy(tmpbuffer, hptr->data+dataoffset, ptr->length);

        newStringVar(var_anker, ptr->name, (char*)tmpbuffer);

        free(tmpbuffer);
        dataoffset += ptr->length;
        ptr = ptr->next;
    }
}

void var2name(struct variables *var_anker, char *ldaname)
{
        
    struct variables *var_hptr = var_anker->next;
    struct varnames  *names_hptr, *names_ptr;
    int varoffset = 0,
        i = 0;
    bool ingrp = false;

    names_hptr = getVarNames(ldaname);

    printfork(names_hptr);
    printVars(var_anker);

    names_ptr = names_hptr = names_hptr->next->nextlevel;

    while(true)
    {
        var_hptr = var_anker->next;
        for(i=0; i != varoffset; i++)
        {
            var_hptr = var_hptr->next;
        }
        if(var_hptr == NULL)
        {
            printf("offset to high\n");
            return;
        }

        if(names_ptr->type == GROUP_LEADER)
        {
            names_ptr = names_ptr->nextlevel;
            ingrp = true;
        }
        if(names_ptr->type == REDEFINE)
        {
            handleRedefine(var_anker, varoffset-1, names_ptr);
            names_ptr = names_ptr->next;
        }

        /*printVar(var_anker, var_hptr->name);*/
        /*printSingleVar(names_ptr);*/
        strcpy(var_hptr->name, names_ptr->name);

        printf("\n\n");

        if(names_ptr->next == NULL && ingrp == true)
        {
            while(names_ptr->level != 2)
                names_ptr = names_ptr->prev; 
            ingrp = false;
        }

        names_ptr = names_ptr->next;
        if(names_ptr == NULL)
            return;
        varoffset++;
    }
}
