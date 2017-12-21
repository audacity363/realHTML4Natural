#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <wchar.h>
#include <string.h>
#include <signal.h>

#include "natuser.h"
#include "natni.h"

#include "vars.h"
#include "rh4n_json_mapper.h"
#include "natutils.h"

extern pnni_611_functions getNNIFunctions(void **shlibptr);
extern vars_t *getVarPtr(pnni_611_functions nnifuncs, void *parmhandle);

//int checkVarcompatibility(vars_t *target, pnni_611_functions nnifuncs, void *parmhandle);

long getVar(WORD nparms, void *parmhandle, void *traditional) {
    printf("getVar got called\n");
    if(traditional) {
        return(RH4N_RET_PARM_MISSMATCH);
    }
    if(nparms != 3) {
        printf("parm missmatch. Expected [3] got [%d]\n", nparms);
        return(RH4N_RET_PARM_MISSMATCH);
    }

    void *shNNILib = NULL;
    pnni_611_functions nnifuncs = NULL;
    struct parameter_description varname_pd;
    int rc = 0;
    char namebuff[MAX_NAME_LENGTH+1];

    vars_t *var_anker = NULL, *target = NULL;

    if((nnifuncs = getNNIFunctions(&shNNILib)) == NULL) {
        printf("Could not open libnatural.so\n");
        return(RH4N_RET_SO_ERR);
    }

    if((var_anker = getVarPtr(nnifuncs, parmhandle)) == NULL) {
        return(RH4N_RET_VAR_PTR_ERR);
    }
    
    if((rc = nnifuncs->pf_nni_get_parm_info(nnifuncs, 1, parmhandle, &varname_pd)) != NNI_RC_OK) {
        printf("nni get parm info = ret [%d]\n", rc);
        return(RH4N_RET_NNI_ERR);
    }

    if(varname_pd.format != NNI_TYPE_ALPHA && varname_pd.length_all > MAX_NAME_LENGTH) {
        printf("parm missmatch on name\n");
        return(RH4N_RET_PARM_TYPE_MISSMATCH);
    }

    memset(namebuff, 0x00, sizeof(namebuff));
    if((rc = nnifuncs->pf_nni_get_parm(nnifuncs, 1, parmhandle, varname_pd.length_all, namebuff)) != NNI_RC_OK) {
        printf("nni get parm = [%d]\n", rc);
        return(RH4N_RET_NNI_ERR);
    }
    printf("Nat length_all: [%d]\n", varname_pd.length_all);
    namebuff[varname_pd.length_all] = '\0';
    //trimSpaces(namebuff);

    printf("returning var: [%s]\n", namebuff);
    if((target = isDefinedGrp(var_anker, NULL, namebuff)) == NULL) {
        printf("Var [%s] is not defined\n", namebuff);
        return(RH4N_RET_UNKOWN_VAR);
    }

    printf("Got var [%s]: [%p]\n", namebuff, target);

    if((rc = checkVarcompatibility(target, nnifuncs, parmhandle)) != RH4N_RET_OK) {
        CloseLib(&shNNILib);
        printf("Error: [%d]\n", rc);
        return(rc);
    }

    CloseLib(&shNNILib);
    return(0);
}

