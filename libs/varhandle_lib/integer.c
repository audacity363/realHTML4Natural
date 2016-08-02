#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "management.h"

int getIntValue(struct variables *anker, char *name)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    { 
        return(-1);
    }

    return(*(int*)ptr->data);
}

int getIntValuefromArray(struct variables *anker, char *name, int index)
{
    struct variables *ptr;
    ptr = anker;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(-1);
    }

    if(index >= ptr->length)
    {
        return(-2);
    }

    return(((int*)ptr->data)[index]);
}

int getIntValuefrom2DArray(struct variables *anker, char *name, int x, 
                                int y, int *value)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Unkown Variable [%s]", name);
        return(-1);
    }

    if(x >= ptr->x_length || y >= ptr->y_length)
    {
        strcpy(varhandle_error_str, "Index Error: index out of range");
        return(-2);
    }

    *value = ((int*)ptr->data)[ptr->x_length*x+y];

    return(0);
}

void newIntVar(struct variables *anker, char *name, int value)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    hptr = malloc(sizeof(struct variables));
    strcpy(hptr->name, name);

    hptr->data = malloc(sizeof(int));
    *(int*)hptr->data = value;
    hptr->type = INT;
    hptr->next = NULL;

    ptr->next = hptr;
    return;
}

void newIntArray(struct variables *anker, char *name, int initvalue)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    hptr = malloc(sizeof(struct variables));
    if(hptr == NULL)
    {
        printf("Can not allocate Memory (newIntArray)\n");
        return;
    }
    hptr->type = INTARRAY;
    hptr->length = 1;
    strcpy(hptr->name, name);

    hptr->data = malloc(sizeof(int)*hptr->length);
    *(int*)(hptr->data+(sizeof(int)*(hptr->length-1))) = initvalue;

    hptr->next = NULL;

    ptr->next = hptr;
    return;
}

void new2DIntArray(struct variables *anker, char *name, int x_length,
                   int y_length)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    hptr = malloc(sizeof(struct variables));

    strcpy(hptr->name, name);

    hptr->type = TWO_DINTARRAY;
    hptr->x_length = x_length;
    hptr->y_length = y_length;

    hptr->data = malloc((x_length*y_length)*sizeof(int));

    hptr->next = NULL;

    ptr->next = hptr;
}

void appendIntArray(struct variables *anker, char *name, int value)
{
    struct variables *ptr;
    int length;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return;
    }

    ptr->length++;
    length = ptr->length;

    ptr->data = realloc(ptr->data, sizeof(int)*length);
    *(int*)(ptr->data+(sizeof(int)*(length-1))) = value;

    return;
}

int editIntVar(struct variables *anker, char *name, int value)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(-1);
    }

   if(ptr->type != INT)
       //Wrong Var Type
       return(-2);

    free(ptr->data);
    ptr->data = malloc(sizeof(int));
    *(int*)ptr->data = value;
    return(0);
}

int editIntVarArray(struct variables *anker, char *name, int value, int x)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(-1);
    }

    if(x >= ptr->length)
    {
        return(-2);
    }

    ((int*)ptr->data)[x] = value;
    return(0);
}


int editIntVar2DArray(struct variables *anker, char *name, int value, int x,
                      int y)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(-1);
    }

    if(ptr->type != TWO_DINTARRAY)
        return(-1);

    if(x >= ptr->x_length || y >= ptr->y_length)
        return(-1);

    ((int*)ptr->data)[ptr->x_length*x+y] = value;
    return(0);
}
