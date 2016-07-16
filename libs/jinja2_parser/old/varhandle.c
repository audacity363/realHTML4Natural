#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/standard.h"
#include "parser.h"

void initVariable(struct variables *anker)
{
    anker = malloc(sizeof(struct variables));
}

char* getStringValue(struct variables *anker, char *name)
{
    struct variables *ptr;
    char *hptr;

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
        {
            return((char*)ptr->data);
        }
        ptr = ptr->next;
    }

    hptr = malloc(sizeof(""));
    strcpy(hptr, "");
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

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
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

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
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

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
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
    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
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
    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
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

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
        return(-1);

   if(ptr->type != STRING)
       //Wrong Var Type
       return(-2);

    free(ptr->data);
    ptr->data = malloc(sizeof(char)*strlen(value));
    strcpy((char*)ptr->data, value);
    return(0);
}

int getIntValue(struct variables *anker, char *name)
{
    struct variables *ptr;

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
        {
            return(*(int*)ptr->data);
        }
        ptr = ptr->next;
    }
    return(NULL);
}

int getIntValuefromArray(struct variables *anker, char *name, int index)
{
    struct variables *ptr;
    char *bla = malloc(sizeof(""));
    strcpy(bla, "");

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
    {
        return(-1);
    } 

    if(index >= ptr->length)
    {
        return(-1);
    }

    return(((int*)ptr->data)[index]);
}

int getIntValuefrom2DArray(struct variables *anker, char *name, int x, 
                                int y, int *value)
{
    struct variables *ptr;

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
    {
        return(-1);
    }

    if(x >= ptr->x_length || y >= ptr->y_length)
        return(-2);

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

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
        {
            break;
        }
        ptr = ptr->next;
    }

    if(ptr == NULL)
    {
        return;
    }
    ptr->length++;
    length = ptr->length;
    ptr->data = realloc(ptr->data, sizeof(int)*length);
    *(int*)(ptr->data+sizeof(int)+length-1) = value;
    return;
}

int editIntVar(struct variables *anker, char *name, int value)
{
    struct variables *ptr;

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
        return(-1);

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

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
        return(-1);

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

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            break;
        ptr = ptr->next;
    }

    if(ptr == NULL)
        return(-1);

    if(ptr->type != TWO_DINTARRAY)
        return(-1);

    if(x >= ptr->x_length || y >= ptr->y_length)
        return(-1);

    ((int*)ptr->data)[ptr->x_length*x+y] = value;
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

void printArrayfromPtr(struct variables *ptr)
{
    int x;

    printf("[");
    for(x=0; x < ptr->length; x++)
    {
        if(ptr->type == STRINGARRAY)
            printf("\"%s\"", ((char**)ptr->data)[x]);
        else
            printf("%d", ((int*)ptr->data)[x]);
        if(x+1 < ptr->length)
            printf(",");
    }
    printf("]");
}

void printArray(struct variables *anker, char *name, bool showname, 
                    char *returnstring)
{
    struct variables *ptr;
    int i;
    char buf[1000];

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name) && ptr->type == STRINGARRAY)
        {
            if(showname == true)
                printf("Name: [%s] = ", name);

            if(returnstring == NULL)
                printf("[");
            else
            {
                sprintf(returnstring, "[");
            }

            for(i=0; i < ptr->length; i++)
            {
                if(returnstring == NULL)
                    printf("\"%s\"", ((char**)ptr->data)[i]);
                else
                {
                    sprintf(buf, "\"%s\"", ((char**)ptr->data)[i]);
                    strcat(returnstring, buf);
                }
                if(i+1 < ptr->length)
                {
                    if(returnstring == NULL)
                        printf(",");
                    else
                    {
                        strcat(returnstring, ",");
                    }
                }
            }

            if(returnstring == NULL)
                printf("]");
            else
            {
                strcat(returnstring, "]");
            }
            if(showname == true)
                printf("\n");

        }
        else if(cmp(ptr->name, name) && ptr->type == INTARRAY)
        {
            if(showname == true)
                printf("Name: [%s] = ", name);

            printf("[");
            for(i=0; i < ptr->length; i++)
            {
                printf("%d", *(int*)ptr->data+(sizeof(int)*i));
                if(i+1 < ptr->length)
                {
                    printf(",");
                }
            }
            printf("]");
            if(showname == true)
                printf("\n");

        }
        ptr = ptr->next;
    }
}

