#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "natuser.h"
#include "natni.h"
#include "vars.h"
#include "rh4n.h"

int handleI1Variable(int, void *, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI11DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI12DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI13DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI2Variable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI21DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI22DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI23DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI4Variable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI41DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI42DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int handleI43DVariable(int, void*, pnni_611_functions, struct parameter_description, char*, vars_t*);
int shiftInt(int, int);


int handleIntegerVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    if((pd.flags & NNI_FLG_XARRAY) && pd.length_all == 0)
        return(RH4N_XARRAY_ERROR);

    if(pd.length == 1 && pd.dimensions == 0)
        return(handleI1Variable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 1 && pd.dimensions == 1)
        return(handleI11DVariable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 1 && pd.dimensions == 2)
        return(handleI12DVariable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 1 && pd.dimensions == 3)
        return(handleI13DVariable(index, parmhandle, nni_funcs, pd, name, anker));

    else if(pd.length == 2 && pd.dimensions == 0)
        return(handleI2Variable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 2 && pd.dimensions == 1)
        return(handleI21DVariable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 2 && pd.dimensions == 2)
        return(handleI22DVariable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 2 && pd.dimensions == 3)
        return(handleI23DVariable(index, parmhandle, nni_funcs, pd, name, anker));

    else if(pd.length == 4 && pd.dimensions == 0)
        return(handleI4Variable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 4 && pd.dimensions == 1)
        return(handleI41DVariable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 4 && pd.dimensions == 2)
        return(handleI42DVariable(index, parmhandle, nni_funcs, pd, name, anker));
    else if(pd.length == 4 && pd.dimensions == 3)
        return(handleI43DVariable(index, parmhandle, nni_funcs, pd, name, anker));

    return(RH4N_UNKOWN_INT_TYPE);
}

int shiftI1toInt(NATTYP_I1 val)
{
    int result = 0;
    result = val >> 24;
    return(result);
}

int handleI1Variable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I1 i1_buff = 0;
    int i_buff = 0, ret = 0;

    ret = nni_funcs->pf_nni_get_parm(nni_funcs, index, parmhandle, sizeof(NATTYP_I1), &i1_buff);
    if(checkNNIReturnCode(ret) != NNI_RC_OK)
        return(ret*-1);

    if(addInteger(anker, NULL, name, i1_buff) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    return(0);
}

int handleI11DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I1 i1_buff = 0;
    int i_buff = 0, ret = 0, x = 0, occ[3];

    if(add1DIntegerArray(anker, NULL, name, pd.occurrences[0]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
    {
        occ[0] = x; occ[1] = occ[2] = 0;
        ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I1), &i1_buff, occ);
        if(checkNNIReturnCode(ret) != NNI_RC_OK)
            return(ret*-1);

        if(edit1DIntegerArray(anker, NULL, name, i1_buff, x) < 0)
            return(RH4N_VAR_LIBRARY_ERR);
    }

    return(0);
}

int handleI12DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I1 i1_buff = 0;
    int i_buff = 0, ret = 0, x = 0, y = 0, occ[3];

    if(add2DIntegerArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
        for(y=0; y < pd.occurrences[1]; y++)
        {
            occ[0] = x; occ[1] = y; occ[2] = 0;
            ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I1), &i1_buff, occ);
            if(checkNNIReturnCode(ret) != NNI_RC_OK)
                return(ret*-1);

            if(edit2DIntegerArray(anker, NULL, name, i1_buff, x, y) < 0)
                return(RH4N_VAR_LIBRARY_ERR);
        }

    return(0);
}

int handleI13DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I1 i1_buff = 0;
    int i_buff = 0, ret = 0, x = 0, y = 0, z = 0, occ[3];

    if(add3DIntegerArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1], pd.occurrences[2]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
        for(y=0; y < pd.occurrences[1]; y++)
            for(z=0; z < pd.occurrences[2]; z++)
            {
                occ[0] = x; occ[1] = y; occ[2] = z;
                ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I1), &i1_buff, occ);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    return(ret*-1);

                if(edit3DIntegerArray(anker, NULL, name, i1_buff, x, y, z) < 0)
                    return(RH4N_VAR_LIBRARY_ERR);
            }

    return(0);
}

int handleI2Variable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I2 i2_buff = 0;
    int i_buff = 0, ret = 0;

    ret = nni_funcs->pf_nni_get_parm(nni_funcs, index, parmhandle, sizeof(NATTYP_I2), &i2_buff);
    if(checkNNIReturnCode(ret) != NNI_RC_OK)
        return(ret*-1);

    if(addInteger(anker, NULL, name, i2_buff) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    return(0);
}

