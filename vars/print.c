#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "print.h"

bool last_entry = false;

void printAllVarsToFile_in(vars_t *anker, FILE *fp, int offset, bool);
void printInteger(vars_t *var, FILE *fp, bool);
void print1DInteger(vars_t *var, FILE *fp, bool);
void print2DInteger(vars_t *var, FILE *fp, bool);
void print3DInteger(vars_t *var, FILE *fp, bool);
void printsingleBoolean(bool val, FILE *fp, bool);
void printBoolean(vars_t *var, FILE *fp, bool);
void print1DBoolean(vars_t *var, FILE *fp, bool);
void print2DBoolean(vars_t *var, FILE *fp, bool);
void print3DBoolean(vars_t *var, FILE *fp, bool);
void print1DFloat(vars_t *var, FILE *fp, bool);
void print2DFloat(vars_t *var, FILE *fp, bool);
void print3DFloat(vars_t *var, FILE *fp, bool);
void printFloat(vars_t *var, FILE *fp, bool);
void printString(vars_t *var, FILE *fp, bool);
void print1DString(vars_t *var, FILE *fp, bool);
void print2DString(vars_t *var, FILE *fp, bool);
void print3DString(vars_t *var, FILE *fp, bool);
void printGroup(vars_t *var, FILE *fp, bool);

#define SIZEOF_PRINT_FUNCS 17

int print_function_dic_i[SIZEOF_PRINT_FUNCS] = {
        INTEGER,
        GROUP,
        ONEDINTEGER,
        TWODINTEGER,
        THREEDINTEGER,
        STRING,
        BOOL,
        ONEDBOOL,
        TWODBOOL,
        THREEDBOOL,
        FLOAT,
        ONEDFLOAT,
        TWODFLOAT,
        THREEDFLOAT,
        ONEDSTRING,
        TWODSTRING,
        THREEDSTRING
    };

void (*print_function_dic_v[SIZEOF_PRINT_FUNCS])(vars_t*, FILE*, bool) = {
        printInteger,
        printGroup,
        print1DInteger,
        print2DInteger,
        print3DInteger,
        printString,
        printBoolean,
        print1DBoolean,
        print2DBoolean,
        print3DBoolean,
        printFloat,
        print1DFloat,
        print2DFloat,
        print3DFloat,
        print1DString,
        print2DString,
        print3DString
    };

void printAllVars(vars_t *anker)
{
    printAllVarsToFile(anker, stdout);
}

void printAllVarsToFile(vars_t *anker, FILE *fp)
{
    printAllVarsToFile_in(anker, fp, 0, false);
}

void printAllVarsJSON(vars_t *anker)
{
    printAllVarsToFileJSON(anker, stdout);
}

void printAllVarsToFileJSON(vars_t *anker, FILE *fp)
{
    fprintf(fp, "{\n");
    printAllVarsToFile_in(anker, fp, 0, true);
    fprintf(fp, "}\n");
}

int printVarsToFileJSON(vars_t *anker, char **var_names, int length, FILE *fp)
{
    int i = 0, x = 0;
    vars_t *target = NULL;
    void (*print_func)(vars_t*, FILE*, bool) = NULL;

    fprintf(fp, "{");
    for(; x < length; x++)
    {
        if(!(target = isDefined(anker, var_names[x])))
            return(VAR_NOT_DEFINED);
        for(i=0; i < SIZEOF_PRINT_FUNCS; i++)
        {
            if(print_function_dic_i[i] == target->type)
            {
                print_func = print_function_dic_v[i];
                print_func(target, fp, true);
            }
        }
        if(x+1 < length)
            fprintf(fp, ",");
    }
    fprintf(fp, "}\n");
    return(0);
}

void printAllVarsToFile_in(vars_t *anker, FILE *fp, int offset, bool json)
{
    int i = 0, x = 0;
    vars_t *hptr = NULL;

    if(anker->name == NULL)
        hptr = anker->next;
    else
        hptr = anker;

    void (*print_func)(vars_t*, FILE*, bool) = NULL;

    while(hptr)
    {

        for(i=0; i < SIZEOF_PRINT_FUNCS; i++)
        {
            if(print_function_dic_i[i] == hptr->type)
            {
                for(x=0; x < offset; x++)
                    printf("\t");
                print_func = print_function_dic_v[i];
                print_func(hptr, fp, json);
            }
        }
        //Don't know why the NULL macro does not work here???
        if(hptr->next != 0x00 && json)
            fprintf(fp, ",");
        else if(!json)
            fprintf(fp, "\n");

        hptr = hptr->next;
    }
}


void printInteger(vars_t *var, FILE *fp, bool json)
{
    if(json)
        fprintf(fp, "\"%s\": %d", var->name, *((int*)var->data));
    else
        fprintf(fp, "[%s] = [%d]", var->name, *((int*)var->data));
}

