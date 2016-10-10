#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>

#include "standard.h"

#include "httpapi.h"
#include "httpint.h"
#include "lookup.h"
#include "natural.h"
#include "hexdump.h"
#include "varhandle.h"

#include "natni.h"
#include "natuser.h"

#define NAT_LIB_NAME "libnatural.so"
#define GET_INTERFACE_FUNC "nni_get_interface"
#define LOGON_LIB "TGAP0734"
#define TEST_PROG "SHTEST"

void printNaturalException( struct natural_exception *pnatexcep, char*);
int OpenLib(void **shLib, char *name);
void CloseLib(void **shLib);

path_struct checkifNaturalPath(HttpParam* hp, char *url)
{
    char error_str[1024];
    printf("looking Up: [%s]\n", url);
    path_struct settings;

    settings = getpathsettings(url, hp->config_path, error_str);
    if(settings.error == true)
    {
        fprintf(stderr, "%s\n", error_str);
    }
    return(settings);
}

int natural_child(HttpParam *hp, HttpSocket* phsSocket,
                      path_struct settings, char *outputfile)
{
    void *shlib = NULL;
    char *error;
    char tmp_buff[2024];
    int i_tmp_buff;
    int rc, i;
    PF_NNI_GET_INTERFACE pf_nni_get_interface = 0;
    pnni_611_functions s_func;
    struct natural_exception nat_ex;
    struct parameter_description params[7];
    void *phandle = NULL;
    int ind[3] = {0, 0, 0}; 
    int occ[3] = {0, 0, 0};
    char req_type[5], settings_str[2024];

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
    printf("Logon into [%s]...", settings.library);
    if((rc = s_func->pf_nni_logon(s_func, settings.library, 0, 0)) != NNI_RC_OK)
    {
        printf("...Error. Nr: [%d]\n", rc);
        CloseLib(&shlib);
        return(-5);
    }
    printf("...Done\n");

    s_func->pf_nni_create_parm(s_func, 7, &phandle);

    occ[0] = 20;
    //Values
    s_func->pf_nni_init_parm_sa(s_func, 0, phandle, 'A', 2024, 0, 1, occ, 0);
    //Value Length
    s_func->pf_nni_init_parm_sa(s_func, 1, phandle, 'I', 4, 0, 1, occ, 0);
    //Keys
    s_func->pf_nni_init_parm_sa(s_func, 2, phandle, 'A', 100, 0, 1, occ, 0);
    //Parameter count
    s_func->pf_nni_init_parm_s(s_func, 3, phandle, 'I', 4, 0, 0);
    //Type
    s_func->pf_nni_init_parm_s(s_func, 4, phandle, 'A', 5, 0, 0);
    //File
    s_func->pf_nni_init_parm_s(s_func, 5, phandle, 'A', 100, 0, 0);
    //Settings
    s_func->pf_nni_init_parm_d(s_func, 6, phandle, 'A', 0);

    printf("There are [%d] POST Arguments\n", phsSocket->request.pchParamNumber);
    printf("Arguments:\n");
    for(i=0; i < phsSocket->request.pchParamNumber; i++)
    {
        printf("(Length: [%d]) [%s]=[%s]\n", phsSocket->request.stParams[i].valueLength,
            phsSocket->request.stParams[i].pchParamName, 
            phsSocket->request.stParams[i].pchParamValue);
    }

    for(ind[0]=0; ind[0] < occ[0]; ind[0]++)
    {
        memcpy(tmp_buff, phsSocket->request.stParams[ind[0]].pchParamValue, sizeof(tmp_buff));
        i_tmp_buff = phsSocket->request.stParams[ind[0]].valueLength;

        //if(ind[0] == 0)
        //    logHexDump(tmp_buff, sizeof(tmp_buff), stdout);
        rc = s_func->pf_nni_put_parm_array(s_func, 0, phandle, 2024, tmp_buff, ind);
        //sprintf(tmp_buff, "%d", i_tmp_buff)
        rc = s_func->pf_nni_put_parm_array(s_func, 1, phandle, 4, &i_tmp_buff, ind);

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
        s_func->pf_nni_put_parm_array(s_func, 2, phandle, 100, tmp_buff, ind);
    }

    s_func->pf_nni_put_parm(s_func, 3, phandle, 4, &phsSocket->request.pchParamNumber);

    strcpy(tmp_buff, req_type);
    s_func->pf_nni_put_parm(s_func, 4, phandle, 1024, tmp_buff);

    strcpy(tmp_buff, outputfile);
    s_func->pf_nni_put_parm(s_func, 5, phandle, 1024, tmp_buff);

    //strcpy(tmp_buff, "test=ok");
    sprintf(settings_str, "templates=%s;lib=%s;natsrc=%s", hp->template_path, settings.library, hp->natsource_path);
    s_func->pf_nni_put_parm(s_func, 6, phandle, strlen(settings_str)+1, settings_str);

    for(i=0; i < 7; i++)
    {
        s_func->pf_nni_get_parm_info(s_func, i, phandle, &params[i]);
    }


    //Call Subprog with no parms
    printf("Call subprogramm [%s]...", settings.programm);
    if(s_func->pf_nni_callnat(s_func, settings.programm, 7, params, &nat_ex) != NNI_RC_OK)
    {
        printf("...Error\n");
        printNaturalException(&nat_ex, outputfile);
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


int callNaturalProgram(HttpParam *hp, HttpSocket* phsSocket,
                      path_struct settings, char *outputfile)
{
    pid_t child;

    struct timeval tm1, tm2;
    unsigned long long duration;

    gettimeofday(&tm1, NULL);

    if((child = fork()) == -1)
    {
        printf("Child creation failed [%s]\n", strerror(errno));
        return(-1);
    }

    if(child == 0)
    {
        natural_child(hp, phsSocket, settings, outputfile);
        exit(0);
    }

    printf("Wait for pid: [%d]...\n", child);
    wait(NULL);
    printf("...OK\n");
    gettimeofday(&tm2, NULL);
    duration = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;
    printf("Execusion duration: [%llu]\n", duration);
    return(0);
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
void printNaturalException( struct natural_exception *pnatexcep , char *outputfile)
{
    struct variables anker;
    char error_str[2024];
    int error_line;

    anker.next = NULL;
    strcpy(anker.name, "anker");

    newStringVar(&anker, "error_str", pnatexcep->natMessageText);
    newStringVar(&anker, "library", pnatexcep->natLibrary);
    newIntVar(&anker, "error_nr", pnatexcep->natMessageNumber);

    start_jinjaparser(&anker, outputfile, "/u/it/a140734/C/realHtml4Natural/web_server/templates/error/nat_error.html",
                        error_str, &error_line);


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
