#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <wchar.h>
#include <string.h>
#include <signal.h>

#include "natuser.h"
#include "natni.h"

#include "vars.h"
#include "natutils.h"

pnni_611_functions getNNIFunctions(void **shlibptr);
vars_t *getVarPtr(pnni_611_functions nnifuncs, void *parmhandle);

long printvars(WORD nparms, void *parmhandle, void *traditional) {
    return(0);
#if 0
    if(traditional) {
        return(1);
    }

    void *shNNILib = NULL;
    struct parameter_description pd;
    char *errormsg;
    pnni_611_functions nnifuncs;  

    vars_t *var_anker = NULL;

    if((nnifuncs = getNNIFunctions(&shNNILib)) == NULL) {
        return(2);
    }

    printf("---------------------------------NATUSER---------------------------------\n");

    if((var_anker = getVarPtr(nnifuncs, parmhandle)) == NULL) {
        return(3);
    }
    printf("ptr: [%p]\n", var_anker);
    printAllVarsToFile(var_anker, stdout);
    printf("-------------------------------------------------------------------------\n");

    CloseLib(&shNNILib);
    return(0);
#endif

}

vars_t *getVarPtr(pnni_611_functions nnifuncs, void *parmhandle) {
    struct parameter_description pd;
    int rc = 0;
    vars_t *var_anker = NULL;

    if((rc = nnifuncs->pf_nni_get_parm_info(nnifuncs, 0, parmhandle, &pd)) != NNI_RC_OK) {
        printf("nni get info = ret: [%d]\n", rc);
        return(NULL);
    }

    if(pd.format != NNI_TYPE_BIN) {
        printf("Parm 0 != BIN\n");
        return(NULL);
    }

    if(pd.length_all != sizeof(var_anker)) {
        printf("Parm 0 length does not match. Expexted: [%d] got: [%d]\n", sizeof(var_anker), pd.length_all);
        return(NULL);
    }
    
    if((rc = nnifuncs->pf_nni_get_parm(nnifuncs, 0, parmhandle, sizeof(var_anker), &var_anker)) != NNI_RC_OK) {
        printf("nni get parm = ret: [%d]\n", rc);
        return(NULL);
    }

    if(var_anker == NULL) {
        printf("var_anker == NULL\n");
        return(NULL);
    }

    return(var_anker);
}

pnni_611_functions getNNIFunctions(void **shlibptr) {
    char *errormsg = NULL;
    pnni_611_functions nnifuncs;  
    PF_NNI_GET_INTERFACE pf_nni_get_interface = 0;

    if((errormsg = OpenLib(shlibptr, "libnatural.so")) != NULL)
    {
        printf("Error: %s\n", errormsg);
        return(NULL);
    }

    if((pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(*shlibptr, "nni_get_interface")) == NULL) {
        printf("Error: Could not get symbol: nni_get_interface\n");
        return(NULL);
    }

    (pf_nni_get_interface)(NNI_VERSION_CURR, (void**)&nnifuncs);

    return(nnifuncs);
}
