#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "utils.h"

//TODO: Write a function which replaces the variablen search code
//TODO: change group type to an char pointer array so that you can read multiple groups

int addInteger(vars_t *anker, char *group, char *name, int val)
{
    vars_t *new = NULL;

    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = INTEGER;
    if((new->data = malloc(sizeof(int))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    *((int*)new->data) = val;
    return(0);
}

int getInteger(vars_t *anker, char *group, char *name, int *val)
{
    vars_t *target = NULL,
           *grp = NULL;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    *val = *(int*)(target->data);
    return(0);

}

int editInteger(vars_t *anker, char *group, char *name, int val)
{
    vars_t *target = NULL,
           *grp = NULL;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    *(int*)(target->data) = val;
    return(0);

}

int add1DIntegerArray(vars_t *anker, char *group, char *name, int length)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = ONEDINTEGER;
    new->x_length = length;
    if((new->data = malloc(sizeof(int)*length)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);
}

int getIntegerFrom1DArray(vars_t *anker, char *group, char *name,
                          int *i_target, int x_index)
{
    vars_t *target = NULL,
           *grp = NULL;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(x_index >= target->x_length)
    {
        return(X_INDEX_OUT_OF_RANGE);
    }

    *i_target = ((int*)target->data)[x_index];
    return(0);

}

int edit1DIntegerArray(vars_t *anker, char *group, char *name, int val, int x_index)
{
    vars_t *target = NULL,
           *grp = NULL;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(x_index >= target->x_length)
    {
        return(X_INDEX_OUT_OF_RANGE);
    }

    ((int*)target->data)[x_index] = val;
    return(0);
}

int editFull1DIntegerArray(vars_t *anker, char *group, char *name, void *val)
{
    vars_t *target = NULL,
           *grp = NULL;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    memcpy(target->data, val, sizeof(int)*target->x_length);

    return(0);
}

int createNewVarFrom1DIntegerArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index)
{
    vars_t *target = NULL, *grp = NULL;
    int ret = 0, i_target = 0;

    if(group)
    {
        if(!(grp = isDefined(inanker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(inanker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(target->type != ONEDINTEGER)
        return(WRONG_VAR_TYPE);

    if((ret = getIntegerFrom1DArray(inanker, group, name, &i_target,
                    x_index)) != 0)
    {
        return(ret);
    }

    if((ret = addInteger(outanker, out_grp, new_name, i_target)) != 0)
    {
        return(0);
    }
    
    return(0);
}

int add2DIntegerArray(vars_t *anker, char *group, char *name, int x_length, int y_length)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = TWODINTEGER;
    new->x_length = x_length;
    new->y_length = y_length;
    if((new->data = malloc((x_length*y_length)*sizeof(int))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);

}

int getIntegerFrom2DArray(vars_t *anker, char *group, char *name,
                          int *i_target, int x_index, int y_index)
{
    vars_t *target = NULL,
           *grp = NULL;
    int offset = 0;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(x_index >= target->x_length)
    {
        return(X_INDEX_OUT_OF_RANGE);
    }

    if(y_index >= target->y_length)
    {
        return(X_INDEX_OUT_OF_RANGE);
    }

    offset = (x_index*target->y_length)+y_index;
    *i_target = ((int*)target->data)[offset];
    return(0);

}

int edit2DIntegerArray(vars_t *anker, char *group, char *name, int val, int x_index, int y_index)
{
    vars_t *target = NULL,
           *grp = NULL;
    size_t offset = 0;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(x_index >= target->x_length)
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length)
        return(Y_INDEX_OUT_OF_RANGE);


    offset = (x_index*target->y_length)+y_index;
    ((int*)target->data)[offset] = val;
    return(0);
}

int editFull2DIntegerArray(vars_t *anker, char *group, char *name, void *val)
{
    vars_t *target = NULL,
           *grp = NULL;
    size_t offset = 0;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    memcpy(target->data, val, (target->x_length*target->y_length)*sizeof(int));
    return(0);
}


int createNew1DArrayFrom2DIntegerArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index)
{
    int i = 0, ret = 0, i_target = 0,
        y_length = 0;

    if(getArrayLength(inanker, group, name, NULL, &y_length, NULL) != 0)
    {
        return(VAR_NOT_DEFINED);
    }

    if((ret = add1DIntegerArray(outanker, out_grp, new_name, y_length)) != 0)
        return(ret);

    for(; i < y_length; i++)
    {
        if((ret = getIntegerFrom2DArray(inanker, group, name, &i_target, x_index, i)))
            return(ret);

        if((ret = edit1DIntegerArray(outanker, out_grp, new_name, i_target, i)) != 0)
            return(ret);
    }
    return(0);
}

int createNewVarFrom2DIntegerArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index, int y_index)
{
    int i = 0, ret = 0, i_target = 0;

    if((ret = getIntegerFrom2DArray(inanker, group, name, &i_target, x_index, y_index)))
        return(ret);

    if((ret = addInteger(outanker, out_grp, new_name, i_target)) != 0)
        return(ret);

    return(0);
}

int add3DIntegerArray(vars_t *anker, char *group, char *name, int x_length, int y_length, int z_length)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = THREEDINTEGER;
    new->x_length = x_length;
    new->y_length = y_length;
    new->z_length = z_length;
    if((new->data = malloc(((x_length*y_length)*z_length)*sizeof(int))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);

}

int getIntegerFrom3DArray(vars_t *anker, char *group, char *name,
                          int *i_target, int x_index, int y_index, int z_index)
{
    vars_t *target = NULL,
           *grp = NULL;

    int offset = 0;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(x_index >= target->x_length)
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length)
        return(Y_INDEX_OUT_OF_RANGE);
    if(z_index >= target->z_length)
        return(Z_INDEX_OUT_OF_RANGE);

    offset = (z_index*target->x_length * target->y_length);
    offset += (y_index*target->x_length) + x_index;
    *i_target = ((int*)target->data)[offset];
    return(0);

}

int edit3DIntegerArray(vars_t *anker, char *group, char *name, int val, int x_index, int y_index, int z_index)
{
    vars_t *target = NULL,
           *grp = NULL;
    size_t offset = 0;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    if(x_index >= target->x_length)
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length)
        return(Y_INDEX_OUT_OF_RANGE);
    if(z_index >= target->z_length)
        return(Z_INDEX_OUT_OF_RANGE);

    // ( z * xSize * ySize ) + ( y * xSize ) + x

    offset = (z_index*target->x_length * target->y_length);
    offset += (y_index*target->x_length) + x_index;
    ((int*)target->data)[offset] = val;
    return(0);
}

int createNew2DArrayFrom3DIntegerArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index)
{
    int i = 0, x = 0, ret = 0, i_target = 0,
        y_length = 0, z_length = 0;

    if(getArrayLength(inanker, group, name, NULL, &y_length, &z_length) != 0)
    {
        return(VAR_NOT_DEFINED);
    }

    if((ret = add2DIntegerArray(outanker, out_grp, new_name, y_length, z_length)) != 0)
        return(ret);

    for(; i < y_length; i++)
    {
        for(x=0; x < z_length; x++)
        {
            if((ret = getIntegerFrom3DArray(inanker, group, name, &i_target, x_index, i, x)) != 0)
                return(ret);

            if((ret = edit2DIntegerArray(outanker, out_grp, new_name, i_target, i, x)) != 0)
                return(ret);
        }
    }
    return(0);
}

