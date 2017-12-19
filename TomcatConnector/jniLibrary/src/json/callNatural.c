#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>

#include "jni.h"

#include "vars.h"
#include "natni.h"
#include "jni_rh4n.h"

int callNatural_JSON(vars_t *var_anker) {
    void *shNNILib = NULL;
    pnni_611_functions nnifuncs;  
    PF_NNI_GET_INTERFACE pf_nni_get_interface = 0;
    struct natural_exception nat_ex;
    char *errormsg;
    int rc;

    struct parameter_description nniparms[1];
    struct naturalparms *target;
    void *pvnniparms = NULL;

    printf("ptr size: [%d]\n", sizeof(var_anker));

    if((errormsg = OpenLib(&shNNILib, "libnatural.so")) != NULL)
    {
        printf("Error: %s\n", errormsg);
        return(-1);
    }

    pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(shNNILib, "nni_get_interface");

    (pf_nni_get_interface)(NNI_VERSION_CURR, (void**)&nnifuncs);

    if((rc = nnifuncs->pf_nni_initialize(nnifuncs, "etid=$$ parm=logiparm", 0, 0)) != NNI_RC_OK) {
        printf("Error: %d\n", rc);
        return(-2);
    }

    printf("create parm: [%d]\n", nnifuncs->pf_nni_create_parm(nnifuncs, 1, &pvnniparms));

    printf("init parm: [%d]\n", nnifuncs->pf_nni_init_parm_s(nnifuncs, 0, pvnniparms, NNI_TYPE_BIN, 8, 0, 0));
    printf("putparm: [%d]\n", nnifuncs->pf_nni_put_parm(nnifuncs, 0, pvnniparms, sizeof(var_anker), &var_anker));
    printf("get parm: [%d]\n", nnifuncs->pf_nni_get_parm_info(nnifuncs, 0, pvnniparms, &nniparms[0]));

    printf("logon: [%d]\n", nnifuncs->pf_nni_logon(nnifuncs, "TGAP0734", NULL, NULL));

    if((rc = nnifuncs->pf_nni_callnat(nnifuncs, "PTRTEST", 1, nniparms, &nat_ex)) != NNI_RC_OK) {
        printNaturalException(&nat_ex, stdout);
    }

}
