#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "utils.h"

int addBoolean(vars_t *anker, char *group, char *name, bool val)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = BOOL;
    if((new->data = malloc(sizeof(bool))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    *((bool*)new->data) = val;
    return(0);
}

int add1DBooleanArray(vars_t *anker, char *group, char *name, int length)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = ONEDBOOL;
    new->x_length = length;
    if((new->data = malloc(sizeof(bool)*length)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);
}

int getBooleanFrom1DArray(vars_t *anker, char *group, char *name,
                          bool *b_target, int x_index)
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

    *b_target = ((bool*)target->data)[x_index];
    return(0);

}


int edit1DBooleanArray(vars_t *anker, char *group, char *name, bool val, int x_index)
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

    ((bool*)target->data)[x_index] = val;
    return(0);
}

int createNewVarFrom1DBooleanArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index)
{
    vars_t *target = NULL, *grp = NULL;
    int ret = 0;
    bool b_target = 0;

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

    if(target->type != ONEDBOOL)
        return(WRONG_VAR_TYPE);

    if((ret = getBooleanFrom1DArray(inanker, group, name, &b_target,
                    x_index)) != 0)
    {
        return(ret);
    }

    if((ret = addBoolean(outanker, out_grp, new_name, b_target)) != 0)
    {
        return(0);
    }
    
    return(0);
}

int editFull1DBooleanArray(vars_t *anker, char *group, char *name, void *val)
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

    memcpy(target->data, val, sizeof(bool)*target->x_length);

    return(0);
}

int add2DBooleanArray(vars_t *anker, char *group, char *name, int x_length, int y_length)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = TWODBOOL;
    new->x_length = x_length;
    new->y_length = y_length;
    if((new->data = malloc((x_length*y_length)*sizeof(bool))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    memset(new->data, 0x00, (x_length*y_length)*sizeof(bool));
    return(0);

}

int getBooleanFrom2DArray(vars_t *anker, char *group, char *name,
                          bool *b_target, int x_index, int y_index)
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
    *b_target = ((bool*)target->data)[offset];
    return(0);

}



int edit2DBooleanArray(vars_t *anker, char *group, char *name, bool val, int x_index, int y_index)
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
    ((bool*)target->data)[offset] = val;
    return(0);
}

int createNew1DArrayFrom2DBooleanArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index)
{
    int i = 0, ret = 0, y_length = 0;
    bool b_target = false;

    if(getArrayLength(inanker, group, name, NULL, &y_length, NULL) != 0)
    {
        return(VAR_NOT_DEFINED);
    }

    if((ret = add1DBooleanArray(outanker, out_grp, new_name, y_length)) != 0)
        return(ret);

    for(; i < y_length; i++)
    {
        if((ret = getBooleanFrom2DArray(inanker, group, name, &b_target, x_index, i)))
            return(ret);

        if((ret = edit1DBooleanArray(outanker, out_grp, new_name, b_target, i)) != 0)
            return(ret);
    }
    return(0);
}

int editFull2DBooleanArray(vars_t *anker, char *group, char *name, void *val)
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

    memcpy(target->data, val, (target->x_length*target->y_length)*sizeof(bool));
    return(0);
}


int add3DBooleanArray(vars_t *anker, char *group, char *name, int x_length, int y_length, int z_length)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = THREEDBOOL;
    new->x_length = x_length;
    new->y_length = y_length;
    new->z_length = z_length;
    if((new->data = malloc(((x_length*y_length)*z_length)*sizeof(bool))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }
    return(0);

}

int getBooleanFrom3DArray(vars_t *anker, char *group, char *name,
                          bool *b_target, int x_index, int y_index, int z_index)
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
    *b_target = ((bool*)target->data)[offset];
    return(0);

}

int edit3DBoolArray(vars_t *anker, char *group, char *name, bool val, int x_index, int y_index, int z_index)
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

    offset = (z_index*target->x_length * target->y_length);
    offset += (y_index*target->x_length) + x_index;
    ((bool*)target->data)[offset] = val;
    return(0);
}

int createNew2DArrayFrom3DBooleanArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index)
{
    int i = 0, x = 0, ret = 0, 
        y_length = 0, z_length = 0;

    bool b_target = false;

    if(getArrayLength(inanker, group, name, NULL, &y_length, &z_length) != 0)
    {
        return(VAR_NOT_DEFINED);
    }

    if((ret = add2DBooleanArray(outanker, out_grp, new_name, y_length, z_length)) != 0)
        return(ret);

    for(; i < y_length; i++)
    {
        for(x=0; x < z_length; x++)
        {
            if((ret = getBooleanFrom3DArray(inanker, group, name, &b_target, x_index, i, x)) != 0)
                return(ret);

            if((ret = edit2DBooleanArray(outanker, out_grp, new_name, b_target, i, x)) != 0)
                return(ret);
        }
    }
    return(0);
}

int createNew1DArrayFrom3DBooleanArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *out_grp,
                                  char *new_name, int x_index, int y_index)
{
    int i = 0, x = 0, ret = 0, 
        y_length = 0, z_length = 0;

    bool b_target = false;

    if(getArrayLength(inanker, group, name, NULL, NULL, &z_length) != 0)
    {
        return(VAR_NOT_DEFINED);
    }

    if((ret = add1DBooleanArray(outanker, out_grp, new_name, z_length)) != 0)
        return(ret);

    for(; i < z_length; i++)
    {
        if((ret = getBooleanFrom3DArray(inanker, group, name, &b_target, x_index, y_index, i)) != 0)
            return(ret);

        if((ret = edit1DBooleanArray(outanker, out_grp, new_name, b_target, i)) != 0)
            return(ret);
    }
    return(0);
}

int editFull3DBooleanArray(vars_t *anker, char *group, char *name, void *val)
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
                *target->z_length)*sizeof(bool));

    return(0);
}
