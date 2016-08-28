#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"

void initVariable(struct variables *anker)
{
    anker = malloc(sizeof(struct variables));
}

struct variables *searchVar(struct variables *anker, char *name)
{
    struct variables *hptr;

    hptr = anker;
    while(hptr != NULL)
    {
        if(strcmp(hptr->name, name) == 0)
            break;
        hptr = hptr->next;
    }

    return(hptr);

}

int copyVar(struct variables *first, struct variables *second, char *name, 
            char *error_str)
{
    struct variables *hptr1, *hptr2, *new_var;

    if((hptr1 = searchVar(first, name)) == NULL)
    {
        sprintf(error_str, "Unkown Variable: %s", name);
        return(-1);
    }

    hptr2 = second;

    while(hptr2->next != NULL)
    {
        hptr2 = hptr2->next;
    }

    new_var = malloc(sizeof(struct variables));
    switch(hptr1->type)
    {
        case INT:
            new_var->data = malloc(sizeof(int));
            break;
        case INTARRAY:
            new_var->data = malloc(sizeof(int)*(hptr1->length));
            break;    
        case TWO_DINTARRAY:
            new_var->data = malloc((hptr1->x_length*hptr1->y_length)*sizeof(int));
            break;
        case STRING:
            new_var->data = malloc(sizeof(char)*strlen(hptr1->data));
            break;
    }
    memcpy(new_var, hptr2, sizeof(struct variables));
    hptr2->next = new_var;

    return(0);
}

int getArrayLength(struct variables *anker, char *name, int *x, int *y)
{
    struct variables *ptr;
   
    ptr = anker;
    while(ptr != NULL)
    {
        if(strcmp(ptr->name, name) == 0 && 
          (ptr->type == STRINGARRAY || ptr->type == INTARRAY))
        {
            return(ptr->length);
        }
        else if(strcmp(ptr->name, name) == 0 &&
               (ptr->type == TWO_DSTRINGARRAY || ptr->type == TWO_DINTARRAY))
        {
            if(x == NULL || y == NULL)
            {
                return(-2);
            }
            *x = ptr->x_length;
            *y = ptr->y_length;
            return(0);
        }
        ptr = ptr->next;
    }
    return(-1);
}

int deleteVar(struct variables *anker, char *name)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr != NULL)
    {
        if(ptr->next == NULL)
        {
            break;
        }
        if(cmp(ptr->next->name, name))
        {
            hptr = ptr->next;
            break;
        }
        ptr = ptr->next;
    }

    if(ptr->next == NULL)
        return(-1);

    ptr->next = hptr->next;
    free(hptr->data);
    free(hptr);
    return(0);
}



struct variables* createTmpArrayOut2DArray(struct variables *anker, char *name,
                                           int x)
{
    struct variables *ptr, *hptr;
    int y;

    ptr = malloc(sizeof(struct variables));

    hptr = anker;
    while(hptr != NULL)
    {
        if(cmp(hptr->name, name))
            break;
        hptr = hptr->next;
    }

    if(hptr == NULL)
        return(NULL);

    if(x >= hptr->x_length)
    {
        return(NULL);
    }

    if(hptr->type == TWO_DSTRINGARRAY)
    {
        ptr->type = STRINGARRAY;
        ptr->length = hptr->y_length;
        ptr->data = malloc(sizeof(char*)*hptr->y_length);

        for(y=0; y < hptr->y_length; y++)
        {
            ((char**)ptr->data)[y] = malloc(
                               strlen(((char**)hptr->data)[hptr->x_length*x+y]));
            strcpy(((char**)ptr->data)[y],
                    ((char**)hptr->data)[hptr->x_length*x+y]);
        }
    }
    else if(hptr->type == TWO_DINTARRAY)
    {
        ptr->type = INTARRAY;
        ptr->length = hptr->y_length;
        ptr->data = malloc(sizeof(int)*hptr->y_length);

        for(y=0; y < hptr->y_length; y++)
        {
            ((int*)ptr->data)[y] =((int*)hptr->data)[hptr->x_length*x+y];
        }
    }

    return(ptr);
}