int checkVarcompatibility(vars_t *target, pnni_611_functions nnifuncs, void *parmhandle) {
    struct parameter_description natvar_pd;
    int rc = 0;

    if((rc = nnifuncs->pf_nni_get_parm_info(nnifuncs, GET_VAR_RET_PARM_POS, parmhandle, &natvar_pd)) != NNI_RC_OK) {
        printf("nni get parm info: ret [%d]\n", rc);
        return(RH4N_RET_NNI_ERR);
    }

    if(natvar_pd.flags & IF4_FLG_PROTECTED) {
        return(RH4N_RET_CONST_VAR);
    }

    if(target->type == INTEGER && natvar_pd.dimensions == 0 
          && (natvar_pd.format == NNI_TYPE_INT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("Handle Int\n");
        return(json_i_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == ONEDINTEGER && natvar_pd.dimensions == 1
          && (natvar_pd.format == NNI_TYPE_INT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("handle 1D Int\n");
        return(json_1di_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == TWODINTEGER && natvar_pd.dimensions == 2
          && (natvar_pd.format == NNI_TYPE_INT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("handle 2D Int\n");
        return(json_2di_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == THREEDINTEGER && natvar_pd.dimensions == 3 
          && (natvar_pd.format == NNI_TYPE_INT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("handle 3D Int\n");
        return(json_3di_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } 
    
    else if(target->type == STRING && natvar_pd.dimensions == 0 
          && natvar_pd.format == NNI_TYPE_ALPHA) {
        printf("Handle String\n");
        return(json_a_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == ONEDSTRING && natvar_pd.dimensions == 1
          && natvar_pd.format == NNI_TYPE_ALPHA) {
        printf("handle 1D String\n");
        return(json_1da_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == TWODSTRING && natvar_pd.dimensions == 2
          && natvar_pd.format == NNI_TYPE_ALPHA) {
        printf("handle 2D String\n");
        return(json_2da_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == THREEDSTRING && natvar_pd.dimensions == 3 
          && natvar_pd.format == NNI_TYPE_ALPHA) {
        printf("handle 3D String\n");
        return(json_3da_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    }

    else if(target->type == STRING && natvar_pd.dimensions == 0 
          && natvar_pd.format == NNI_TYPE_NUM) {
        printf("Handle String to Numeric\n");
        return(json_aton_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == ONEDSTRING && natvar_pd.dimensions == 1
          && natvar_pd.format == NNI_TYPE_NUM) {
        printf("handle 1D String to Numeric\n");
        return(json_1daton_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == TWODSTRING && natvar_pd.dimensions == 2
          && natvar_pd.format == NNI_TYPE_NUM) {
        printf("handle 2D String to Numeric\n");
        return(json_2daton_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == THREEDSTRING && natvar_pd.dimensions == 3 
          && natvar_pd.format == NNI_TYPE_NUM) {
        printf("handle 3D String to Numeric\n");
        return(json_3daton_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    }

    else if(target->type == BOOL && natvar_pd.dimensions == 0 
          && natvar_pd.format == NNI_TYPE_LOG ) {
        printf("Handle Bool\n");
        return(json_l_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == ONEDBOOL && natvar_pd.dimensions == 1
          && natvar_pd.format == NNI_TYPE_LOG ) {
        printf("handle 1D Bool\n");
        return(json_1dl_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == TWODBOOL && natvar_pd.dimensions == 2
          && natvar_pd.format == NNI_TYPE_ALPHA ) {
        printf("handle 2D Bool\n");
        return(json_2dl_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == THREEDBOOL && natvar_pd.dimensions == 3 
          && natvar_pd.format == NNI_TYPE_ALPHA ){
        printf("handle 3D Bool\n");
        return(json_3dl_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    }

    else if(target->type == FLOAT && natvar_pd.dimensions == 0 
          && (natvar_pd.format == NNI_TYPE_FLOAT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("Handle Float\n");
        return(json_f_handling(GET_VAR_RET_PARM_POS, natvar_pd, nnifuncs, parmhandle, target));
    } else if(target->type == ONEDFLOAT && natvar_pd.dimensions == 1
          && (natvar_pd.format == NNI_TYPE_FLOAT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("handle 1D Float\n");
        return(RH4N_RET_NOT_SUPPORTED);
    } else if(target->type == TWODFLOAT && natvar_pd.dimensions == 2
          && (natvar_pd.format == NNI_TYPE_FLOAT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("handle 2D Float\n");
        return(RH4N_RET_NOT_SUPPORTED);
        return(RH4N_RET_NOT_SUPPORTED);
    } else if(target->type == THREEDFLOAT && natvar_pd.dimensions == 3 
          && (natvar_pd.format == NNI_TYPE_FLOAT || natvar_pd.format == NNI_TYPE_NUM)) {
        printf("handle 3D Float\n");
        return(RH4N_RET_NOT_SUPPORTED);
    }
    else {
        printf("Variables doesn't match\n");
        printf("Nat type: [%c] JSON type: [%d]\n", natvar_pd.format, target->type);
        return(RH4N_RET_VAR_MISSMATCH);
    }
    return(RH4N_RET_OK);
}

