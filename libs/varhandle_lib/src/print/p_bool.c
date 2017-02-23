#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "print.h"

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
        fprintf(fp, "\"%s\": ", var->name);
    else
        fprintf(fp, "[%s] = [", var->name);

    printsingleBoolean(*((bool*)var->data), fp, json);
    if(!json)
        fprintf(fp, "]");
}

void printRawBoolean(vars_t *var, FILE *fp)
{
    printsingleBoolean(*((bool*)var->data), fp, 0);
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

void printRaw1DBoolean(vars_t *var, FILE *fp)
{
    int x = 0;

    fprintf(fp, "[");
    for(x=0; x < var->x_length; x++)
    {
        printsingleBoolean(((bool*)var->data)[x], fp, 0);
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");

}

void print1DBooleanWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    if(json) fprintf(fp, "\"%s\": ", var->name);
    printsingleBoolean(((bool*)var->data)[x_index], fp, json);
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

void printRaw2DBoolean(vars_t *var, FILE *fp)
{
    size_t x = 0, y = 0, offset = 0;

    fprintf(fp, "[");

    for(x=0; x < var->x_length; x++)
    {
        fprintf(fp, "[");
        for(y=0; y < var->y_length; y++)
        {
            offset = (x*var->y_length)+y;
            printsingleBoolean(((bool*)var->data)[offset], fp, 0);
            if(y+1 < var->y_length)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
        if(x+1 < var->x_length)
            fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print2DBooleanWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    size_t x = 0, y = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[");

    for(y=0; y < var->y_length; y++)
    {
        offset = (x_index*var->y_length)+y;
        printsingleBoolean(((bool*)var->data)[offset], fp, json);
        if(y+1 < var->y_length)
            fprintf(fp, ", ");
    }

    fprintf(fp, "]");
}

void print2DBooleanWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index, bool json)
{
    size_t x = 0, y = 0, offset = 0;

    if(json) fprintf(fp, "\"%s\": ", var->name);

    offset = (x_index*var->y_length)+y_index;
    printsingleBoolean(((bool*)var->data)[offset], fp, json);
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

void printRaw3DBoolean(vars_t *var, FILE *fp)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

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
                printsingleBoolean(((bool*)var->data)[offset], fp, 0);
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

void print3DBooleanWithXIndex(vars_t *var, FILE *fp, int x_index, bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[");

    for(y=0; y < var->y_length; y++)
    {
        fprintf(fp, "[");
        for(z=0; z < var->z_length; z++)
        {
            offset = (z*var->x_length * var->y_length);
            offset += (y*var->x_length) + x_index;
            printsingleBoolean(((bool*)var->data)[offset], fp, json);
            if(z+1 < var->z_length)
               fprintf(fp, ", ");
        }
        fprintf(fp, "]");

        if(y+1 < var->y_length)
        fprintf(fp, ", ");
    }
    
    fprintf(fp, "]");
}

void print3DBooleanWithXYIndex(vars_t *var, FILE *fp, int x_index, int y_index,
                               bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json)
        fprintf(fp, "\"%s\": [", var->name);
    else
        fprintf(fp, "[");

    for(z=0; z < var->z_length; z++)
    {
        offset = (z*var->x_length * var->y_length);
        offset += (y_index*var->x_length) + x_index;
        printsingleBoolean(((bool*)var->data)[offset], fp, json);
        if(z+1 < var->z_length)
           fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

void print3DBooleanWithXYZIndex(vars_t *var, FILE *fp, int x_index, int y_index,
                               int z_index, bool json)
{
    size_t x = 0, y = 0, z = 0, offset = 0;

    if(json) fprintf(fp, "\"%s\": ", var->name);

    offset = (z_index*var->x_length * var->y_length);
    offset += (y_index*var->x_length) + x_index;
    printsingleBoolean(((bool*)var->data)[offset], fp, json);
}
