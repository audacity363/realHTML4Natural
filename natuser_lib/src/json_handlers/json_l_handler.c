#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "natuser.h"
#include "natni.h"

#include "standard.h"

#include "vars.h"
#include "rh4n_json_mapper.h"


int json_l_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0;
    
    char val = 0;

    val = *((bool*)target->data);

    printf("Boolval: %#2x\n", val);
    printf("Bool length: [%d]\n", natvar_pd.length);

    if((rc = nnifuncs->pf_nni_put_parm(nnifuncs, natparm_pos, parmhandle, LSIZE, &val)) != NNI_RC_OK) {
        return(RH4N_RET_NNI_ERR);
    }

    return(RH4N_RET_OK);
}

int json_1dl_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0, occ[3] = {0, 0, 0}, x = 0;

    char val = 0;

    if(natvar_pd.flags & NNI_FLG_UBVAR_0) {
        occ[0] = target->x_length;
        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK) {
            return(RH4N_RET_NNI_ERR);
        }
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    }

    for(; x < target->x_length; x++) {
        occ[0] = x;
        val = ((bool*)target->data)[x];

        if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, LSIZE, &val, occ)) != NNI_RC_OK) {
            return(RH4N_RET_NNI_ERR);
        }
    }


    return(RH4N_RET_OK);
}

int json_2dl_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0, occ[3] = {0, 0, 0}, x = 0, y = 0, offset = 0;

    char val = 0;

    if(natvar_pd.flags & NNI_FLG_UBVAR_0 && natvar_pd.flags & NNI_FLG_UBVAR_1) {
        occ[0] = target->x_length;
        occ[1] = target->y_length;

        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK) {
            return(RH4N_RET_NNI_ERR);
        }
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    } else if(natvar_pd.occurrences[1] < target->y_length) {
        return(RH4N_RET_DIM2_TOO_SMALL);
    }

    for(; x < target->x_length; x++) {
        occ[0] = x;
        for(y=0; y < target->y_length; y++) {
            occ[1] = y;
            offset = (x*target->y_length)+y;
            val = ((bool*)target->data)[offset];

            if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, LSIZE, &val, occ)) != NNI_RC_OK) {
                return(RH4N_RET_NNI_ERR);
            }
        }
    }

    return(RH4N_RET_OK);
}

int json_3dl_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0, occ[3] = {0, 0, 0}, x = 0, y = 0, z = 0, offset = 0;

    char val = 0;

    if(natvar_pd.flags & NNI_FLG_UBVAR_0 && natvar_pd.flags & NNI_FLG_UBVAR_1) {
        occ[0] = target->x_length;
        occ[1] = target->y_length;

        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK) {
            return(RH4N_RET_NNI_ERR);
        }
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    } else if(natvar_pd.occurrences[1] < target->y_length) {
        return(RH4N_RET_DIM2_TOO_SMALL);
    }

    for(; x < target->x_length; x++) {
        occ[0] = x;
        for(y=0; y < target->y_length; y++) {
            occ[1] = y;
            for(z=0; z < target->z_length; z++) {
                occ[2] = z;
                offset = (z*target->x_length * target->y_length);
                offset += (y*target->x_length) + x;
                val = ((bool*)target->data)[offset];

                if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, LSIZE, &val, occ)) != NNI_RC_OK) {
                    return(RH4N_RET_NNI_ERR);
                }
            }
        }
    }

    return(RH4N_RET_OK);
}
