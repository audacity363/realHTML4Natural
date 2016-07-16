#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "management.h"

char* getStringValue(struct variables *anker, char *name)
{
    struct variables *ptr;
    char *hptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        hptr = malloc(sizeof(""));
        strcpy(hptr, "");
        return(hptr);
    }
    return((char*)ptr->data);
}

void newStringVar(struct variables *anker, char *name, char *value)
{
    struct variables *ptr, *hptr;
    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    hptr = malloc(sizeof(struct variables));
    strcpy(hptr->name, name);
    hptr->data = malloc(sizeof(char)*strlen(value));
    strcpy((char*)hptr->data, value);
    hptr->type = STRING;
    hptr->next = NULL;
    ptr->next = hptr;
    return;
}


void newStringArray(struct variables *anker, char *name, char *initvalue)
{
    struct variables *ptr, *hptr;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    hptr = malloc(sizeof(struct variables));
    hptr->type = STRINGARRAY;
    strcpy(hptr->name, name);
    hptr->length = 1;

    hptr->data = malloc(sizeof(char*));
    ((char**)hptr->data)[0] = malloc(sizeof(char)*500);

    strcpy(((char**)hptr->data)[0], initvalue);

    hptr->next = NULL;

    ptr->next = hptr;
}

void new2DStringArray(struct variables *anker, char *name, int x_length, 
                      int y_length)
{
    struct variables *ptr, *hptr;
    int x, y;

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    hptr = malloc(sizeof(struct variables));
    hptr->type = TWO_DSTRINGARRAY;
    hptr->y_length = y_length;
    hptr->x_length = x_length;

    strcpy(hptr->name, name);

    hptr->data = malloc(sizeof(char*)*(x_length*y_length));

    for(x=0; x < x_length; x++)
    {
        for(y=0; y < y_length; y++)
        {
            ((char**)hptr->data)[x_length*x+y] = 
                    malloc(MAXSTRINGLENGTH*sizeof(char));
        }
    }

    hptr->next = NULL;
    ptr->next = hptr;
}

void appendStringArray(struct variables *anker, char *name, char *value)
{
    struct variables *ptr;
    int length;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return;
    }

    ptr->length++;
    length = ptr->length;
    ptr->data = realloc(ptr->data, sizeof(char*)*length);

    ((char**)ptr->data)[length-1] = malloc(sizeof(char)*MAXSTRINGLENGTH);

    strcpy(((char**)ptr->data)[length-1], value);
}

char *getStringValuefromArray(struct variables *anker, char *name, int index)
{
    struct variables *ptr;
    char *bla = malloc(sizeof(""));
    strcpy(bla, "");

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(bla);
    }

    if(index >= ptr->length)
    {
        return(bla);
    }

    return(((char**)ptr->data)[index]);
}

char *getStringValuefrom2DArray(struct variables *anker, char *name, int x, 
                                int y)
{
    struct variables *ptr;
    char *bla = malloc(sizeof(""));
    strcpy(bla, "");

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(bla);
    } 

    if(x >= ptr->x_length || y >= ptr->y_length)
        return(bla);

    return(((char**)ptr->data)[ptr->x_length*x+y]);
}

int editStringVarArray(struct variables *anker, char *name, char *value, int x)
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

    strcpy(((char**)ptr->data)[x], value);
    return(0);

}

int editStringVar2DArray(struct variables *anker, char *name, char *value,
                         int x, int y)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(-1);
    }

    if(x >= ptr->x_length || y >= ptr->y_length)
        return(-1);

    strcpy(((char**)ptr->data)[ptr->x_length*x+y], value);
    return(0);
}

int editStringVar(struct variables *anker, char *name, char *value)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        return(-1);
    }

   if(ptr->type != STRING)
       //Wrong Var Type
       return(-2);

    free(ptr->data);
    ptr->data = malloc(sizeof(char)*strlen(value));
    strcpy((char*)ptr->data, value);
    return(0);
}