int handleI21DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I2 i2_buff = 0;
    int i_buff = 0, ret = 0, x = 0, occ[3];

    if(add1DIntegerArray(anker, NULL, name, pd.occurrences[0]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
    {
        occ[0] = x; occ[1] = occ[2] = 0;
        ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I2), &i2_buff, occ);
        if(checkNNIReturnCode(ret) != NNI_RC_OK)
            return(ret*-1);

        if(edit1DIntegerArray(anker, NULL, name, i2_buff, x) < 0)
            return(RH4N_VAR_LIBRARY_ERR);
    }

    return(0);
}

int handleI22DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I2 i2_buff = 0;
    int i_buff = 0, ret = 0, x = 0, y = 0, occ[3];

    if(add2DIntegerArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
        for(y=0; y < pd.occurrences[1]; y++)
        {
            occ[0] = x; occ[1] = y; occ[2] = 0;
            ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I2), &i2_buff, occ);
            if(checkNNIReturnCode(ret) != NNI_RC_OK)
                return(ret*-1);

            if(edit2DIntegerArray(anker, NULL, name, i2_buff, x, y) < 0)
                return(RH4N_VAR_LIBRARY_ERR);
        }

    return(0);
}

int handleI23DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I2 i2_buff = 0;
    int i_buff = 0, ret = 0, x = 0, y = 0, z = 0, occ[3];

    if(add3DIntegerArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1], pd.occurrences[2]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
        for(y=0; y < pd.occurrences[1]; y++)
            for(z=0; z < pd.occurrences[2]; z++)
            {
                occ[0] = x; occ[1] = y; occ[2] = z;
                ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I2), &i2_buff, occ);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    return(ret*-1);

                if(edit3DIntegerArray(anker, NULL, name, i2_buff, x, y, z) < 0)
                    return(RH4N_VAR_LIBRARY_ERR);
            }

    return(0);
}

int handleI4Variable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I4 i4_buff = 0;
    int i_buff = 0, ret = 0;

    ret = nni_funcs->pf_nni_get_parm(nni_funcs, index, parmhandle, sizeof(NATTYP_I4), &i4_buff);
    if(checkNNIReturnCode(ret) != NNI_RC_OK)
        return(ret*-1);

    if(addInteger(anker, NULL, name, i4_buff) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    return(0);
}

int handleI41DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I4 i4_buff = 0;
    int i_buff = 0, ret = 0, x = 0, occ[3];

    if(add1DIntegerArray(anker, NULL, name, pd.occurrences[0]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
    {
        occ[0] = x; occ[1] = occ[2] = 0;
        ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I4), &i4_buff, occ);
        if(checkNNIReturnCode(ret) != NNI_RC_OK)
            return(ret*-1);

        if(edit1DIntegerArray(anker, NULL, name, i4_buff, x) < 0)
            return(RH4N_VAR_LIBRARY_ERR);
    }

    return(0);
}

int handleI42DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I4 i4_buff = 0;
    int i_buff = 0, ret = 0, x = 0, y = 0, occ[3];

    if(add2DIntegerArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
        for(y=0; y < pd.occurrences[1]; y++)
        {
            occ[0] = x; occ[1] = y; occ[2] = 0;
            ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I4), &i4_buff, occ);
            if(checkNNIReturnCode(ret) != NNI_RC_OK)
                return(ret*-1);

            if(edit2DIntegerArray(anker, NULL, name, i4_buff, x, y) < 0)
                return(RH4N_VAR_LIBRARY_ERR);
        }

    return(0);
}

int handleI43DVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    NATTYP_I4 i4_buff = 0;
    int i_buff = 0, ret = 0, x = 0, y = 0, z = 0, occ[3];

    if(add3DIntegerArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1], pd.occurrences[2]) < 0)
        return(RH4N_VAR_LIBRARY_ERR);

    for(x=0; x < pd.occurrences[0]; x++)
        for(y=0; y < pd.occurrences[1]; y++)
            for(z=0; z < pd.occurrences[2]; z++)
            {
                occ[0] = x; occ[1] = y; occ[2] = z;
                ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sizeof(NATTYP_I4), &i4_buff, occ);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    return(ret*-1);

                if(edit3DIntegerArray(anker, NULL, name, i4_buff, x, y, z) < 0)
                    return(RH4N_VAR_LIBRARY_ERR);
            }

    return(0);
}

int shiftInt(int input, int length)
{
    fprintf(logfile, "Working with: [%d]\n", input);
    int result = 0;
    switch(length)
    {
        case 1:
            result = input >> 24;
            break;
        case 2:
            result = input >> 16;
            break;
        case 4:
            result = input;
            break;
    }
    return(result);
}
