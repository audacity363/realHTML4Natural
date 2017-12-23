#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "natuser.h"
#include "natni.h"

#include "standard.h"

#include "vars.h"
#include "rh4n_json_mapper.h"

int json_i_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0;
    NATTYP_I1 i1_val = 0;
    NATTYP_I2 i2_val = 0;
    NATTYP_I4 i4_val = 0;


    switch(natvar_pd.length) {
        case I1SIZE:
            if((int)target->data > MAXI1)
                return(RH4N_RET_BUFFER_OVERFLOW);
            i1_val = (NATTYP_I1)target->data;
            if((rc = nnifuncs->pf_nni_put_parm(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I1), &i1_val)) != NNI_RC_OK) 
                return(RH4N_RET_NNI_ERR);
            break;
        case I2SIZE:
            if((int)target->data > MAXI2)
                return(RH4N_RET_BUFFER_OVERFLOW);
            i2_val = (NATTYP_I2)target->data;
            if((rc = nnifuncs->pf_nni_put_parm(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I2), &i2_val)) != NNI_RC_OK) 
                return(RH4N_RET_NNI_ERR);
            break;
        case I4SIZE:
            if((int)target->data > MAXI4)
                return(RH4N_RET_BUFFER_OVERFLOW);
            i4_val = (NATTYP_I4)target->data;
            if((rc = nnifuncs->pf_nni_put_parm(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I4), &i4_val)) != NNI_RC_OK) 
                return(RH4N_RET_NNI_ERR);
            break;
    }
    return(RH4N_RET_OK);

}


int json_1di_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0, i = 0, x_array_flag = 0, occ[3] = {0, 0, 0}, val = 0;
    NATTYP_I1 i1_val = 0;
    NATTYP_I2 i2_val = 0;
    NATTYP_I4 i4_val = 0;

    if(natvar_pd.flags & IF4_FLG_UBVAR_0) {
        occ[0] = target->x_length;
        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK)
            return(RH4N_RET_NNI_ERR);
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    }

    for(; i < target->x_length; i++) {
        occ[0] = i;
        val = ((int*)target->data)[i];
        printf("i = [%d] = [%d]\n", i, val);
        switch(natvar_pd.length) {
            case I1SIZE:
                if(val > MAXI1)
                    return(RH4N_RET_BUFFER_OVERFLOW);
                i1_val = (NATTYP_I1)val;
                if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I1), 
                        &i1_val, occ)) != NNI_RC_OK) {
                    return(RH4N_RET_NNI_ERR);
                }
                break;
            case I2SIZE:
                if(val > MAXI2)
                    return(RH4N_RET_BUFFER_OVERFLOW);
                i2_val = (NATTYP_I2)val;
                if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I2), 
                        &i2_val, occ)) != NNI_RC_OK) {
                    return(RH4N_RET_NNI_ERR);
                }
                break;
            case I4SIZE:
                if(val > MAXI4)
                    return(RH4N_RET_BUFFER_OVERFLOW);
                i4_val = (NATTYP_I4)val;
                if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I4), 
                        &i4_val, occ)) != NNI_RC_OK) {
                    return(RH4N_RET_NNI_ERR);
                }
                break;
        }
    }
    return(RH4N_RET_OK);
}

