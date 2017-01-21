#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "print.h"

void printFloat(vars_t *var, FILE *fp, bool json)
{
    if(json)
        fprintf(fp, "\"%s\": %f", var->name, *((double*)var->data));
    else
        fprintf(fp, "[%s] = [%f]", var->name, *((double*)var->data));
}

void printRawFloat(vars_t *var, FILE *fp)
{
    fprintf(fp, "%f", *((double*)var->data));
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

void printRaw1DFloat(vars_t *var, FILE *fp)
{
    int i = 0;

    fprintf(fp, "[");

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

void print1DFloatWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    int i = 0;

    if(json) fprintf(fp, "\"%s\": ", var->name);

    fprintf(fp, "%f", ((double*)var->data)[x_index]);
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

void printRaw2DFloat(vars_t *var, FILE *fp)
{
    int x = 0, y = 0;

    fprintf(fp, "[", var->name);

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

void print2DFloatWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    int x = 0, y = 0;

    if (json) fprintf(fp, "\"%s\": [");
    else fprintf(fp, "[", var->name);

    for(y=0; y < var->y_length; y++)
    {
        fprintf(fp, "%f", ((double*)var->data)[(x_index*var->y_length)+y]);
        if(y+1 < var->y_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print2DFloatWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index,
                             bool json)
{
    int x = 0, y = 0;

    if (json) fprintf(fp, "\"%s\": ");

    fprintf(fp, "%f", ((double*)var->data)[(x*var->y_length)+y_index]);
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

void printRaw3DFloat(vars_t *var, FILE *fp)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    fprintf(fp, "[", var->name);

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

void print3DFloatWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[", var->name);

    for(y=0; y < var->y_length; y++)
    {
        fprintf(fp, "[");
        for(z=0; z < var->z_length; z++)
        {
            offset = (z*var->x_length * var->y_length);
            offset += (y*var->x_length) + x_index;
            fprintf(fp, "%f", ((double*)var->data)[offset]);
            if(z+1 < var->z_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(y+1 < var->y_length)
            fprintf(fp, ", ");
    }

    fprintf(fp, "]");
}

void print3DFloatWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index, 
                                bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[", var->name);

    for(z=0; z < var->z_length; z++)
    {
        offset = (z*var->x_length * var->y_length);
        offset += (y_index*var->x_length) + x_index;
        fprintf(fp, "%f", ((double*)var->data)[offset]);
        if(z+1 < var->z_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DFloatWithXYZIndex(vars_t *var, FILE *fp, int x_index, int y_index, 
                                int z_index, bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": ", var->name);

    offset = (z_index*var->x_length * var->y_length);
    offset += (y_index*var->x_length) + x_index;
    fprintf(fp, "%f", ((double*)var->data)[offset]);
}