struct variables* getVarPointer(struct variables *anker, char* name)
{
    struct variables *ptr;

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            return(ptr);
        ptr = ptr->next;
    }
    return(NULL);
}

void print2DArray(struct variables *anker, char *name, bool rt,
                    char *returnstring)
{
    int x, y;
    struct variables *ptr;
    char buf[1000];

    if((ptr = getVarPointer(anker, name)) == NULL)
    {
        fprintf(stderr, "Var not found\n");
        return;
    }

    if(rt == true)
        printf("Name: [%s] = ", ptr->name);

    printf("%s[%s", rt==true?"\n":"", rt==true?"\n":"");
    for(x=0; x < ptr->x_length; x++)
    {  
        if(returnstring == NULL)
            printf("%s[", rt==true?"\t":"");
        else
            sprintf("%s[", rt==true?"\t":"");

        for(y=0; y < ptr->y_length; y++)
        {
            if(ptr->type == TWO_DSTRINGARRAY)
            {
                if(returnstring == NULL)
                    printf("\"%s\"", ((char**)ptr->data)[ptr->x_length*x+y]);
                else
                {
                    sprintf(buf, "\"%s\"",
                        ((char**)ptr->data)[ptr->x_length*x+y]);
                    strcat(returnstring, buf);
                }
            }
            else if(ptr->type == TWO_DINTARRAY)
            {

                if(returnstring == NULL)
                    printf("%d", ((int*)ptr->data)[ptr->x_length*x+y]);
                 else
                 {
                    sprintf(buf, "%d", ((int*)ptr->data)[ptr->x_length*x+y]);
                    strcat(returnstring, buf);
                 }
            }

            if(y+1 < ptr->y_length)
            {
                if(returnstring == NULL)
                    printf(", ");
                else
                    strcat(returnstring, ", ");
            }
        }
        if(returnstring == NULL)
            printf("],%s", rt==true?"\n":"");
        else
            strcat(returnstring, "],");
    }
    if(returnstring == NULL)
        printf("]%s",rt==true?"\n":"");
    else
        strcat(returnstring, "]");
}


int getVarType(struct variables *anker, char *name)
{
    struct variables *ptr;

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
        {
            return(ptr->type);
        }
        ptr = ptr->next;
    }
    fprintf(stderr, "Unknown Variable: [%s]\n", name);
    return(-1);
}

void printVars(struct variables *anker)
{
    struct variables *ptr;

    printf("--------------------------------------------------------------\n");

    ptr = anker;
    while(ptr != NULL)
    {
        switch(ptr->type)
        {
            case STRING:
                printf("Name:[%s] = [%s]\n", ptr->name, (char*)ptr->data);
                break;
            case INT:
                printf("Name:[%s] = [%d]\n", ptr->name, *(int*)ptr->data);
                break;
            case STRINGARRAY:
                printArray(anker, ptr->name, true, NULL);
                break;
            case INTARRAY:
                printArray(anker, ptr->name, true, NULL);
                break;
            case TWO_DSTRINGARRAY:
                print2DArray(anker, ptr->name, true, NULL);
                break;
            case TWO_DINTARRAY:
                print2DArray(anker, ptr->name, true, NULL);
                break;


        }
        ptr = ptr->next;
    }
    printf("--------------------------------------------------------------\n");
    return;
}

void printVar(struct args word)
{
    if(word.type == STRING)
        printf("%s", word.cargs);
    else if(word.type == INT)
        printf("%d", word.iargs);
    else
        printf("kenne ich nicht\n");
}
