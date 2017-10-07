#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "gen_json.h"

void J_printInteger(vars_t *var, FILE *output)
{
    fprintf(output, "%d", *((int*)var->data));
}

void J_print1DIntegerX(vars_t *var, int x, FILE *output)
{
    fprintf(output, "%d", ((int*)var->data)[x]);
}

void J_print2DIntegerXY(vars_t *var, int x, int y, FILE *output)
{
    fprintf(output, "%d", ((int*)var->data)[(x*var->y_length)+y]);
}

void J_print3DIntegerXYZ(vars_t *var, int x, int y, int z, FILE *output)
{
    int offset = (z*var->x_length * var->y_length);
    offset += (y*var->x_length) + x;

    fprintf(output, "%d", ((int*)var->data)[offset]);
}

void J_printString(vars_t *var, FILE *output)
{
    fprintf(output, "\"%S\"", (wchar_t*)var->data);
}

void J_print1DStringX(vars_t *var, int x, FILE *output)
{
    fprintf(output, "\"%S\"", (wchar_t*)(var->data+(((var->length)*sizeof(wchar_t))*x)));
}

void J_print2DStringXY(vars_t *var, int x, int y, FILE *output)
{
    int offset = (var->y_length*(var->length*sizeof(wchar_t)));
    offset = offset*(x)+((var->length*sizeof(wchar_t))*(y));

    fprintf(output, "\"%S\"", (wchar_t*)(var->data+offset));
}

void J_print3DStringXYZ(vars_t *var, int x, int y, int z, FILE *output)
{
    int var_size = var->length*sizeof(wchar_t);
    int sizeofz = var_size*(var->z_length);
    int sizeofy = sizeofz*(var->y_length);
    int offset = (x*sizeofy)+(y*sizeofz)+(var_size*z);

    fprintf(output, "\"%S\"", (wchar_t*)(var->data+offset));
}

void J_printsingleBoolean(bool val, FILE *fp)
{
    if(val == 1)
        fprintf(fp, "true");
    else
        fprintf(fp, "false");
}

void J_printBool(vars_t *var, FILE *output)
{
    J_printsingleBoolean(*((bool*)var->data), output);
}

void J_print1DBoolX(vars_t *var, int x, FILE *output)
{
    J_printsingleBoolean(((bool*)var->data)[x], output);
}

void J_print2DBoolXY(vars_t *var, int x, int y, FILE *output)
{
    int offset = (x*var->y_length)+y;
    J_printsingleBoolean(((bool*)var->data)[offset], output);
}

void J_print3DBoolXYZ(vars_t *var, int x, int y, int z, FILE *output)
{
    
    int offset = (z*var->x_length * var->y_length);
    offset += (y*var->x_length) + x;
    J_printsingleBoolean(((bool*)var->data)[offset], output);
}

void J_printFloat(vars_t *var, FILE *output)
{
    fprintf(output, "%f", *((double*)var->data));
}

void J_print1DFloatX(vars_t *var, int x, FILE *output)
{
    fprintf(output, "%f", ((double*)var->data)[x]);
}

void J_print2DFloatXY(vars_t *var, int x, int y, FILE *output)
{
    fprintf(output, "%f", ((double*)var->data)[(x*var->y_length)+y]);
}

void J_print3DFloatXYZ(vars_t *var, int x, int y, int z, FILE *output)
{
    int offset = (z*var->x_length * var->y_length);
    offset += (y*var->x_length) + x;
    fprintf(output, "%f", ((double*)var->data)[offset]);
}

int onedmatcher[] = {
    ONEDINTEGER,
    ONEDBOOL,
    ONEDSTRING,
    ONEDFLOAT
};

int twodmatcher[] = {
    TWODINTEGER,
    TWODBOOL,
    TWODSTRING,
    TWODFLOAT
};

int threedmatcher[] = {
    THREEDINTEGER,
    THREEDBOOL,
    THREEDSTRING,
    THREEDFLOAT
};

void (*onedprinter[])(vars_t*, int, FILE*) = {
    J_print1DIntegerX,
    J_print1DBoolX,
    J_print1DStringX,
    J_print1DFloatX
};

void (*twodprinter[])(vars_t*, int, int, FILE*) = {
    J_print2DIntegerXY,
    J_print2DBoolXY,
    J_print2DStringXY,
    J_print2DFloatXY
};

void (*threedprinter[])(vars_t*, int, int, int, FILE*) = {
    J_print3DIntegerXYZ,
    J_print3DBoolXYZ,
    J_print3DStringXYZ,
    J_print3DFloatXYZ,
};

