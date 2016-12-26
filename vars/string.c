#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "utils.h"

//TODO: Write copy function and track complete size of variable in vars_t struct
//TODO: Write a function which replaces the variablen search code
//TODO: change group type to an char pointer array so that you can read multiple groups

int getStringLength(vars_t *anker, char *group, char *name)
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

    if(target->type != STRING &&
       target->type != ONEDSTRING &&
       target->type != TWODSTRING &&
       target->type != THREEDSTRING)
        return(WRONG_VAR_TYPE);

    return(target->length);

}

int addString(vars_t *anker, char *group, char *name, wchar_t *val, size_t length)
{
    vars_t *new = NULL;
    int ret = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = STRING;
    if((new->data = malloc((length+1)*sizeof(wchar_t))) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    new->length = length+1;

    wcscpy((wchar_t*)new->data, val);
    return(0);
}

int getString(vars_t *anker, char *group, char *name, wchar_t *val, int length)
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

    if(target->type != STRING)
        return(WRONG_VAR_TYPE);

    if(wcslen((wchar_t*)target->data) >= length)
        return(BUFFER_TO_SAMLL);

    wcscpy(val, (wchar_t*)target->data);
    return(0);
}

int create1DCharArrayFromString(vars_t *inanker, vars_t *outanker, char *group, 
                            char *name, char *new_name)
{
    vars_t *target = NULL, *grp = NULL;
    int ret = 0, x = 0;
    size_t length = 0, offset = 0;
    wchar_t buff[2] = {0, 0};

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

    if(target->type != STRING)
        return(WRONG_VAR_TYPE);

    length = wcslen((wchar_t*)target->data);

    if((ret = add1DStringArray(outanker, NULL, new_name, 2, length)) != 0)
        return(ret);

    memset(buff, 0x00, sizeof(buff));
    for(x=0; x < length; x++)
    {
        offset = x*sizeof(wchar_t);
        memcpy(buff, target->data+offset, sizeof(wchar_t));
        if((ret = edit1DStringArray(outanker, NULL, new_name, buff, x)) != 0)
            return(ret);
    }

    return(0);
}

