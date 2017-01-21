#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"

int handleONEDINTEGER(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DInteger(target, stdout);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print1DIntegerWithXIndex(target, stdout, index[0], false);
    }
    return(0);
}

int handleTWODINTEGER(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DInteger(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print2DIntegerWithXIndex(target, stdout, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print2DIntegerWithXYIndex(target, stdout, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDINTEGER(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DInteger(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print3DIntegerWithXIndex(target, stdout, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DIntegerWithXYIndex(target, stdout, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DIntegerWithXYZIndex(target, stdout, index[0], index[1], index[2], false);
    }
    return(0);
}

int handleONEDBOOL(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DBoolean(target, stdout);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print1DBooleanWithXIndex(target, stdout, index[0], false);
    }
    return(0);
}

int handleTWODBOOL(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DBoolean(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print2DBooleanWithXIndex(target, stdout, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print2DBooleanWithXYIndex(target, stdout, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDBOOL(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DBoolean(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print3DBooleanWithXIndex(target, stdout, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DBooleanWithXYIndex(target, stdout, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DBooleanWithXYZIndex(target, stdout, index[0], index[1], index[2], false);
    }
    return(0);
}

int handleONEDFLOAT(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DFloat(target, stdout);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print1DFloatWithXIndex(target, stdout, index[0], false);
    }
    return(0);
}

int handleTWODFLOAT(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DFloat(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print2DFloatWithXIndex(target, stdout, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print2DFloatWithXYIndex(target, stdout, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDFLOAT(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DFloat(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print3DFloatWithXIndex(target, stdout, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DFloatWithXYIndex(target, stdout, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DFloatWithXYZIndex(target, stdout, index[0], index[1], index[2], false);
    }
    return(0);
}

int handleONEDSTRING(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DString(target, stdout);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print1DStringWithXIndex(target, stdout, index[0], false);
    }
    return(0);
}

int handleTWODSTRING(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        fprintf(stderr, "Index type not supported\n");
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DString(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print2DStringWithXIndex(target, stdout, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print2DStringWithXYIndex(target, stdout, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDSTRING(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DString(target, stdout);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        print3DStringWithXIndex(target, stdout, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DStringWithXYIndex(target, stdout, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            fprintf(stderr, "X index out of range\n");
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            fprintf(stderr, "Y index out of range\n");
            return(-3);
        }
        print3DStringWithXYZIndex(target, stdout, index[0], index[1], index[2], false);
    }
    return(0);
}
