#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "print.h"

void printInteger(vars_t *var, FILE *fp, bool json)
{
    if(json)
        fprintf(fp, "\"%s\": %d", var->name, *((int*)var->data));
    else
        fprintf(fp, "[%s] = [%d]", var->name, *((int*)var->data));
}

void printRawInteger(vars_t *var, FILE *fp)
{
    fprintf(fp, "%d", *((int*)var->data));
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

void printRaw1DInteger(vars_t *var, FILE *fp)
{
    int i = 0;

    fprintf(fp, "[");
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

void print1DIntegerWithXIndex(vars_t *var, FILE *fp, int x, int json)
{
    int val = ((int*)var->data)[x];

    if(json)
        fprintf(fp, "\"%s\": %d", var->name, val);
    else
        fprintf(fp, "%d", val);
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

void printRaw2DInteger(vars_t *var, FILE *fp)
{
    int x = 0, y = 0;

    fprintf(fp, "[");
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

void print2DIntegerWithXIndex(vars_t *var, FILE *fp, int x_index, int json)
{
    int y = 0;

    if(json) fprintf(fp, "\"%s\": [", var->name);
    else fprintf(fp, "[");
    
    for(; y < var->y_length; y++)
    {
        fprintf(fp, "%d", ((int*)var->data)[(x_index*var->y_length)+y]);
        if(y+1 < var->y_length)
            fprintf(fp, ",");
    }
    fprintf(fp, "]");
}

void print2DIntegerWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index, 
                               int json)
{
    int val = ((int*)var->data)[(x_index*var->y_length)+y_index];

    if(json) fprintf(fp, "\"%s\": %d", var->name, val);
    else fprintf(fp, "%d", val);
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

void printRaw3DInteger(vars_t *var, FILE *fp)
{
    int x = 0, y = 0, z = 0,
        offset = 0;

    fprintf(fp, "[");
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

void print3DIntegerWithXIndex(vars_t *var, FILE *fp, int x_index, int json)
{
    int x = 0, y = 0, z = 0,
        offset = 0;

    if (json) fprintf(fp, "\"%s\": [", var->name);
    else fprintf(fp, "[");

    for(y=0; y < var->y_length; y++)
    {
        fprintf(fp, "[");
        for(z=0; z < var->z_length; z++)
        {
            offset = (z*var->x_length * var->y_length);
            offset += (y*var->x_length) + x_index;
            fprintf(fp, "%d", ((int*)var->data)[offset]);
            if(z+1 < var->z_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(y+1 < var->y_length)
            fprintf(fp, ", ");
    }

    fprintf(fp, "]");
}

void print3DIntegerWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index, int json)
{
    int x = 0, y = 0, z = 0,
        offset = 0;

    if (json) fprintf(fp, "\"%s\": [", var->name);
    else fprintf(fp, "[");

    for(z=0; z < var->z_length; z++)
    {
        offset = (z*var->x_length * var->y_length);
        offset += (y_index*var->x_length) + x_index;
        fprintf(fp, "%d", ((int*)var->data)[offset]);
        if(z+1 < var->z_length)
            fprintf(fp, ", ");
    }

    fprintf(fp, "]");
}

void print3DIntegerWithXYZIndex(vars_t *var, FILE *fp, int x_index, int y_index, 
                                int z_index, int json)
{
    int x = 0, y = 0, z = 0,
        offset = 0, val = 0;

    offset = (z_index*var->x_length * var->y_length);
    offset += (y_index*var->x_length) + x_index;

    val = ((int*)var->data)[offset];

    if (json) fprintf(fp, "\"%s\": %d", var->name, val);
    else fprintf(fp, "%d", val);
}
