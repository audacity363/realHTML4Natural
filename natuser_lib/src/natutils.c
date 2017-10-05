#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>

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

int OpenLib(void **shLib, char *name)
{
    char *error;

    *shLib = dlopen(name, RTLD_NOW);
    if(!*shLib)
    {
        return(-1);
    }
    return(0);
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