int add1DStringArray(vars_t *anker, char *group, char *name, int var_length, size_t x_length)
{
    vars_t *new = NULL;
    int ret = 0;
    size_t size = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = ONEDSTRING;
    new->length = var_length+1;
    new->x_length = x_length;

    size = new->length*new->x_length*sizeof(wchar_t);

    if((new->data = malloc(size)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    return(0);
}


int edit1DStringArray(vars_t *anker, char *group, char *name, wchar_t *val, int x_index)
{
    vars_t *target = NULL, *grp = NULL;
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

    if(wcslen(val)+1 >= target->length)
        return(VAR_VALUE_TO_LONG);        

    if(x_index >= target->x_length) 
        return(X_INDEX_OUT_OF_RANGE);

    offset = ((target->length)*sizeof(wchar_t))*(x_index);

    memset((void*)(target->data+offset), 0x00, target->length*sizeof(wchar_t));

    wcscpy((wchar_t*)(target->data+offset), val);
    return(0);
}

int getStringFrom1DArray(vars_t *anker, char *group, char *name, wchar_t *val,
                         int length, int x_index)
{
    vars_t *target = NULL, *grp = NULL;
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

    if(target->type != ONEDSTRING)
        return(WRONG_VAR_TYPE);

    if(x_index >= target->x_length)
        return(X_INDEX_OUT_OF_RANGE);

    offset = ((target->length+1)*sizeof(wchar_t))*x_index;

    if(wcslen((wchar_t*)target->data) >= length)
        return(BUFFER_TO_SAMLL);

    wcscpy(val, (wchar_t*)(target->data+offset));
    return(0);
}

int createNewVarFrom1DStringArray(vars_t *inanker, vars_t *outanker,
                                  char *group, char *name, char *new_name,
                                  int x_index)
{
    vars_t *target = NULL, *grp = NULL;
    int ret = 0;
    wchar_t  *wc_target = NULL;

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

    if(target->type != ONEDSTRING)
        return(WRONG_VAR_TYPE);

    if(!(wc_target = malloc(target->length*sizeof(wchar_t))))
        return(MEMORY_ALLOC_ERROR);

    if((ret = getStringFrom1DArray(inanker, group, name, wc_target,
                    target->length, x_index)) != 0)
    {
        free(wc_target);
        return(ret);
    }

    if((ret = addString(outanker, NULL, new_name, wc_target, wcslen(wc_target))) != 0)
    {
        free(wc_target);
        return(0);
    }
    
    free(wc_target);
    return(0);
}

int create1DCharArrayFrom1DStringArray(vars_t *inanker, vars_t *outanker,
                                       char *group, char *name, char *new_name,
                                       int x_index)
{
    vars_t *target = NULL,
           *grp = NULL;
    int ret = 0,
        x = 0;
    wchar_t buff[2] = {0, 0},
            *wc_target = NULL;

    size_t length = 0;

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

    if(target->type != ONEDSTRING)
        return(WRONG_VAR_TYPE);

    if(!(wc_target = malloc(target->length*sizeof(wchar_t))))
        return(MEMORY_ALLOC_ERROR);

    if((ret = getStringFrom1DArray(inanker, group, name, wc_target,
                    target->length, x_index)) != 0)
    {
        free(wc_target);
        return(ret);
    }
    

    length = wcslen(wc_target);

    if((ret = add1DStringArray(outanker, NULL, new_name, 2, length)) != 0)
    {
        free(wc_target);
        return(ret);
    }

    memset(buff, 0x00, sizeof(buff));
    for(x=0; x < length; x++)
    {
        memcpy(buff, wc_target+x, sizeof(wchar_t));
        if((ret = edit1DStringArray(outanker, NULL, new_name, buff, x)) != 0)
        {
            free(wc_target);
            return(ret);
        }
    }
    free(wc_target);

    return(0);
}

int add2DStringArray(vars_t *anker, char *group, char *name, int var_length, 
                     int x_length, int y_length)
{
    vars_t *new = NULL;
    int ret = 0;
    size_t size = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = TWODSTRING;
    new->length = var_length+1;
    new->x_length = x_length;
    new->y_length = y_length;
    size = (((var_length+1)*sizeof(wchar_t))*(y_length))*(x_length);
    if((new->data = malloc(size)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    return(0);
}


int edit2DStringArray(vars_t *anker, char *group, char *name, wchar_t *val, int x_index, int y_index)
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

    if(wcslen(val)+1 >= target->length)
        return(VAR_VALUE_TO_LONG);        

    if(x_index >= target->x_length) 
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length) 
        return(Y_INDEX_OUT_OF_RANGE);

    offset = (target->y_length*(target->length*sizeof(wchar_t)));
    offset = offset*(x_index)+((target->length*sizeof(wchar_t))*y_index);

    wcscpy((wchar_t*)(target->data+offset), val);
    return(0);
}

int getStringFrom2DArray(vars_t *anker, char *group, char *name, wchar_t *val,
                         int length, int x_index, int y_index)
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

    if(target->type != ONEDSTRING)
        return(WRONG_VAR_TYPE);

    if(target->x_length >= x_index)
        return(X_INDEX_OUT_OF_RANGE);

    if(target->y_length >= y_index)
        return(Y_INDEX_OUT_OF_RANGE);

    if(wcslen((wchar_t*)target->data) >= length)
        return(BUFFER_TO_SAMLL);

    offset = (target->y_length*(target->length*sizeof(wchar_t)));
    offset = offset*(x_index)+((target->length*sizeof(wchar_t))*y_index);

    wcscpy(val, (wchar_t*)(target->data+offset));
    return(0);
}

int create1DStringArrayFrom2DStringArray(vars_t *inanker, vars_t *outanker,
                                       char *group, char *name, char *new_name,
                                       int x_index)
{
    vars_t *target = NULL,
           *grp = NULL;
    int y = 0;
    size_t offset = 0;

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

    if(target->type != TWODSTRING)
        return(WRONG_VAR_TYPE);

    for(y=0; y < target->y_length; y++)
    {
        offset = (target->y_length*(target->length*sizeof(wchar_t)));
        offset = offset*(x_index)+((target->length*sizeof(wchar_t))*(y));

    }
    return(0);
}



int add3DStringArray(vars_t *anker, char *group, char *name, int var_length, 
                     int x_length, int y_length, int z_length)
{
    vars_t *new = NULL;
    int ret = 0;
    size_t size = 0;

    if((ret = addNewVar(anker, &new, group, name)) != 0)
        return(ret);

    new->type = THREEDSTRING;
    new->length = var_length+1;
    new->x_length = x_length;
    new->y_length = y_length;
    new->z_length = z_length;

    size = x_length * y_length * z_length *sizeof(wchar_t);
    size = size*(var_length+1);

    if((new->data = malloc(size)) == NULL)
    {
        free(new->name);
        free(new);
        return(MEMORY_ALLOC_ERROR);
    }

    return(0);
}



int edit3DStringArray(vars_t *anker, char *group, char *name, wchar_t *val,
                      int x_index, int y_index, int z_index)
{
    vars_t *target = NULL,
           *grp = NULL;
    size_t offset = 0, var_size = 0, sizeofy = 0, sizeofz = 0;

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

    if(wcslen(val)+1 >= target->length)
        return(VAR_VALUE_TO_LONG);        

    if(x_index >= target->x_length) 
        return(X_INDEX_OUT_OF_RANGE);
    if(y_index >= target->y_length) 
        return(Y_INDEX_OUT_OF_RANGE);
    if(z_index >= target->z_length) 
        return(Z_INDEX_OUT_OF_RANGE);


    var_size = target->length*sizeof(wchar_t);
    sizeofz = var_size*(target->z_length);
    sizeofy = sizeofz*(target->y_length);

    offset = (x_index*sizeofy)+(y_index*sizeofz)+(var_size*z_index);

    wcscpy((wchar_t*)(target->data+offset), val);
    return(0);
}

int getStringFrom3DArray(vars_t *anker, char *group, char *name, wchar_t *val,
                         int length, int x_index, int y_index, int z_index)
{
    vars_t *target = NULL,
           *grp = NULL;
    size_t offset = 0,
           sizeofy = 0,
           sizeofz = 0,
           var_size = 0;

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

    if(target->type != THREEDSTRING)
        return(WRONG_VAR_TYPE);

    if(target->x_length >= x_index)
        return(X_INDEX_OUT_OF_RANGE);

    if(target->y_length >= y_index)
        return(Y_INDEX_OUT_OF_RANGE);

    if(target->z_length >= z_index)
        return(Z_INDEX_OUT_OF_RANGE);

    if(wcslen((wchar_t*)target->data) >= length)
        return(BUFFER_TO_SAMLL);

    var_size = target->length*sizeof(wchar_t);
    sizeofz = var_size*(target->z_length);
    sizeofy = sizeofz*(target->y_length);

    offset = (x_index*sizeofy)+(y_index*sizeofz)+(var_size*z_index);

    wcscpy(val, (wchar_t*)(target->data+offset));
    return(0);
}
