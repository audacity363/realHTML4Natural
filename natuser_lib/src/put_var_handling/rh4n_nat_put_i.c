#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "natuser.h"
#include "natni.h"

#include "standard.h"
#include "rh4n_nat_varhandling.h"

int rh4nnatputInteger(RH4nNatVarHandleParms *args, char *groupname, char *varname) {
    int rc = 0, varsize = 0;
    NATTYP_I1 i1_val = 0;
    NATTYP_I2 i2_val = 0;
    NATTYP_I4 i4_val = 0;

    if(args->desc->dimensions > 0) return(rh4nnatputIntegerArray(args, groupname, varname));

    if((rc = rh4nvarGetInt(args->varlist, groupname, varname, &i4_val)) != RH4N_RET_OK) {
        sprintf(args->errorstr, "Could not get integer from %s.%s. Varlib return: %d", groupname, varname, rc);
        return(rc);
    }

    rh4n_log_debug(args->props->logging, "Got Integer: %d", i4_val);

    switch(args->desc->length) {
        case I1SIZE:
            if(i4_val > MAXI1)
                return(RH4N_RET_BUFFER_OVERFLOW);
            i4_val = i4_val << 24;
            varsize = sizeof(NATTYP_I1);
            break;
        case I2SIZE:
            if(i4_val > MAXI2)
                return(RH4N_RET_BUFFER_OVERFLOW);
            i4_val = i4_val << 16;
            varsize = sizeof(NATTYP_I2);
            break;
        case I4SIZE:
            if(i4_val > MAXI4)
                return(RH4N_RET_BUFFER_OVERFLOW);
            varsize = sizeof(NATTYP_I4);
            break;
    }

    rh4n_log_debug(args->props->logging, "Got varsize: %d", varsize);


    if((rc = args->nnifuncs->pf_nni_put_parm(args->nnifuncs, args->parmindex, args->parmhandle, 
            varsize, &i4_val)) != NNI_RC_OK) 
        return(RH4N_RET_NNI_ERR);

    return(RH4N_RET_OK);
}

int rh4nnatputIntegerArray(RH4nNatVarHandleParms *args, char* groupname, char* varname) {
    char *buff = NULL;
    int rc = 0, x = 0, y = 0, z = 0, arrlength[3] = { 0, 0, 0}, arrindex[3] = { -1, -1, -1 };
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;

    if((rc = rh4nvarGetRef(args->varlist, groupname, varname, &_refvar)) != RH4N_RET_OK) {
        sprintf(args->errorstr, "Could not get variable %s.%s. Varlib return: %d", groupname, varname, rc);
        return(rc);
    }

    if((rc = rh4nvarGetArrayLength(&_refvar.var->var, arrlength)) != RH4N_RET_OK) {
        sprintf(args->errorstr, "Could not get array length for %s.%s. Varlib return: %d", groupname, varname, rc);
        return(rc);
    }

    if(!(args->desc->flags & NNI_FLG_UBVAR_0) && args->desc->occurrences[0] < arrlength[0]) {
        return(RH4N_RET_DIM1_TOO_SMALL);
    } else if(args->desc->dimensions > 1 && !(args->desc->flags & NNI_FLG_UBVAR_1) && args->desc->occurrences[1] < arrlength[1]) {
        return(RH4N_RET_DIM2_TOO_SMALL);
    } else if(args->desc->dimensions > 2 && !(args->desc->flags & NNI_FLG_UBVAR_2) && args->desc->occurrences[2] < arrlength[2]) {
        return(RH4N_RET_DIM3_TOO_SMALL);
    }
    
    if(args->desc->flags & NNI_FLG_XARRAY) {
        rh4n_log_debug(args->props->logging, "Resize array to [%d,%d,%d]", arrlength[0], arrlength[1], arrlength[2]);
        if((rc = args->nnifuncs->pf_nni_resize_parm_array(args->nnifuncs, args->parmindex, args->parmhandle, arrlength)) != NNI_RC_OK) {
            return(RH4N_RET_NNI_ERR);
        }
    }

    for(; x < arrlength[0]; x++) {
        arrindex[0] = x;

        if(args->desc->dimensions == 1) {
            if((rc = rh4nnatputIntegerArrayEntry(args, groupname, varname, arrindex)) != RH4N_RET_OK) {
                return(rc);
            }
        } else {
            for(y=0; y < arrlength[1]; y++) {
                arrindex[1] = y;
                if(args->desc->dimensions == 2) {
                    if((rc = rh4nnatputIntegerArrayEntry(args, groupname, varname, arrindex)) != RH4N_RET_OK) {
                        return(rc);
                    }
                } else {
                    for(z=0; z < arrlength[2]; z++) {
                        arrindex[2] = z;
                        if((rc = rh4nnatputIntegerArrayEntry(args, groupname, varname, arrindex)) != RH4N_RET_OK) {
                            return(rc);
                        }
                    }
                }
            }
        }
    }
    return(RH4N_RET_OK);
}

int rh4nnatputIntegerArrayEntry(RH4nNatVarHandleParms *args, char* groupname, char* varname, int index[3]) {
    int strlength = 0, rc = 0, varsize = 0;
    NATTYP_I4 i4_val = 0;

    if((rc = rh4nvarGetIntArrayEntry(args->varlist, groupname, varname, index, &i4_val)) != RH4N_RET_OK) {
        sprintf(args->errorstr, "Could not get integer from %s.%s x: %d y: %d z: %d. Varlib return: %d", groupname, 
            varname, index[0], index[1], index[2], rc);
        return(rc);
    }

    switch(args->desc->length) {
        case I1SIZE:
            if(i4_val > MAXI1)
                return(RH4N_RET_BUFFER_OVERFLOW);
            i4_val = i4_val << 24;
            varsize = sizeof(NATTYP_I1);
            break;
        case I2SIZE:
            if(i4_val > MAXI2)
                return(RH4N_RET_BUFFER_OVERFLOW);
            i4_val = i4_val << 16;
            varsize = sizeof(NATTYP_I2);
            break;
        case I4SIZE:
            if(i4_val > MAXI4)
                return(RH4N_RET_BUFFER_OVERFLOW);
            varsize = sizeof(NATTYP_I4);
            break;
    }


    if((rc = args->nnifuncs->pf_nni_put_parm_array(args->nnifuncs, args->parmindex, args->parmhandle, varsize, 
            &i4_val, index)) != NNI_RC_OK) {
        if(rc < 0) {
            return(RH4N_RET_NNI_ERR);
        }
    }
    return(RH4N_RET_OK);
}
