#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "management.h"

int newIntVar(struct variables *anker, char *name, int value)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    if((hptr = malloc(sizeof(struct variables))) == NULL)
    {
        strcpy(varhandle_error_str, "MemoryError: Can not allocate Memory for variable Entry (newIntVar)");
        return(-1);
    }

    strcpy(hptr->name, name);

    if((hptr->data = malloc(sizeof(int))) == NULL)
    {
        strcpy(varhandle_error_str, "MemoryError: Can not allocate Memory for variable (newIntVar)");
        return(-2);
    }
    *(int*)hptr->data = value;
    hptr->type = INT;

    hptr->length = sizeof(int);
    hptr->x_length = 0;
    hptr->y_length = 0;

    hptr->next = NULL;

    ptr->next = hptr;
    return(0);
}

int getIntValue(struct variables *anker, char *name, int *value)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    { 
        sprintf(varhandle_error_str, "Unkown Variable [%s]", name);
        value = NULL;
        return(-1);
    }

   if(ptr->type != INT)
   {
        sprintf(varhandle_error_str, "TypeError: variable [%s] is not an integer", name);
       return(-2);
   }

    *value = *(int*)ptr->data;
    return(0);
}

int editIntVar(struct variables *anker, char *name, int value)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Unkown Variable [%s]", name);
        return(-1);
    }

   if(ptr->type != INT)
   {
        sprintf(varhandle_error_str, "TypeError: variable [%s] is not an integer", name);
       return(-2);
   }

    *(int*)ptr->data = value;
    return(0);
}



//---------------------------------------------------------------------------

int newIntArray(struct variables *anker, char *name, int x_length)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    if((hptr = malloc(sizeof(struct variables))) == NULL)
    {
        strcpy(varhandle_error_str, "MemoryError: Can not allocate Memory for variable Entry (newIntArray)");
        return(-1);
    }

    hptr->type = INTARRAY;

    hptr->length = sizeof(int);
    hptr->x_length = x_length;
    hptr->y_length = 0;

    strcpy(hptr->name, name);

    if((hptr->data = malloc(sizeof(int)*x_length)) == NULL)
    {
        strcpy(varhandle_error_str, "MemoryError: Can not allocate Memory for variable (newIntArray)");
        return(-2);
    }

    hptr->next = NULL;

    ptr->next = hptr;
    return(0);
}

int getIntValuefromArray(struct variables *anker, char *name, int x_index, int *value)
{
    struct variables *ptr;
    ptr = anker;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Unkown Variable [%s]", name);
        return(-1);
    }

    if(ptr->type != INTARRAY)
    {
        sprintf(varhandle_error_str, "TypeError: variable [%s] is not an integer array", name);
        return(-2);
    }

    if(x_index >= ptr->x_length)
    {
        sprintf(varhandle_error_str, "IndexError: Index out of range");
        return(-3);
    }

    *value =((int*)(ptr->data))[x_index];

    return(0);
}

int editIntVarArray(struct variables *anker, char *name, int value, int x_index)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Unkown Variable [%s]", name);
        return(-1);
    }

    if(ptr->type != INTARRAY)
    {
        sprintf(varhandle_error_str, "TypeError: variable [%s] is not an integer array", name);
        return(-2);
    }

    if(x_index >= ptr->x_length)
    {
        sprintf(varhandle_error_str, "IndexError: Index out of range");
        return(-2);
    }

    ((int*)ptr->data)[x_index] = value;
    return(0);
}

//---------------------------------------------------------------------------

int new2DIntArray(struct variables *anker, char *name, int x_length,
                   int y_length)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    if((hptr = malloc(sizeof(struct variables))) == NULL)
    {
        strcpy(varhandle_error_str, "MemoryError: Can not allocate Memory for variable Entry (new2DIntArray)");
        return(-1);
    }

    strcpy(hptr->name, name);

    hptr->type = TWO_DINTARRAY;

    hptr->length = sizeof(int);
    hptr->x_length = x_length;
    hptr->y_length = y_length;

    if((hptr->data = malloc((x_length*y_length)*sizeof(int))) == NULL)
    {
        strcpy(varhandle_error_str, "MemoryError: Can not allocate Memory for variable (new2DIntArray)");
        return(-2);
    }

    hptr->next = NULL;

    ptr->next = hptr;
    return(0);
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

int editIntVar2DArray(struct variables *anker, char *name, int value, int x,
                      int y)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Unkown Variable [%s]", name);
        return(-1);
    }

    if(ptr->type != TWO_DINTARRAY)
    {
        sprintf(varhandle_error_str, "TypeError: Varaible [%s] is not a two dimension integer array", name);
        return(-2);
    }

    if(x >= ptr->x_length || y >= ptr->y_length)
    {
        strcpy(varhandle_error_str, "Index Error: index out of range");
        return(-3);
    }

    ((int*)ptr->data)[ptr->x_length*x+y] = value;
    return(0);
}

//---------------------------------------------------------------------------