void J_print1DArray(vars_t *var, FILE *output)
{
    int x = 0;
    
    fputc('[', output);
    for(; x < var->x_length; x++)
    {
        J_print1DArrayX(var, x, output);
        if(x+1 < var->x_length)
            fputc(',', output);
    }
    fputc(']', output);
}

void J_print1DArrayX(vars_t *var, int x, FILE *output)
{
    int i = 0;
    void (*print_func)(vars_t*, int, FILE*) = NULL;

    for(;i < NUMBER_BASIC_VAR_TYPES; i++)
    {
        if(onedmatcher[i] == var->type)
        {
            print_func = onedprinter[i];
            break;
        }
    }

    if(!print_func)
        return;

    print_func(var, x, output);
}

void J_print2DArray(vars_t * var, FILE *output)
{
    int x = 0;

    fputc('[', output);
    for(; x < var->x_length; x++)
    {
        J_print2DArrayX(var, x, output);
        if(x+1 < var->x_length)
            fputc(',', output);
    }
    fputc(']', output);
}

void J_print2DArrayX(vars_t *var, int x, FILE *output)
{
    int y = 0;

    fputc('[', output);
    for(; y < var->y_length; y++)
    {
        J_print2DArrayXY(var, x, y, output);
        if(y+1 < var->y_length)
            fputc(',', output);
    }
    fputc(']', output);

}

void J_print2DArrayXY(vars_t *var, int x, int y, FILE *output)
{
    int i = 0;
    void (*print_func)(vars_t*, int, int, FILE*) = NULL;

    for(;i < NUMBER_BASIC_VAR_TYPES; i++)
    {
        if(twodmatcher[i] == var->type)
        {
            print_func = twodprinter[i];
            break;
        }
    }

    if(!print_func)
        return;

    print_func(var, x, y, output);
}

void J_print3DArray(vars_t *var, FILE *output)
{
    int x = 0;

    fputc('[', output);
    for(; x < var->x_length; x++)
    {
        J_print3DArrayX(var, x, output);
        if(x+1 < var->x_length)
            fputc(',', output);
    }
    fputc(']', output);
}

void J_print3DArrayX(vars_t *var, int x, FILE *output)
{
    int y = 0;

    fputc('[', output);
    for(; y < var->y_length; y++)
    {
        J_print3DArrayXY(var, x, y, output);
        if(y+1 < var->y_length)
            fputc(',', output);
    }
    fputc(']', output);

}

void J_print3DArrayXY(vars_t *var, int x, int y, FILE *output)
{
    int z = 0;

    fputc('[', output);
    for(; z < var->z_length; z++)
    {
        J_print3DArrayXYZ(var, x, y, z, output);
        if(z+1 < var->z_length)
            fputc(',', output);
    }
    fputc(']', output);
}

void J_print3DArrayXYZ(vars_t *var, int x, int y, int z, FILE *output)
{
    int i = 0;
    void (*print_func)(vars_t*, int, int, int, FILE*) = NULL;

    for(;i < NUMBER_BASIC_VAR_TYPES; i++)
    {
        if(threedmatcher[i] == var->type)
        {
            print_func = threedprinter[i];
            break;
        }
    }

    if(!print_func)
        return;
    
    print_func(var, x, y, z, output);
}

void printEntry(vars_t *var, int x, int y, int z, FILE *output)
{
    switch(var->type)
    {
        case INTEGER:
            J_printInteger(var, output);
            break;
        case BOOL:
            J_printBool(var, output);
            break;
        case STRING:
            J_printString(var, output);
            break;
        case FLOAT:
            J_printFloat(var, output);
            break;
        case ONEDINTEGER:
        case ONEDBOOL:
        case ONEDSTRING:
        case ONEDFLOAT:
            if(x == -1)
                J_print1DArray(var, output);
            else
                J_print1DArrayX(var, x, output);
            break;
        case TWODINTEGER:
        case TWODBOOL:
        case TWODSTRING:
        case TWODFLOAT:
            if(x == -1 && y == -1)
                J_print2DArray(var, output);
            else if(y == -1)
                J_print2DArrayX(var, x, output);
            else
                J_print2DArrayXY(var, x, y, output);
            break;
        case THREEDINTEGER:
        case THREEDBOOL:
        case THREEDSTRING:
        case THREEDFLOAT:
            if(x == -1 &&  y == -1 && z == -1)
                J_print3DArray(var, output);
            else if(y == -1 && z == -1)
                J_print3DArrayX(var, x, output);
            else if(z == -1)
                J_print3DArrayXY(var, x, y, output);
            else
                J_print3DArrayXYZ(var, x, y, z, output);
            break;
    }
}
