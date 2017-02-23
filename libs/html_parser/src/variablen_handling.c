#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"

#include "parser_errno.h"

int handleONEDINTEGER(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DInteger(target, f_output);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print1DIntegerWithXIndex(target, f_output, index[0], false);
    }
    return(0);
}

int handleTWODINTEGER(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DInteger(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print2DIntegerWithXIndex(target, f_output, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print2DIntegerWithXYIndex(target, f_output, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDINTEGER(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DInteger(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print3DIntegerWithXIndex(target, f_output, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print3DIntegerWithXYIndex(target, f_output, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            parser_errno = INDEX_Z_OUT_OF_RANGE;
            return(-3);
        }
        print3DIntegerWithXYZIndex(target, f_output, index[0], index[1], index[2], false);
    }
    return(0);
}

int handleONEDBOOL(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DBoolean(target, f_output);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print1DBooleanWithXIndex(target, f_output, index[0], false);
    }
    return(0);
}

int handleTWODBOOL(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DBoolean(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print2DBooleanWithXIndex(target, f_output, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print2DBooleanWithXYIndex(target, f_output, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDBOOL(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DBoolean(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print3DBooleanWithXIndex(target, f_output, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print3DBooleanWithXYIndex(target, f_output, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            parser_errno = INDEX_Z_OUT_OF_RANGE;
            return(-3);
        }
        print3DBooleanWithXYZIndex(target, f_output, index[0], index[1], index[2], false);
    }
    return(0);
}

int handleONEDFLOAT(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DFloat(target, f_output);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print1DFloatWithXIndex(target, f_output, index[0], false);
    }
    return(0);
}

int handleTWODFLOAT(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DFloat(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print2DFloatWithXIndex(target, f_output, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print2DFloatWithXYIndex(target, f_output, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDFLOAT(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DFloat(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print3DFloatWithXIndex(target, f_output, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print3DFloatWithXYIndex(target, f_output, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            parser_errno = INDEX_Z_OUT_OF_RANGE;
            return(-3);
        }
        print3DFloatWithXYZIndex(target, f_output, index[0], index[1], index[2], false);
    }
    return(0);
}

int handleONEDSTRING(vars_t *target, int index_type, int *index)
{
    if(index_type > 1)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw1DString(target, f_output);
        return(0);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print1DStringWithXIndex(target, f_output, index[0], false);
    }
    return(0);
}

int handleTWODSTRING(vars_t *target, int index_type, int *index)
{
    if(index_type > 2)
    {
        parser_errno = UNSUPPORTED_INDEX;
        return(-1);
    }

    if(index_type == 0)
    {
        printRaw2DString(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print2DStringWithXIndex(target, f_output, index[0], false);
    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print2DStringWithXYIndex(target, f_output, index[0], index[1], false);
    }
    return(0);
}

int handleTHREEDSTRING(vars_t *target, int index_type, int *index)
{
    if(index_type == 0)
    {
        printRaw3DString(target, f_output);
        return(0);
    }
    else if(index_type == 1)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        print3DStringWithXIndex(target, f_output, index[0], false);
    }
    else if(index_type == 2)
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        print3DStringWithXYIndex(target, f_output, index[0], index[1], false);

    }
    else
    {
        if(target->x_length <= index[0])
        {
            parser_errno = INDEX_X_OUT_OF_RANGE;
            return(-2);
        }
        if(target->y_length <= index[1])
        {
            parser_errno = INDEX_Y_OUT_OF_RANGE;
            return(-3);
        }
        if(target->z_length <= index[2])
        {
            parser_errno = INDEX_Z_OUT_OF_RANGE;
            return(-3);
        }
        print3DStringWithXYZIndex(target, f_output, index[0], index[1], index[2], false);
    }
    return(0);
}
