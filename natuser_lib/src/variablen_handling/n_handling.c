#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "natuser.h"
#include "natni.h"
#include "vars.h"
#include "rh4n.h"
#include "hexdump.h"

int handleNumericVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                          struct parameter_description pd, char *name, vars_t *anker)
{

    void *nat_buff = NULL;
    char *c_buff = NULL, strbuff[8];
    wchar_t *wc_buff = NULL;

    int sum_length = pd.byte_length,
        ret = 0,
        x=0, y=0, z=0,
        occ[3];


    if((pd.flags & NNI_FLG_XARRAY) && pd.length_all == 0)
        return(RH4N_XARRAY_ERROR);

    if((nat_buff = malloc(sum_length)) == NULL)
        return(RH4N_NO_MEMORY);

    if((c_buff = malloc(sum_length+3)) == NULL)
    {
        free(nat_buff);
        return(RH4N_NO_MEMORY);
    }
    memset(c_buff, 0x00, sum_length+3);

    if((wc_buff = malloc((sum_length+3)*sizeof(wchar_t))) == NULL)
    {
        free(nat_buff);
        free(c_buff);
        return(RH4N_NO_MEMORY);
    }
    memset(wc_buff, 0x00, (sum_length+3)*sizeof(wchar_t));


    if(pd.dimensions == 0)
    {
        ret = nni_funcs->pf_nni_get_parm(nni_funcs, index, parmhandle, sum_length, nat_buff);
        if(checkNNIReturnCode(ret) != NNI_RC_OK)
        {
            ret = ret*-1;
            goto end;
        }


        ret = nni_funcs->pf_nni_to_string(nni_funcs, sum_length, nat_buff, NNI_TYPE_NUM,
                sum_length, pd.precision, sum_length+3, c_buff);
        if(checkNNIReturnCode(ret) != NNI_RC_OK)
        {
            ret = ret*-1;
            goto end;
        }

        trimleadingSpaces(c_buff);

        
        convert1Bto4BString(c_buff, wc_buff, sum_length+3);

        if((ret = addString(anker, NULL, name, wc_buff, wcslen(wc_buff))) < 0)
        {
            ret = RH4N_VAR_LIBRARY_ERR;
            goto end;
        }
    }
    else if(pd.dimensions == 1)
    {

        if((ret = add1DStringArray(anker, NULL, name, sum_length+3, pd.occurrences[0])) != 0)
        {
            ret  = RH4N_VAR_LIBRARY_ERR;
            goto end;
        }

        for(x=0; x < pd.occurrences[0]; x++)
        {
            occ[0] = x; occ[1] = occ[2] = 0;

            ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sum_length, nat_buff, occ);
            if(checkNNIReturnCode(ret) != NNI_RC_OK)
            {
                ret = ret*-1;
                goto end;
            }


            ret = nni_funcs->pf_nni_to_string(nni_funcs, sum_length, nat_buff, NNI_TYPE_NUM,
                    sum_length, pd.precision, sum_length+3, c_buff);
            if(checkNNIReturnCode(ret) != NNI_RC_OK)
            {
                ret = ret*-1;
                goto end;
            }

            trimleadingSpaces(c_buff);

            
            convert1Bto4BString(c_buff, wc_buff, sum_length+3);

            if((ret = edit1DStringArray(anker, NULL, name, wc_buff, x)) != 0)
            {
                ret = RH4N_VAR_LIBRARY_ERR;
                goto end;
            }
        }
    }
    else if(pd.dimensions == 2)
    {

        if((ret = add2DStringArray(anker, NULL, name, sum_length+3, pd.occurrences[0], pd.occurrences[1])) != 0)
        {
            ret = RH4N_VAR_LIBRARY_ERR;
            goto end;
        }

        for(x=0; x < pd.occurrences[0]; x++)
            for(y=0; y < pd.occurrences[1]; y++)
            {
                occ[0] = x; occ[1] = y; occ[2] = 0;

                ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sum_length, nat_buff, occ);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                {
                    ret = ret*-1;
                    goto end;
                }


                ret = nni_funcs->pf_nni_to_string(nni_funcs, sum_length, nat_buff, NNI_TYPE_NUM,
                        sum_length, pd.precision, sum_length+3, c_buff);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                {
                    ret = ret*-1;
                    goto end;
                }

                trimleadingSpaces(c_buff);
                
                convert1Bto4BString(c_buff, wc_buff, sum_length+3);

                if((ret = edit2DStringArray(anker, NULL, name, wc_buff, x, y)) != 0)
                {
                    ret = ret*-1;
                    goto end;
                }
            }
    }
    else if(pd.dimensions == 3)
    {

        if((ret = add3DStringArray(anker, NULL, name, sum_length+3, pd.occurrences[0], pd.occurrences[1],
                    pd.occurrences[2])) != 0)
        {
            ret = RH4N_VAR_LIBRARY_ERR;
            goto end;
        }

        for(x=0; x < pd.occurrences[0]; x++)
            for(y=0; y < pd.occurrences[1]; y++)
                for(z=0; z < pd.occurrences[2]; z++)
                {
                    occ[0] = x; occ[1] = y; occ[2] = z;

                    ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle, sum_length, nat_buff, occ);
                    if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    {
                        ret = ret*-1;
                        goto end;
                    }


                    ret = nni_funcs->pf_nni_to_string(nni_funcs, sum_length, nat_buff, NNI_TYPE_NUM,
                            sum_length, pd.precision, sum_length+3, c_buff);
                    if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    {
                        ret = ret*-1;
                        goto end;
                    }

                    trimleadingSpaces(c_buff);

                    
                    convert1Bto4BString(c_buff, wc_buff, sum_length+3);

                    if((ret = edit3DStringArray(anker, NULL, name, wc_buff, x, y, z)) != 0)
                    {
                        ret = RH4N_VAR_LIBRARY_ERR;
                        goto end;
                    }
                }
    }

end:

    free(nat_buff);
    free(c_buff);
    free(wc_buff);

    return(ret);
}
