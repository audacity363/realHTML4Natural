#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "varhandle.h"
#include "management.h"

int newUStringVar(struct variables *anker, char *name, wchar_t *value, int length)
{
    struct variables *ptr, *hptr;


    if(wcslen(value) == 0)
    {
        strcpy(varhandle_error_str, "Invalid Variable Length");
        return(-1);
    }

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    if((hptr = malloc(sizeof(struct variables))) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        return(-2);
    }

    strcpy(hptr->name, name);
    hptr->type = U_STRING;
    hptr->next = NULL;
    if((hptr->data = malloc((length+1)*sizeof(wchar_t))) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        free(hptr);
        return(-3);
    }
    bzero(hptr->data, (length+1)*sizeof(wchar_t));
    hptr->length = length;
    
    wcscpy((wchar_t*)hptr->data, value);

    ptr->next = hptr;

    return(0);
}

int editUStringVar(struct variables *anker, char *name, wchar_t *value)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%d] not found", name);
        return(-1);
    }

    if(wcslen(value) > ptr->length)
    {
        strcpy(varhandle_error_str, "Buffer to small");
        return(-2);
    }

    wcscpy((wchar_t*)ptr->data, value);
}

wchar_t* getUStringVal(struct variables *anker, char *name)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%d] not found", name);
        return(NULL);
    }

    return((wchar_t*)ptr->data);
}

int newUStringArray(struct variables *anker, char *name, int var_length,
                    int array_length)
{
    struct variables *ptr, *hptr;

    if(var_length < 1)
    {
        strcpy(varhandle_error_str, "Invalid Variable Length");
        return(-1);
    }

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    if((hptr = malloc(sizeof(struct variables))) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        return(-2);
    }

    strcpy(hptr->name, name);

    hptr->type = U_STRINGARRAY;
    hptr->length = var_length;
    hptr->x_length = array_length;
    hptr->next = NULL;

    if((hptr->data = malloc(((var_length+4)*sizeof(wchar_t))*array_length)) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        return(-3);
    }

    bzero(hptr->data, ((var_length+4)*sizeof(wchar_t))*array_length);

    ptr->next = hptr;
    return(0);
}

int editUStringArray(struct variables *anker, char *name, wchar_t *value, int x)
{
    struct variables *ptr, *hptr;

    if(wcslen(value) < 1)
    {
        strcpy(varhandle_error_str, "Invalid Variable Length");
        return(-1);
    }

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%s] not found", name);
        return(-2);
    }

    if(ptr->type != U_STRINGARRAY)
    {
        sprintf(varhandle_error_str, "[%d] is not an Unicode Array", name);
        return(-3);
    }
    if(ptr->x_length < x)
    {
        sprintf(varhandle_error_str, "[%s]: Index out of range", name);
        return(-4);
    }
    if(wcslen(value) >= ptr->length)
    {
        sprintf(varhandle_error_str, "Value to long for buffer");
        return(-5);
    }

    wcscpy((wchar_t*)(ptr->data+((ptr->length+4)*sizeof(wchar_t)*x)), value);
    return(0);
}

wchar_t *getUStringArrayVal(struct variables *anker, char *name, int x)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%s] not found", name);
        return(NULL);
    }

    return((wchar_t*)(ptr->data+((ptr->length+4)*sizeof(wchar_t)*x)));
}

int newU2DStringArray(struct variables *anker, char *name, int var_length,
                    int x_length, int y_length)
{
    struct variables *ptr, *hptr;
    int size;

    if(var_length < 1)
    {
        strcpy(varhandle_error_str, "Invalid Variable Length");
        return(-1);
    }

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    if((hptr = malloc(sizeof(struct variables))) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        return(-2);
    }

    strcpy(hptr->name, name);

    hptr->type = U_TWO_DSTRINGARRAY;
    hptr->length = var_length;
    hptr->x_length = x_length;
    hptr->y_length = y_length;
    hptr->next = NULL;

    size = (((var_length+4)*sizeof(wchar_t))*y_length)*x_length;

    if((hptr->data = malloc(size)) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        return(-3);
    }

    bzero(hptr->data, size);

    ptr->next = hptr;
    return(0);
}

