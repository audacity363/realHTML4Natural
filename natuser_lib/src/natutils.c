#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>

#include "vars.h"

#include "natuser.h"
#include "natni.h"
#include "natutils.h"

char *so_error = NULL;

pnni_611_functions initNNI(void *lib)
{
    pnni_611_functions s_funcs;
    PF_NNI_GET_INTERFACE pf_nni_get_interface = 0;
    int rc = 0;
    char *error = NULL;

    if(!(pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(lib, GET_INTERFACE_FUNC)))
    {
        error = dlerror();
        printf("Error while loading Function [%s]: [%s]\n", GET_INTERFACE_FUNC,
            error);
        return(NULL);
    }

    if(((pf_nni_get_interface)(NNI_VERSION_611, (void**)&s_funcs)) != NNI_RC_OK)
    {
        printf("...Error while gettings Function Table\n");
        return(NULL);
    }

    return(s_funcs);
}

//Basicly the same as initNNI with the difference that this function opens th shared object
//by itself 
//urgent TODO; merge the two functions!
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

char* OpenLib(void **shLib, char *name)
{
    static char error[1024];

    *shLib = dlopen(name, RTLD_NOW);
    if(!*shLib) {
        sprintf(error, "Error while loading Module [%s]: [%s]", name, dlerror());
        return(error);
    }
    return(NULL);
}

void CloseLib(void **shLib)
{
    dlclose(*shLib);
}

void convert2Bto4BString(char *inbuffer, wchar_t *outbuffer, int length)
{
    int i, offset;

    void *v_in, *v_out, *v_tmp;

    v_in = (void*)inbuffer;
    v_out = (void*)outbuffer;

    for(i=0; i < length; i++)
    {
        offset = (sizeof(wchar_t)*i)+2;

        v_tmp = v_out+offset;

        memcpy(v_tmp, v_in+(i*2), 2);
    }
}

void convert1Bto4BString(char *inbuffer, wchar_t *outbuffer, int length)
{
    int i, offset;

    void *v_in, *v_out, *v_tmp;

    v_in = (void*)inbuffer;
    v_out = (void*)outbuffer;

    for(i=0; i < length; i++)
    {
        offset = (sizeof(wchar_t)*i)+3;

        v_tmp = v_out+offset;

        memcpy(v_tmp, v_in+i, 1);
    }
}

char *getErrorMsg(int error)
{
    static char *errorstrs[] = 
    {

        "LDA Parameter Error (check length and format)",
        "Internal Variable Error",
        "malloc() Error. No Memory",
        "XArray has no length (check if it is initialized)"
    };

    if((error-1) > (sizeof(errorstrs)/sizeof(char*)))
    {
        return(NULL);
    }

    return(errorstrs[error-1]);
}

void logError(int ret, char *outputpath, FILE *logfile) 
{
    FILE *outputfile = NULL;
    char *errorstr = NULL;

    fprintf(logfile, "Errornumber: [%d]\n", ret);

    if((outputfile = fopen(outputpath, "w")) == NULL)
    {
        fprintf(logfile, "Error while writing Error: [%s]\n", strerror(errno));
        return;
    }

    if(ret < 0)
    {

        fprintf(outputfile, "NNI Error: [%d]", ret*-1);
        fprintf(logfile, "NNI Error: [%d]\n", ret*-1);
    }
    else
    {
        if((errorstr = getErrorMsg(ret)) == NULL)
        {
            fprintf(outputfile, "No Errorstr defined for number: [%d]", ret);
            fprintf(logfile, "No Errorstr defined for number: [%d]\n", ret);
        }
        else
        {
            fprintf(outputfile, "Error: [%s]", errorstr);
            fprintf(logfile, "Error: [%s]\n", errorstr);
        }
    }

    fclose(outputfile);
    return;
}

vars_t *getVarPtr(pnni_611_functions nnifuncs, void *parmhandle, int position) {
    struct parameter_description pd;
    int rc = 0;
    vars_t *var_anker = NULL;

    if((rc = nnifuncs->pf_nni_get_parm_info(nnifuncs, position, parmhandle, &pd)) != NNI_RC_OK) {
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
    
    if((rc = nnifuncs->pf_nni_get_parm(nnifuncs, position, parmhandle, sizeof(var_anker), &var_anker)) != NNI_RC_OK) {
        printf("nni get parm = ret: [%d]\n", rc);
        return(NULL);
    }

    if(var_anker == NULL) {
        printf("var_anker == NULL\n");
        return(NULL);
    }

    return(var_anker);
}