void print1DInteger(vars_t *var, FILE *fp, bool json)
{
    int i = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);
    for(i=0; i < var->x_length; i++)
    {
        fprintf(fp, "%d", ((int*)var->data)[i]);
        if(i+1 < var->x_length)
        {
            fprintf(fp, ", ");
        }
    }
    fprintf(fp, "]");
}

void print2DInteger(vars_t *var, FILE *fp, bool json)
{
    int x = 0, y = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);
    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "%d", ((int*)var->data)[(x*var->y_length)+y]);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DInteger(vars_t *var, FILE *fp, bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                offset = (z*var->x_length * var->y_length);
                offset += (y*var->x_length) + x;
                fprintf(fp, "%d", ((int*)var->data)[offset]);
                if(z+1 < var->z_length)
                    fprintf(fp, ", ");
            }
            fprintf(fp, "]");
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }

        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void printsingleBoolean(bool val, FILE *fp, bool json)
{
    if(val == 1)
        fprintf(fp, "true");
    else
        fprintf(fp, "false");
}

void printBoolean(vars_t *var, FILE *fp, bool json)
{
    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    printsingleBoolean(*((bool*)var->data), fp, json);
    fprintf(fp, "]");
}

void print1DBoolean(vars_t *var, FILE *fp, bool json)
{
    int x = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        printsingleBoolean(((bool*)var->data)[x], fp, json);
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print2DBoolean(vars_t *var, FILE *fp, bool json)
{
    size_t x = 0, y = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            offset = (x*var->y_length)+y;
            printsingleBoolean(((bool*)var->data)[offset], fp, json);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DBoolean(vars_t *var, FILE *fp, bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                offset = (z*var->x_length * var->y_length);
                offset += (y*var->x_length) + x;
                printsingleBoolean(((bool*)var->data)[offset], fp, json);
                if(z+1 < var->z_length)
                   fprintf(fp, ", ");
            }
            fprintf(fp, "]");

            if(y+1 < var->y_length)
            fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void printFloat(vars_t *var, FILE *fp, bool json)
{
    if(json)
        fprintf(fp, "\"%s\": %f", var->name, *((double*)var->data));
    else
        fprintf(fp, "[%s] = [%f]", var->name, *((double*)var->data));
}

void print1DFloat(vars_t *var, FILE *fp, bool json)
{
    int i = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(i=0; i < var->x_length; i++)
    {
        fprintf(fp, "%f", ((double*)var->data)[i]);
        if(i+1 < var->x_length)
        {
            fprintf(fp, ", ");
        }
    }
    fprintf(fp, "]");
}

void print2DFloat(vars_t *var, FILE *fp, bool json)
{
    int x = 0, y = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "%f", ((double*)var->data)[(x*var->y_length)+y]);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DFloat(vars_t *var, FILE *fp, bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                offset = (z*var->x_length * var->y_length);
                offset += (y*var->x_length) + x;
                fprintf(fp, "%f", ((double*)var->data)[offset]);
                if(z+1 < var->z_length)
                    fprintf(fp, ", ");
            }
            fprintf(fp, "]");
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }

        fprintf(fp, "]");

        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}



void printString(vars_t *var, FILE *fp, bool json)
{
    if(json)
        fprintf(fp, "\"%s\": \"%S\"", var->name, (wchar_t*)var->data);
    else
        fprintf(fp, "[%s] = [%S]", var->name, (wchar_t*)var->data);
}

void print1DString(vars_t *var, FILE *fp, bool json)
{
    size_t x = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        offset = ((var->length)*sizeof(wchar_t))*x;
        fprintf(fp, "\"%S\"", (wchar_t*)(var->data+offset));
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print2DString(vars_t *var, FILE *fp, bool json)
{
    size_t x = 0, y = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            offset = (var->y_length*(var->length*sizeof(wchar_t)));
            offset = offset*(x)+((var->length*sizeof(wchar_t))*(y));
            fprintf(fp, "\"%S\"", (wchar_t*)(var->data+offset));
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DString(vars_t *var, FILE *fp, bool json)
{
    size_t x = 0, y = 0, z = 0, 
        offset = 0, var_size = 0,
        sizeofz = 0, sizeofy = 0;

    var_size = var->length*sizeof(wchar_t);
    sizeofz = var_size*(var->z_length);
    sizeofy = sizeofz*(var->y_length);

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            fprintf(fp, "[");
            for(z=0; z < var->z_length; z++)
            {
                offset = (x*sizeofy)+(y*sizeofz)+(var_size*z);
                fprintf(fp, "\"%S\"", (wchar_t*)(var->data+offset));
                if(z+1 < var->z_length)
                    fprintf(fp, ", ");
            }
            fprintf(fp, "]");
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}


void printGroup(vars_t *var, FILE *fp, bool json)
{
    int off = 0;

    if(json)
        printf("\"%s\":{", var->name);
    else
    {
        printf("Group: [%s]:\n", var->name);
        off = 1;
    }

    printAllVarsToFile_in(var->next_lvl, fp, off, json);

    if(json)
        fprintf(fp, "}");
}