int editU2DStringArray(struct variables *anker, char *name, wchar_t *value,
                       int x, int y)
{
    struct variables *ptr, *hptr;
    int offset;

    if(wcslen(value) < 1)
    {
        strcpy(varhandle_error_str, "Invalid Variable Length");
        return(-1);
    }

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%s] not found", name);
        return(-2);
    }

    if(ptr->type != U_TWO_DSTRINGARRAY)
    {
        sprintf(varhandle_error_str, "[%d] is not an Unicode Array", name);
        return(-3);
    }
    if(ptr->x_length < x || ptr->y_length < y)
    {
        sprintf(varhandle_error_str, "[%s]: Index out of range", name);
        return(-4);
    }
    if(wcslen(value) >= ptr->length)
    {
        sprintf(varhandle_error_str, "Value to long for buffer");
        return(-5);
    }

    offset = ((ptr->y_length*(ptr->length*sizeof(wchar_t)))*(x))+((ptr->length*sizeof(wchar_t))*y);

    wcscpy((wchar_t*)(ptr->data+offset), value);
    return(0);
}

wchar_t *getUString2DArrayVal(struct variables *anker, char *name, int x, int y)
{
    struct variables *ptr, *hptr;
    int offset;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%s] not found", name);
        return(NULL);
    }

    if(ptr->type != U_TWO_DSTRINGARRAY)
    {
        sprintf(varhandle_error_str, "[%d] is not an Unicode Array", name);
        return(NULL);
    }
    if(ptr->x_length < x || ptr->y_length < y)
    {
        sprintf(varhandle_error_str, "[%s]: Index out of range", name);
        return(NULL);
    }

    offset = ((ptr->y_length*(ptr->length*sizeof(wchar_t)))*(x))+((ptr->length*sizeof(wchar_t))*y);
    return((wchar_t*)(ptr->data+offset));

}

int newU3DStringArray(struct variables *anker, char *name, int var_length,
                      int x_length, int y_length, int z_length)
{
    struct variables *ptr, *hptr;
    int size;

    if(var_length < 1)
    {
        strcpy(varhandle_error_str, "Invalid Variable Length");
        return(-1);
    }

    ptr = anker;
    while(ptr->next != NULL)
        ptr = ptr->next;

    if((hptr = malloc(sizeof(struct variables))) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        return(-2);
    }

    strcpy(hptr->name, name);

    hptr->type = U_THREE_DSTRINGARRAY;
    hptr->length = var_length;
    hptr->x_length = x_length;
    hptr->y_length = y_length;
    hptr->z_length = z_length;

    hptr->next = NULL;

    size = ((((var_length+4)*sizeof(wchar_t))*y_length)*x_length)*z_length;

    if((hptr->data = malloc(size)) == NULL)
    {
        strcpy(varhandle_error_str, "Can not allocate Memory");
        return(-3);
    }

    bzero(hptr->data, size);

    ptr->next = hptr;
    return(0);
}

int editU3DStringArray(struct variables *anker, char *name, wchar_t *value, int x,
                      int y, int z)
{
    struct variables *ptr, *hptr;
    int offset, new_var_length;

    if(wcslen(value) < 1)
    {
        strcpy(varhandle_error_str, "Invalid Variable Length");
        return(-1);
    }

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%s] not found", name);
        return(-2);
    }

    if(ptr->type != U_THREE_DSTRINGARRAY)
    {
        sprintf(varhandle_error_str, "[%d] is not an Unicode Array", name);
        return(-3);
    }
    if(ptr->x_length < x || ptr->y_length < y || ptr->z_length < z)
    {
        sprintf(varhandle_error_str, "[%s]: Index out of range", name);
        return(-4);
    }
    if(wcslen(value) >= ptr->length)
    {
        sprintf(varhandle_error_str, "Value to long for buffer");
        return(-5);
    }

    new_var_length = ptr->length*sizeof(wchar_t);

    offset = (((new_var_length*ptr->z_length)*ptr->y_length)*x)+((new_var_length*ptr->z_length)*y)+(new_var_length*z);

    wcscpy((wchar_t*)(ptr->data+offset), value);
    return(0);
}

wchar_t *getUString3DArrayVal(struct variables *anker, char *name, int x, int y, int z)
{
    struct variables *ptr;
    int offset, new_var_length;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Variable [%s] not found", name);
        return(NULL);
    }

    if(ptr->type != U_THREE_DSTRINGARRAY)
    {
        sprintf(varhandle_error_str, "[%d] is not an Unicode Array", name);
        return(NULL);
    }
    if(ptr->x_length < x || ptr->y_length < y || ptr->z_length < z)
    {
        sprintf(varhandle_error_str, "[%s]: Index out of range", name);
        return(NULL);
    }

    new_var_length = ptr->length*sizeof(wchar_t);

    offset = (((new_var_length*ptr->z_length)*ptr->y_length)*x)+((new_var_length*ptr->z_length)*y)+(new_var_length*z);
    
    return((wchar_t*)(ptr->data+offset));
}
