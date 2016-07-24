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

int getArrayLength(struct variables *anker, char *name, int *x, int *y)
{
    struct variables *ptr;
   
    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name) && 
          (ptr->type == STRINGARRAY || ptr->type == INTARRAY))
        {
            return(ptr->length);
        }
        else if(cmp(ptr->name, name) &&
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
            ptr = NULL;
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
