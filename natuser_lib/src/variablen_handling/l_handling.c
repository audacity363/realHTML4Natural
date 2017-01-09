#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "natuser.h"
#include "natni.h"
#include "vars.h"
#include "rh4n.h"

bool convertToBool(char res)
{
    if(res == NNI_L_TRUE)
        return(true);
    return(false);
}

int handleLogicVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pd, char *name, vars_t *anker)
{
    int ret = 0, x, y, z, occ[3];
    char result = 0;
    bool b_buff = false;

    fprintf(logfile, "Bool length: [%d]\n", pd.byte_length);

    if(pd.dimensions == 0)
    {
        ret = nni_funcs->pf_nni_get_parm(nni_funcs, index, parmhandle, NAT_BOOL_SIZE, &result);
        if(checkNNIReturnCode(ret) != NNI_RC_OK)
            return(ret);

        b_buff = convertToBool(result);

        ret = addBoolean(anker, NULL, name, b_buff);
        if(ret < 0)
            return(RH4N_VAR_LIBRARY_ERR);

    }
    else if(pd.dimensions == 1)
    {
        if(add1DBooleanArray(anker, NULL, name, pd.occurrences[0]) < 0)
            return(RH4N_VAR_LIBRARY_ERR);


        for(x=0; x < pd.occurrences[0]; x++)
        {
            occ[0] = x; occ[1] = occ[2] = 0;
            ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, NAT_BOOL_SIZE, &result, occ);
            if(checkNNIReturnCode(ret) != NNI_RC_OK)
                return(ret);

            b_buff = convertToBool(result);

            ret = edit1DBooleanArray(anker, NULL, name, b_buff, x);
            if(ret < 0)
                return(RH4N_VAR_LIBRARY_ERR);

        }
    }
    else if(pd.dimensions == 2)
    {
        if(add2DBooleanArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1]) < 0)
            return(RH4N_VAR_LIBRARY_ERR);


        for(x=0; x < pd.occurrences[0]; x++)
            for(y=0; y < pd.occurrences[1]; y++)
            {
                occ[0] = x; occ[1] = y; occ[2] = 0;
                ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, NAT_BOOL_SIZE, &result, occ);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    return(ret);

                b_buff = convertToBool(result);

                ret = edit2DBooleanArray(anker, NULL, name, b_buff, x, y);
                if(ret < 0)
                    return(RH4N_VAR_LIBRARY_ERR);

            }
    }
    else if(pd.dimensions == 3)
    {
        if(add3DBooleanArray(anker, NULL, name, pd.occurrences[0], pd.occurrences[1], pd.occurrences[2]) < 0)
            return(RH4N_VAR_LIBRARY_ERR);


        for(x=0; x < pd.occurrences[0]; x++)
            for(y=0; y < pd.occurrences[1]; y++)
                for(z=0; z < pd.occurrences[2]; z++)
                {
                    occ[0] = x; occ[1] = y; occ[2] = z;
                    ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, NAT_BOOL_SIZE, &result, occ);
                    if(checkNNIReturnCode(ret) != NNI_RC_OK)
                        return(ret);

                    b_buff = convertToBool(result);

                    ret = edit3DBooleanArray(anker, NULL, name, b_buff, x, y, z);
                    if(ret < 0)
                        return(RH4N_VAR_LIBRARY_ERR);

                }
    }
    return(0);
}