int createNew1DArrayFrom3DIntegerArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index, int y_index)
{
    int i = 0, x = 0, ret = 0, i_target = 0,
        y_length = 0, z_length = 0;

    if(getArrayLength(inanker, group, name, NULL, NULL, &z_length) != 0)
    {
        return(VAR_NOT_DEFINED);
    }

    if((ret = add1DIntegerArray(outanker, out_grp, new_name, z_length)) != 0)
        return(ret);

    for(; i < z_length; i++)
    {
        if((ret = getIntegerFrom3DArray(inanker, group, name, &i_target, x_index, y_index, i)) != 0)
            return(ret);

        if((ret = edit1DIntegerArray(outanker, out_grp, new_name, i_target, i)) != 0)
            return(ret);
    }
    return(0);
}

int createNewVarFrom3DIntegerArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index, int y_index, 
                                  int z_index)
{
    int i = 0, x = 0, ret = 0, i_target = 0,
        y_length = 0, z_length = 0;


    if((ret = getIntegerFrom3DArray(inanker, group, name, &i_target, x_index,
            y_index, z_index)) != 0)
        return(ret);

    if((ret = addInteger(outanker, out_grp, new_name, i_target)) != 0)
        return(ret);

    return(0);
}

int editFull3DIntegerArray(vars_t *anker, char *group, char *name, void *val)
{
    vars_t *target = NULL,
           *grp = NULL;
    size_t offset = 0;

    if(group)
    {
        if(!(grp = isDefined(anker, group)))
        {
            return(GRP_NOT_DEFINED);
        }
        if(!(target = isDefined(grp->next_lvl, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }
    else
    {
        if(!(target = isDefined(anker, name)))
        {
            return(VAR_NOT_DEFINED);
        }
    }

    memcpy(target->data, val, ((target->x_length*target->y_length)
                *target->z_length)*sizeof(int));

    return(0);
}
