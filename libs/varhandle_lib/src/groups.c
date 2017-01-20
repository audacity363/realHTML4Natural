#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "utils.h"
#include "group.h"

bool GrpisDefined(vars_t *anker, char *name)
{
    vars_t *hptr = anker;

    while(hptr)
    {
        if(strcmp(hptr->name, name) == 0 && hptr->type == GROUP)
        {
            return(true);
        }
        hptr  = hptr->next;
    }
    return(false);
}

int moveVariableToGroup(vars_t *anker, char *var_name, char *grp_name, vars_t **next_entry)
{
    vars_t *grp = NULL, *hptr = NULL, *var = NULL;

    if((grp = getGrpPointer(anker, grp_name)) == NULL)
    {
        return(GRP_NOT_DEFINED);
    }

    if((var = isDefined(anker, var_name)) == NULL)
        return(VAR_NOT_DEFINED);

    var->prev->next = var->next;
    var->next->prev = var->prev;
    *next_entry = var->next;
    var->next = NULL;

    if(grp->next_lvl == NULL)
    {
        grp->next_lvl = var;
        var->prev = grp;
    }
    else
    {
        //Goto the end of the group
        grp = goToAnkerEnd(grp->next_lvl);

        grp->next = var;
        var->prev = grp;
    }
    
    return(0);
}

int addGroup(vars_t *anker, char *name, int x_length, int y_length, int z_length)
{
    vars_t *end = NULL,
           *new = NULL;

    if(isDefined(anker, name))
    {
        return(GRP_ALREADY_DEFINED);
    }

    end = goToAnkerEnd(anker);

    if((new = malloc(sizeof(vars_t))) == NULL)
    {
        return(MEMORY_ALLOC_ERROR);
    }

    new->name = NULL;
    new->type = -1;
    new->level = -1;
    new->length = -1;
    new->name = NULL;
    new->data = NULL;
    new->z_length = -1;
    new->y_length = -1;
    new->x_length = -1;
    new->prev = NULL;
    new->next = NULL;
    new->next_lvl = NULL;

    if((new->name = malloc(strlen(name)+1)) == NULL)
    {
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    strcpy(new->name, name);
    new->type = GROUP;

    if(x_length > 0)
        new->x_length = x_length;
    else
        new->x_length = 0;
    if(y_length > 0)
        new->y_length = y_length;
    else
        new->y_length = 0;
    if(z_length > 0)
        new->z_length = z_length;
    else
        new->z_length = 0;

    end->next = new;
    new->prev = end;
    new->next_lvl = NULL;
    new->next = NULL;
    return(0);
}

vars_t *getGrpPointer(vars_t *anker, char *name)
{
    vars_t *hptr = anker;

    while(hptr)
    {
        if(strcmp(hptr->name, name) == 0 && hptr->type  == NULL)
        {
            return(hptr);
        }
        hptr  = hptr->next;
    }
    return(NULL);
}
