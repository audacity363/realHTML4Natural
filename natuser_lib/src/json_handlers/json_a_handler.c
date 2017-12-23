#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "natuser.h"
#include "natni.h"

#include "standard.h"

#include "vars.h"
#include "rh4n_json_mapper.h"



int json_a_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {

    char *buff = NULL;
    int rc = 0;

    if(!(natvar_pd.flags & NNI_FLG_DYNAMIC)) {
        if(natvar_pd.length < target->length) 
            return(RH4N_RET_BUFFER_OVERFLOW);
    }

    if((buff = malloc(sizeof(char)*target->length)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    if(wcstombs(buff, (wchar_t*)target->data, target->length) < 0) {
        free(buff);
        return(RH4N_RET_UNICODE_ERR);
    }
    
    if((rc = nnifuncs->pf_nni_put_parm(nnifuncs, natparm_pos, parmhandle, strlen(buff), buff)) != NNI_RC_OK) {
        if(rc < 0) {
            free(buff);
            return(RH4N_RET_NNI_ERR);
        }
    }
    free(buff);

    return(RH4N_RET_OK);
}

int json_1da_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {

    char *buff = NULL;
    int rc = 0, occ[3] = {0, 0, 0}, x = 0, offset = 0;

    if(!(natvar_pd.flags & NNI_FLG_DYNAMIC)) {
        if(natvar_pd.length < target->length) 
            return(RH4N_RET_BUFFER_OVERFLOW);
    }

    if(natvar_pd.flags & NNI_FLG_UBVAR_0) {
        printf("resize array\n");
        occ[0] = target->x_length;
        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK) {
            return(RH4N_RET_NNI_ERR);
        }
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    }

    if((buff = malloc(sizeof(char)*target->length)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    for(; x < target->x_length; x++) {
        occ[0] = x;
        offset = ((target->length)*sizeof(wchar_t))*(x);
        memset(buff, NULL, sizeof(char)*target->length);

        if(wcstombs(buff, (wchar_t*)(target->data+offset), target->length) < 0) {
            free(buff);
            return(RH4N_RET_UNICODE_ERR);
        }

        printf("x = [%d] = [%s]\n", x, buff);

        if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, strlen(buff), buff, occ)) != NNI_RC_OK) {
            if(rc < 0) {
                free(buff);
                return(RH4N_RET_NNI_ERR);
            }
        }
    }

    free(buff);
    return(RH4N_RET_OK);
}

int json_2da_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {

    char *buff = NULL;
    int rc = 0, occ[3] = {0, 0, 0}, x = 0, y = 0, offset = 0;

    if(!(natvar_pd.flags & NNI_FLG_DYNAMIC)) {
        if(natvar_pd.length < target->length) 
            return(RH4N_RET_BUFFER_OVERFLOW);
    }

    if(natvar_pd.flags & NNI_FLG_UBVAR_0 && natvar_pd.flags & NNI_FLG_UBVAR_1) {
        printf("resize array\n");
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

    if((buff = malloc(sizeof(char)*target->length)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    for(; x < target->x_length; x++) {
        occ[0] = x;
        for(y=0; y < target->y_length; y++) {
            occ[1] = y;

            offset = (target->y_length*(target->length*sizeof(wchar_t)));
            offset = offset*(x)+((target->length*sizeof(wchar_t))*y);
            memset(buff, NULL, sizeof(char)*target->length);

            if(wcstombs(buff, (wchar_t*)(target->data+offset), target->length) < 0) {
                free(buff);
                return(RH4N_RET_UNICODE_ERR);
            }

            printf("x = [%d] y = [%d] = [%s]\n", x, y, buff);

            if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, strlen(buff), buff, occ)) != NNI_RC_OK) {
                if(rc < 0) {
                    free(buff);
                    return(RH4N_RET_NNI_ERR);
                }
            }
        }
    }

    free(buff);
    return(RH4N_RET_OK);
}

int json_3da_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {

    char *buff = NULL;
    int rc = 0, occ[3] = {0, 0, 0}, x = 0, y = 0, z = 0, offset = 0,
        var_size = 0, sizeofz = 0, sizeofy = 0;

    if(!(natvar_pd.flags & NNI_FLG_DYNAMIC)) {
        if(natvar_pd.length < target->length) 
            return(RH4N_RET_BUFFER_OVERFLOW);
    }

    if(natvar_pd.flags & NNI_FLG_UBVAR_0 && natvar_pd.flags & NNI_FLG_UBVAR_1 && natvar_pd.flags & NNI_FLG_UBVAR_2) {
        printf("resize array\n");
        occ[0] = target->x_length;
        occ[1] = target->y_length;
        occ[2] = target->z_length;
        if((rc = nnifuncs->pf_nni_resize_parm_array(nnifuncs, natparm_pos, parmhandle, occ)) != NNI_RC_OK) {
            return(RH4N_RET_NNI_ERR);
        }
    } else if(natvar_pd.occurrences[0] < target->x_length) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    } else if(natvar_pd.occurrences[1] < target->y_length) {
        return(RH4N_RET_DIM2_TOO_SMALL);
    } else if(RH4N_RET_DIM3_TOO_SMALL);

    if((buff = malloc(sizeof(char)*target->length)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    for(; x < target->x_length; x++) {
        occ[0] = x;
        for(y=0; y < target->y_length; y++) {
            occ[1] = y;
            for(z=0; z < target->z_length; z++) {
                occ[2] = z;

                var_size = target->length*sizeof(wchar_t);
                sizeofz = var_size*(target->z_length);
                sizeofy = sizeofz*(target->y_length);

                offset = (x*sizeofy)+(y*sizeofz)+(var_size*z);

                memset(buff, NULL, sizeof(char)*target->length);

                if(wcstombs(buff, (wchar_t*)(target->data+offset), target->length) < 0) {
                    free(buff);
                    return(RH4N_RET_UNICODE_ERR);
                }

                printf("x = [%d] y = [%d] z = [%d] = [%s]\n", x, y, z, buff);

                if((rc = nnifuncs->pf_nni_put_parm_array(nnifuncs, natparm_pos, parmhandle, strlen(buff), 
                        buff, occ)) != NNI_RC_OK) {
                    if(rc < 0) {
                        free(buff);
                        return(RH4N_RET_NNI_ERR);
                    }
                }
            }
        }
    }

    free(buff);
    return(RH4N_RET_OK);
}
