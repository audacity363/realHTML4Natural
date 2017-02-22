#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "print.h"

void printString(vars_t *var, FILE *fp, bool json)
{
    if(json)
        fprintf(fp, "\"%s\": \"%S\"", var->name, (wchar_t*)var->data);
    else
        fprintf(fp, "[%s] = [%S]", var->name, (wchar_t*)var->data);
}

void printRawString(vars_t *var, FILE *fp)
{
    fprintf(fp, "%S", (wchar_t*)var->data);
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

void printRaw1DString(vars_t *var, FILE *fp)
{
    size_t x = 0, offset = 0;

    fprintf(fp, "[");

    for(x=0; x < var->x_length; x++)
    {
        offset = ((var->length)*sizeof(wchar_t))*x;
        fprintf(fp, "\"%S\"", (wchar_t*)(var->data+offset));
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print1DStringWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    size_t x = 0, offset = 0;

    if(json) fprintf(fp, "\"%s\": ", var->name);

    offset = ((var->length)*sizeof(wchar_t))*x_index;
    fprintf(fp, "%S", (wchar_t*)(var->data+offset));
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

void printRaw2DString(vars_t *var, FILE *fp)
{
    size_t x = 0, y = 0, offset = 0;

    fprintf(fp, "[", var->name);

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

void print2DStringWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    size_t x = 0, y = 0, offset = 0;

    if(json) fprintf(fp, "\"%s\": [", var->name);
    else fprintf(fp, "[", var->name);

    fprintf(fp, "[");
    for(y=0; y < var->y_length; y++)
    {
        offset = (var->y_length*(var->length*sizeof(wchar_t)));
        offset = offset*(x_index)+((var->length*sizeof(wchar_t))*(y));
        fprintf(fp, "\"%S\"", (wchar_t*)(var->data+offset));
        if(y+1 < var->y_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print2DStringWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index, 
                             bool json)
{
    size_t x = 0, y = 0, offset = 0;

    if(json) fprintf(fp, "\"%s\": ", var->name);

    offset = (var->y_length*(var->length*sizeof(wchar_t)));
    offset = offset*(x_index)+((var->length*sizeof(wchar_t))*(y_index));
    fprintf(fp, "%S", (wchar_t*)(var->data+offset));
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

void printRaw3DString(vars_t *var, FILE *fp)
{
    size_t x = 0, y = 0, z = 0, 
        offset = 0, var_size = 0,
        sizeofz = 0, sizeofy = 0;

    var_size = var->length*sizeof(wchar_t);
    sizeofz = var_size*(var->z_length);
    sizeofy = sizeofz*(var->y_length);

    fprintf(fp, "[", var->name);

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

void print3DStringWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    size_t x = 0, y = 0, z = 0, 
        offset = 0, var_size = 0,
        sizeofz = 0, sizeofy = 0;

    var_size = var->length*sizeof(wchar_t);
    sizeofz = var_size*(var->z_length);
    sizeofy = sizeofz*(var->y_length);

    if(json) fprintf(fp, "\"%s\": [", var->name);
    else fprintf(fp, "[", var->name);

    for(y=0; y < var->y_length; y++)
    {
        fprintf(fp, "[");
        for(z=0; z < var->z_length; z++)
        {
            offset = (x_index*sizeofy)+(y*sizeofz)+(var_size*z);
            fprintf(fp, "\"%S\"", (wchar_t*)(var->data+offset));
            if(z+1 < var->z_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(y+1 < var->y_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DStringWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index,
                              bool json)
{
    size_t x = 0, y = 0, z = 0, 
        offset = 0, var_size = 0,
        sizeofz = 0, sizeofy = 0;

    var_size = var->length*sizeof(wchar_t);
    sizeofz = var_size*(var->z_length);
    sizeofy = sizeofz*(var->y_length);

    if(json) fprintf(fp, "\"%s\": [", var->name);
    else fprintf(fp, "[", var->name);

    for(z=0; z < var->z_length; z++)
    {
        offset = (x_index*sizeofy)+(y_index*sizeofz)+(var_size*z);
        fprintf(fp, "\"%S\"", (wchar_t*)(var->data+offset));
        if(z+1 < var->z_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DStringWithXYZIndex(vars_t *var, FILE *fp, int x_index, int y_index,
                              int z_index, bool json)
{
    size_t x = 0, y = 0, z = 0, 
        offset = 0, var_size = 0,
        sizeofz = 0, sizeofy = 0;

    var_size = var->length*sizeof(wchar_t);
    sizeofz = var_size*(var->z_length);
    sizeofy = sizeofz*(var->y_length);

    if(json) fprintf(fp, "\"%s\": ", var->name);

    offset = (x_index*sizeofy)+(y_index*sizeofz)+(var_size*z_index);
    fprintf(fp, "%S", (wchar_t*)(var->data+offset));
}
