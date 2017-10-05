#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "natuser.h"
#include "natni.h"
#include "vars.h"
#include "rh4n.h"
#include "utils.h"

int handleUnicodeVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pb, char *name, vars_t *anker)
{
    char *c_tmp_buff = NULL;

    wchar_t *wc_buffer = NULL;
    int ret = 0, i = 0, x = 0, y = 0, z = 0, dyn_flag = 0,
        length = 0, max_length = 0, occ[3];

    if((pb.flags & NNI_FLG_XARRAY) && pb.occurrences[0] == 0)
        return(RH4N_XARRAY_ERROR);

    dyn_flag = pb.flags & NNI_FLG_DYNVAR;

    if(pb.dimensions == 0)
    {
        if((c_tmp_buff = malloc(pb.length_all+1)) == NULL)
        {
            return(RH4N_NO_MEMORY);
        }

        if((wc_buffer = malloc((pb.length_all+1)*sizeof(wchar_t))) == NULL)
        {
            free(c_tmp_buff);;
            return(RH4N_NO_MEMORY);
        }
        memset(wc_buffer, 0x00, (pb.length_all+1)*sizeof(wchar_t));

        ret = nni_funcs->pf_nni_get_parm(nni_funcs, index, parmhandle,
                    pb.length_all, c_tmp_buff);
        if(checkNNIReturnCode(ret) != NNI_RC_OK)
        {
            free(c_tmp_buff);
            free(wc_buffer);
            return(ret*-1);
        }

        convert2Bto4BString(c_tmp_buff, wc_buffer, pb.length);
        trimSpaces4B(wc_buffer);

        if((ret = addString(anker, NULL, name, wc_buffer, pb.length_all)) < 0)
        {
            free(c_tmp_buff);
            free(wc_buffer);
            return(RH4N_VAR_LIBRARY_ERR);
        }
    }
    else if(pb.dimensions == 1)
    {
        if(dyn_flag)
        {
            //The variable is an dynamic variable and we first have to get the longest length
            for(x=0; x < pb.occurrences[0]; x++)
            {
                occ[0] = x; occ[1] = occ[2] = 0;
                ret = nni_funcs->pf_nni_get_parm_array_length(nni_funcs, index, parmhandle, &length, occ);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                {
                    return(ret*-1);
                }
                if(length > max_length)
                    max_length = length;
            }
        }
        else
            max_length = pb.length;


        if((c_tmp_buff = malloc(max_length+1)) == NULL)
        {
            return(RH4N_NO_MEMORY);
        }

        if((wc_buffer = malloc((max_length+1)*sizeof(wchar_t))) == NULL)
        {
            free(c_tmp_buff);
            return(RH4N_NO_MEMORY);
        }

        if((ret = add1DStringArray(anker, NULL, name, max_length+1, pb.occurrences[0])) != 0)
        {
            free(c_tmp_buff);
            free(wc_buffer);
            return(RH4N_VAR_LIBRARY_ERR);
        }
        for(x=0; x < pb.occurrences[0]; x++)
        {
            memset(wc_buffer, 0x00, (max_length+1)*sizeof(wchar_t));
            memset(c_tmp_buff, 0x00, max_length+1);

            occ[0] = x; occ[1] = occ[2] = 0;
            ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle,
                        max_length, c_tmp_buff, occ);
            if(checkNNIReturnCode(ret) != NNI_RC_OK)
            {
                free(c_tmp_buff);
                free(wc_buffer);
                return(ret*-1);
            }

            convert2Bto4BString(c_tmp_buff, wc_buffer, strlen(c_tmp_buff)+1);
            trimSpaces4B(wc_buffer);

            if((ret = edit1DStringArray(anker, NULL, name, wc_buffer, x)) != 0)
            {
                fprintf(logfile, "Varhandle error: [%d]\n", ret);
                free(c_tmp_buff);
                free(wc_buffer);
                return(RH4N_VAR_LIBRARY_ERR);
            }
        }
    }
    else if(pb.dimensions == 2)
    {
        if(dyn_flag)
        {
            //The variable is an dynamic variable and we first have to get the longest length
            for(x=0; x < pb.occurrences[0]; x++)
            {
                for(y=0; y < pb.occurrences[1]; y++)
                {
                    occ[0] = x; occ[1] = y; occ[2] = 0;
                    ret = nni_funcs->pf_nni_get_parm_array_length(nni_funcs, index, parmhandle, &length, occ);
                    if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    {
                        return(ret*-1);
                    }
                    if(length > max_length)
                        max_length = length;
                }
            }
        }
        else
        {
            max_length = pb.length;
        }

        if((c_tmp_buff = malloc(max_length+1)) == NULL)
        {
            return(RH4N_NO_MEMORY);
        }

        if((wc_buffer = malloc((max_length+1)*sizeof(wchar_t))) == NULL)
        {
            free(c_tmp_buff);
            return(RH4N_NO_MEMORY);
        }
        memset(wc_buffer, 0x00, (max_length+1)*sizeof(wchar_t));

        if((ret = add2DStringArray(anker, NULL, name, max_length+1, pb.occurrences[0], pb.occurrences[1])) != 0)
        {
            free(c_tmp_buff);
            free(wc_buffer);
            return(RH4N_VAR_LIBRARY_ERR);
        }

        for(x=0; x < pb.occurrences[0]; x++)
        {
            for(y=0; y < pb.occurrences[1]; y++)
            {

                memset(wc_buffer, 0x00, (max_length+1)*sizeof(wchar_t));
                memset(c_tmp_buff, 0x00, max_length+1);

                occ[0] = x; occ[1] = y; occ[2] = 0;
                ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle,
                            max_length, c_tmp_buff, occ);
                if(checkNNIReturnCode(ret) != NNI_RC_OK)
                {
                    free(c_tmp_buff);
                    free(wc_buffer);
                    return(ret*-1);
                }

                
                convert2Bto4BString(c_tmp_buff, wc_buffer, strlen(c_tmp_buff)+1);
                trimSpaces4B(wc_buffer);

                if((ret = edit2DStringArray(anker, NULL, name, wc_buffer, x, y)) != 0)
                {
                    fprintf(logfile, "Varhandle error: [%d]\n", ret);
                    free(c_tmp_buff);
                    free(wc_buffer);
                    return(RH4N_VAR_LIBRARY_ERR);
                }

            }
        }

    }
    else if(pb.dimensions == 3)
    {
        if(dyn_flag)
        {
            //The variable is an dynamic variable and we first have to get the longest length
            for(x=0; x < pb.occurrences[0]; x++)
                for(y=0; y < pb.occurrences[1]; y++)
                    for(z=0; z < pb.occurrences[2]; z++)
                    {
                        occ[0] = x; occ[1] = y; occ[2] = z;
                        ret = nni_funcs->pf_nni_get_parm_array_length(nni_funcs, index, parmhandle, &length, occ);
                        if(checkNNIReturnCode(ret) != NNI_RC_OK)
                        {
                            return(ret*-1);
                        }
                        if(length > max_length)
                            max_length = length;
                    }
        }
        else
            max_length = pb.length;


        if((c_tmp_buff = malloc(max_length+1)) == NULL)
        {
            return(RH4N_NO_MEMORY);
        }

        if((wc_buffer = malloc((max_length+1)*sizeof(wchar_t))) == NULL)
        {
            free(c_tmp_buff);;
            return(RH4N_NO_MEMORY);
        }
        memset(wc_buffer, 0x00, (max_length+1)*sizeof(wchar_t));

        if((ret = add3DStringArray(anker, NULL, name, max_length+1, pb.occurrences[0], pb.occurrences[1], pb.occurrences[2])) != 0)
        {
            free(c_tmp_buff);
            free(wc_buffer);
            return(RH4N_VAR_LIBRARY_ERR);
        }

        for(x=0; x < pb.occurrences[0]; x++)
        {
            for(y=0; y < pb.occurrences[1]; y++)
            {
                for(z=0; z < pb.occurrences[2]; z++)
                {

                    memset(wc_buffer, 0x00, (max_length+1)*sizeof(wchar_t));
                    memset(c_tmp_buff, 0x00, max_length+1);

                    occ[0] = x; occ[1] = y; occ[2] = z;
                    ret = nni_funcs->pf_nni_get_parm_array(nni_funcs, index, parmhandle,
                                max_length, c_tmp_buff, occ);
                    if(checkNNIReturnCode(ret) != NNI_RC_OK)
                    {
                        free(c_tmp_buff);
                        free(wc_buffer);
                        return(ret*-1);
                    }

                    convert2Bto4BString(c_tmp_buff, wc_buffer, strlen(c_tmp_buff)+1);
                    trimSpaces4B(wc_buffer);

                    if((ret = edit3DStringArray(anker, NULL, name, wc_buffer, x, y, z)) != 0)
                    {
                        fprintf(logfile, "Varhandle error: [%d]\n", ret);
                        free(c_tmp_buff);
                        free(wc_buffer);
                        return(RH4N_VAR_LIBRARY_ERR);
                    }

                }
            }
        }

    }

    free(c_tmp_buff);
    free(wc_buffer);
    return(NNI_RC_OK);

    return(0);
}