int json_2di_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0, i = 0, y = 0, x_array_flag = 0, occ[3] = {0, 0, 0}, val = 0,
        offset = 0;
    NATTYP_I1 i1_val = 0;
    NATTYP_I2 i2_val = 0;
    NATTYP_I4 i4_val = 0;

    if(natvar_pd.flags & IF4_FLG_UBVAR_0 && natvar_pd.flags & IF4_FLG_UBVAR_1) {
        occ[0] = target->x_length;
        occ[1] = target->y_length;
        printf("Setting array size\n");
        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK)
            return(RH4N_RET_NNI_ERR);
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    } else if(natvar_pd.occurrences[1] < target->y_length) {
        return(RH4N_RET_DIM2_TOO_SMALL);
    }

    for(; i < target->x_length; i++) {
        occ[0] = i;
        for(y=0; y < target->y_length; y++) {
            occ[1] = y;
            offset = (i*target->y_length)+y;
            val = ((int*)target->data)[offset];
            printf("x = [%d] y = [%d] = [%d]\n", i, y, val);
            switch(natvar_pd.length) {
                case I1SIZE:
                    if(val > MAXI1)
                        return(RH4N_RET_BUFFER_OVERFLOW);
                    i1_val = (NATTYP_I1)val;
                    if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I1), 
                            &i1_val, occ)) != NNI_RC_OK) {
                        return(RH4N_RET_NNI_ERR);
                    }
                    break;
                case I2SIZE:
                    if(val > MAXI2)
                        return(RH4N_RET_BUFFER_OVERFLOW);
                    i2_val = (NATTYP_I2)val;
                    if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I2), 
                            &i2_val, occ)) != NNI_RC_OK) {
                        return(RH4N_RET_NNI_ERR);
                    }
                    break;
                case I4SIZE:
                    if(val > MAXI4)
                        return(RH4N_RET_BUFFER_OVERFLOW);
                    i4_val = (NATTYP_I4)val;
                    if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I4), 
                            &i4_val, occ)) != NNI_RC_OK) {
                        return(RH4N_RET_NNI_ERR);
                    }
                    break;
            }
        }
    }
    return(RH4N_RET_OK);
}

int json_3di_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {
    int rc = 0, i = 0, y = 0, z = 0, x_array_flag = 0, occ[3] = {0, 0, 0}, val = 0,
        offset = 0;
    NATTYP_I1 i1_val = 0;
    NATTYP_I2 i2_val = 0;
    NATTYP_I4 i4_val = 0;

    if(natvar_pd.flags & IF4_FLG_UBVAR_0 && natvar_pd.flags & IF4_FLG_UBVAR_1 && natvar_pd.flags & NNI_FLG_UBVAR_2) {
        occ[0] = target->x_length;
        occ[1] = target->y_length;
        occ[2] = target->z_length;
        printf("Setting array size\n");
        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK)
            return(RH4N_RET_NNI_ERR);
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    } else if(natvar_pd.occurrences[1] < target->y_length) {
        return(RH4N_RET_DIM2_TOO_SMALL);
    } else if(natvar_pd.occurrences[2] < target->z_length) {
        return(RH4N_RET_DIM3_TOO_SMALL);
    }

    for(; i < target->x_length; i++) {
        occ[0] = i;
        for(y=0; y < target->y_length; y++) {
            occ[1] = y;
            for(z=0; z < target->z_length; z++) {
                occ[2] = z;

                offset = (z*target->x_length * target->y_length);
                offset += (y*target->x_length) + i;

                val = ((int*)target->data)[offset];
                printf("x = [%d] y = [%d] z = [%d] = [%d]\n", i, y, z, val);
                switch(natvar_pd.length) {
                    case I1SIZE:
                        if(val > MAXI1)
                            return(RH4N_RET_BUFFER_OVERFLOW);
                        i1_val = (NATTYP_I1)val;
                        if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I1), 
                                &i1_val, occ)) != NNI_RC_OK) {
                            return(RH4N_RET_NNI_ERR);
                        }
                        break;
                    case I2SIZE:
                        if(val > MAXI2)
                            return(RH4N_RET_BUFFER_OVERFLOW);
                        i2_val = (NATTYP_I2)val;
                        if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I2), 
                                &i2_val, occ)) != NNI_RC_OK) {
                            return(RH4N_RET_NNI_ERR);
                        }
                        break;
                    case I4SIZE:
                        if(val > MAXI4)
                            return(RH4N_RET_BUFFER_OVERFLOW);
                        i4_val = (NATTYP_I4)val;
                        if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, sizeof(NATTYP_I4), 
                                &i4_val, occ)) != NNI_RC_OK) {
                            return(RH4N_RET_NNI_ERR);
                        }
                        break;
                }
            }
        }
    }
    return(RH4N_RET_OK);
}
