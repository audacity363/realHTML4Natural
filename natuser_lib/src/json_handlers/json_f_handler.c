#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "natuser.h"
#include "natni.h"

#include "standard.h"

#include "vars.h"
#include "rh4n_json_mapper.h"


//Not sure if this works. Can't test the return value in natural beacuse, for some reason, 
//a write work file does not support floats...
int json_f_handling(int natparm_pos, struct parameter_description natvar_pd, 
    pnni_611_functions nnifuncs, void* parmhandle, vars_t *target) {

    double val = 0;
    int rc = 0;

    printf("Float length: [%d]\n", natvar_pd.length);
    printf("Float size: [%d]\n", sizeof(float));
    printf("Float double: [%d]\n", sizeof(double));

    switch(natvar_pd.length) {
        case F4SIZE:
            return(RH4N_RET_USE_F8);
        case F8SIZE:
            val = *((double*)target->data);
            printf("value: [%f]\n", val);
            if((rc = nnifuncs->pf_nni_put_parm(nnifuncs, natparm_pos, parmhandle, sizeof(double), &val)) != NNI_RC_OK) {
                return(RH4N_RET_NNI_ERR);
            }
            break;
    }

    return(RH4N_RET_OK);
}
