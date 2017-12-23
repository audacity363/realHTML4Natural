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

#define PARM_POS_VARPTR 0

long printvars(WORD nparms, void *parmhandle, void *traditional) {
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

    if((var_anker = getVarPtr(nnifuncs, parmhandle, PARM_POS_VARPTR)) == NULL) {
        return(3);
    }
    printf("ptr: [%p]\n", var_anker);
    printAllVarsToFile(var_anker, stdout);
    printf("-------------------------------------------------------------------------\n");

    CloseLib(&shNNILib);
    return(0);
}

