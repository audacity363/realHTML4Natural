#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "var2name.h"
#include "lda_parser.h"

void handleRedefine(struct variables *var_anker, int valueoffset, struct varnames *redefanker)
{
    void *tmpbuffer;
    struct variables *hptr = var_anker->next; 
    struct varnames *ptr;
    int i,
        dataoffset = 0;
    ptr = redefanker->nextlevel;
    

    for(i=0; i != valueoffset; i++)
    {
        hptr = hptr->next;       
    }

    while(ptr != NULL)
    {
        if(dataoffset >= strlen(hptr->data))
        {
            newStringVar(var_anker, ptr->name, " ");
            dataoffset += ptr->length;
            ptr = ptr->next;
            continue;
        }
        tmpbuffer = malloc(ptr->length);
        memcpy(tmpbuffer, hptr->data+dataoffset, ptr->length);

        newStringVar(var_anker, ptr->name, (char*)tmpbuffer);

        free(tmpbuffer);
        dataoffset += ptr->length;
        ptr = ptr->next;
    }
}

int var2name(struct variables *var_anker, char *ldaname, char *error_str)
{
        
    struct variables *var_hptr = var_anker->next;
    struct varnames  *names_hptr, *names_ptr;
    int varoffset = 0,
        i = 0;
    bool ingrp = false;
    char *l_error_str = malloc(2048);
    strcpy(error_str, "Test");

    if((names_hptr = getVarNames(ldaname, l_error_str)) == NULL)
    {
        printf("Error while parsing LDA\n");
        strcpy(error_str, l_error_str);
        return(-1);
    }
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
            strcpy(error_str, "Offset to high");
            return(-1);
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

        strcpy(var_hptr->name, names_ptr->name);

        if(names_ptr->next == NULL && ingrp == true)
        {
            while(names_ptr->level != 2)
                names_ptr = names_ptr->prev; 
            ingrp = false;
        }

        names_ptr = names_ptr->next;
        if(names_ptr == NULL)
            return(0);
        varoffset++;
    }
    return(0);
}
