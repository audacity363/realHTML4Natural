#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "standard.h"

#include "httpapi.h"
#include "httpint.h"
#include "lookup.h"
#include "natural.h"
#include "hexdump.h"

#include "natni.h"
#include "natuser.h"

#define NAT_LIB_NAME "libnatural.so"
#define GET_INTERFACE_FUNC "nni_get_interface"
#define LOGON_LIB "TGAP0734"
#define TEST_PROG "SHTEST"

void printNaturalException( struct natural_exception *pnatexcep );
int OpenLib(void **shLib, char *name);
void CloseLib(void **shLib);

path_struct checkifNaturalPath(HttpParam* hp, char *url)
{
    printf("looking Up: [%s]\n", url);
    path_struct settings;

    settings = getpathsettings(url, hp->config_path);
    return(settings);
}

int callNaturalProgram(HttpParam *hp, HttpSocket* phsSocket,
                      path_struct settings)
{
    void *shlib = NULL;
    char *error;
    char tmp_buff[2024];
    int rc, i;
    PF_NNI_GET_INTERFACE pf_nni_get_interface = 0;
    pnni_611_functions s_func;
    struct natural_exception nat_ex;
    struct parameter_description params[5];
    void *phandle = NULL;
    int ind[3] = {0, 0, 0}; 
    int occ[3] = {0, 0, 0};
    char req_type[5];

    if(phsSocket->pxMP != NULL)
    {
        strcpy(req_type, "POST");
    }
    else
    {
        strcpy(req_type, "GET");
    }

    printf("REQ_TYPE: [%s]\n", req_type);


    if(OpenLib(&shlib, NAT_LIB_NAME) < 0)
    {
        return(-1);
    }

    printf("Done Loading. Pointer: %p\n", shlib);

    pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(shlib,
        GET_INTERFACE_FUNC);

    if(!pf_nni_get_interface)
    {
        error = dlerror();
        printf("Error while loading Function [%s]: [%s]\n", GET_INTERFACE_FUNC,
            error);
        CloseLib(&shlib);
        return(-2);
    }

    //Get Function Table
    printf("Get Function Table...");
    if(((pf_nni_get_interface)(NNI_VERSION_CURR, (void**)&s_func)) != NNI_RC_OK)
    {
        printf("...Error while gettings Functrion Table\n");
        CloseLib(&shlib);
        return(-3);
    }
    printf("...Done\n");

    //Initialize Natural session
    printf("Init Natural Session...");
    if(s_func->pf_nni_initialize(s_func, "parm=logiparm etid=&& NATLOG=ALL", 0, 0) != NNI_RC_OK)
    {
        printf("...Error\n");
        CloseLib(&shlib);
        return(-4);
    }
    printf("...Done\n");

    //Logon into tgap0734
    printf("Logon into [%s]...", LOGON_LIB);
    if((rc = s_func->pf_nni_logon(s_func, LOGON_LIB, 0, 0)) != NNI_RC_OK)
    {
        printf("...Error. Nr: [%d]\n", rc);
        CloseLib(&shlib);
        return(-5);
    }
    printf("...Done\n");

    s_func->pf_nni_create_parm(s_func, 5, &phandle);

    occ[0] = 20;
    //Values
    s_func->pf_nni_init_parm_sa(s_func, 0, phandle, 'A', 2024, 0, 1, occ, 0);
    //Keys
    s_func->pf_nni_init_parm_sa(s_func, 1, phandle, 'A', 100, 0, 1, occ, 0);
    s_func->pf_nni_init_parm_s(s_func, 2, phandle, 'A', 5, 0, 0);
    s_func->pf_nni_init_parm_s(s_func, 3, phandle, 'A', 100, 0, 0);
    s_func->pf_nni_init_parm_d(s_func, 4, phandle, 'A', 0);

    printf("There are [%d] POST Arguments\n", phsSocket->request.pchParamNumber);
    printf("Arguments:\n");
    for(i=0; i < phsSocket->request.pchParamNumber; i++)
    {
        printf("[%s]=[%s]\n", phsSocket->request.stParams[i].pchParamName, 
            phsSocket->request.stParams[i].pchParamValue);
    }

    for(ind[0]=0; ind[0] < occ[0]; ind[0]++)
    {
        memcpy(tmp_buff, phsSocket->request.stParams[ind[0]].pchParamValue, sizeof(tmp_buff));
        if(ind[0] == 0)
            logHexDump(tmp_buff, sizeof(tmp_buff), stdout);
        rc = s_func->pf_nni_put_parm_array(s_func, 0, phandle, 2024, tmp_buff, ind);
        switch(rc)
        {
            case NNI_RC_OK:
                printf("NNI_RC_OK\n");
                break;
            case NNI_RC_NOT_INIT:
                printf("NNI_RC_NOT_INIT\n");
                break;
            case NNI_RC_PARM_ERROR:
                printf("NNI_RC_PARM_ERROR\n");
                break;
            case NNI_RC_ILL_PNUM:
                printf("NNI_RC_ILL_PNUM\n");
                break;
            case NNI_RC_WRT_PROT:
                printf("NNI_RC_WRT_PROT\n");
                break;
            case NNI_RC_DATA_TRUNC:
                printf("NNI_RC_DATA_TRUNC\n");
                break;
            case NNI_RC_NO_MEMORY:
                printf("NNI_RC_NO_MEMORY\n");
                break;
        }
        if(rc > 0)
            printf("only [%d] Bytes were read\n", rc);

        strcpy(tmp_buff, phsSocket->request.stParams[ind[0]].pchParamName);
        s_func->pf_nni_put_parm_array(s_func, 1, phandle, 100, tmp_buff, ind);
    }

    strcpy(tmp_buff, req_type);
    s_func->pf_nni_put_parm(s_func, 2, phandle, 1024, tmp_buff);

    strcpy(tmp_buff, "/tmp/outputtest.html");
    s_func->pf_nni_put_parm(s_func, 3, phandle, 1024, tmp_buff);

    strcpy(tmp_buff, "test=ok");
    s_func->pf_nni_put_parm(s_func, 4, phandle, 1024, tmp_buff);

    for(i=0; i < 5; i++)
    {
        s_func->pf_nni_get_parm_info(s_func, i, phandle, &params[i]);
    }


    //Call Subprog with no parms
    printf("Call subprogramm [%s]...", settings.programm);
    if(s_func->pf_nni_callnat(s_func, settings.programm, 5, &params, &nat_ex) != NNI_RC_OK)
    {
        printf("...Error\n");
        printNaturalException(&nat_ex);
        CloseLib(&shlib);
        return(-6);
    }
    printf("...Done\n");


    printf( "Uninitializing Natural..." );
    s_func->pf_nni_uninitialize(s_func);
    printf("...Done\n");

    printf("Freeing interface...");
    s_func->pf_nni_free_interface(s_func);
    printf("...Done\n");

    CloseLib(&shlib);
}

int OpenLib(void **shLib, char *name)
{
    char *error;

    *shLib = dlopen(name, RTLD_NOW);
    if(!*shLib)
    {
        error = dlerror();
        printf("Error while loading Module [%s]: [%s]\n", name, error);
        return(-1);
    }
    return(0);
}

void CloseLib(void **shLib)
{
    dlclose(*shLib);
}



/* Format and print a Natural exception. */
void printNaturalException( struct natural_exception *pnatexcep )
{
    if (pnatexcep)
    {
        printf( "MessageNumber: %d.\n",
            pnatexcep->natMessageNumber );
        printf( "MessageText:   %s\n",
            pnatexcep->natMessageText ? pnatexcep->natMessageText : "0" );
        printf( "Library:       %s.\n",
            pnatexcep->natLibrary ? pnatexcep->natLibrary : "0" );
        printf( "Member:        %s.\n",
            pnatexcep->natMember ? pnatexcep->natMember : "0" );
        printf( "Name:          %s.\n",
            pnatexcep->natName ? pnatexcep->natName : "0" );
        printf( "Method:        %s.\n",
            pnatexcep->natMethod ? pnatexcep->natMethod : "0" );
        printf( "Line:          %d.\n",
            pnatexcep->natLine );
    }
}
