#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <wchar.h>
#include <string.h>
#include <signal.h>

#include "natuser.h"
#include "natni.h"
#include "vars.h"
#include "rh4n.h"

#define LDAPOS 0
#define TEMPLATEPOS 1
#define DELIVERFILEPOS 2
#define SETTINGSPOS 3

#define NAT_LIB_NAME "libnatural.so"
#define GET_INTERFACE_FUNC "nni_get_interface"

int OpenLib(void **shLib, char *name);
void CloseLib(void **shLib);
pnni_611_functions initNNI(void *lib);
void trimSpaces(char*);
int parseSettings(void *parmhandle, pnni_611_functions nni_funcs,
    struct settings_s *settings);
int readOutVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    vars_t *anker);
void signalHandler(int signal);

FILE *logfile = NULL;

long gen_page(WORD nparms, void *parmhandle, void *traditional)
{
    int ret = 0, i = 0, var_index = 0;

    struct rh4n_parms parms = {NULL, NULL, NULL, {NULL, NULL, 0}};

    pnni_611_functions nni_funcs;
    void *shlib = NULL;

    vars_t *var_anker = NULL;

    if(initVarAnker(&var_anker) != 0)
    {
        return(-1);
    }

    logfile = fopen("./logfile.log", "w");

    if(OpenLib(&shlib, NAT_LIB_NAME) < 0)
    {
        return(-2);
    }

    fprintf(logfile, "Done loading SO. Pointer: [%p]\n", shlib);

    if((nni_funcs = initNNI(shlib)) == 0x00)
    {
        CloseLib(&shlib);
        return(-3);
    }

    //Read out the LDA name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.lda_name, LDAPOS)) == -1)
    {
        fprintf(logfile, "ret: [%d]\n", ret);
        ret = RH4N_LDA_FORMAT_ERR;
        goto cleanup;
    }
    else if(ret != NNI_RC_OK)
        goto cleanup;

    trimSpaces(parms.lda_name);

    fprintf(logfile, "LDAname: [%s]\n", parms.lda_name);

    //Read out the template name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.template_name, TEMPLATEPOS)) == -1)
    {
        ret = RH4N_TEMPLATE_FORMAT_ERR;
        goto cleanup;
    }
    else if(ret != NNI_RC_OK)
        goto cleanup;

    trimSpaces(parms.template_name);
    fprintf(logfile, "Template name: [%s]\n", parms.template_name);

    //Read out the tmp file name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.tmp_file, DELIVERFILEPOS)) == -1)
    {
        ret = RH4N_TMPFILE_FORMAT_ERR;
        goto cleanup;
    }
    else if(ret != NNI_RC_OK)
        goto cleanup;

    if((ret = parseSettings(parmhandle, nni_funcs, &parms.settings)) != NNI_RC_OK)
        goto cleanup;

    for(i=0; i < parms.settings.length; i++)
        fprintf(logfile, "[%s]=[%s]\n", parms.settings.key[i], parms.settings.value[i]);

    fprintf(logfile, "parmhandle:[%p]\n\n", parmhandle);
    for(var_index=4; var_index < nparms; var_index++)
    {
        if((ret = readOutVariable(var_index, parmhandle, nni_funcs, var_anker)) != 0)
        {
            goto cleanup;
        }
    }
    printAllVarsToFile(var_anker, logfile);

cleanup:

    free(parms.lda_name);
    free(parms.template_name);
    free(parms.tmp_file);
    for(i=0; i < parms.settings.length; i++)
    {
        free(parms.settings.key[i]);
        free(parms.settings.value[i]);
    }
    free(parms.settings.key);
    free(parms.settings.value);

    CloseLib(&shlib);

    return(ret);
}

int readOutVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    vars_t *anker)
{
    struct parameter_description pd;

    char tmp_name[NNI_LEN_NAME], teststr[200];
    int ret = 0;

    ret = nni_funcs->pf_nni_get_parm_info(nni_funcs, index, parmhandle, &pd);
    if(checkNNIReturnCode(ret) != NNI_RC_OK)
    {
        return(ret);
    }

    sprintf(tmp_name, "tmp%d", index);

    fprintf(logfile, "Format: [%c]\n", pd.format);
    fprintf(logfile, "Length: [%d]\n", pd.length);
    fprintf(logfile, "INdex: [%d]\n", index);

    switch(pd.format)
    {
        case NNI_TYPE_ALPHA:
            fprintf(logfile, "Alpha Variable\n");
            ret = handleAlphaVariable(index, parmhandle, nni_funcs,
                    pd, tmp_name, anker);
            break;
        case NNI_TYPE_FLOAT:
            break;
        case NNI_TYPE_LOG:
            ret = handleLogicVariable(index, parmhandle, nni_funcs,
                    pd, tmp_name, anker);
            break;
        case NNI_TYPE_NUM:
            ret = handleNumericVariable(index, parmhandle, nni_funcs,
                    pd, tmp_name, anker);
            break;
        case NNI_TYPE_INT:
            ret = handleIntegerVariable(index, parmhandle, nni_funcs,
                    pd, tmp_name, anker);
            break;
        case NNI_TYPE_UNICODE:
            ret = handleUnicodeVariable(index, parmhandle, nni_funcs,
                    pd, tmp_name, anker);
            break;
        default:
            //unsupported variablen type
            fprintf(logfile, "Unsupported variablen type: [%c]\n", pd.format);
            break;
    }

    return(ret);
}

/*
 * Read out a parameter for the framework
 */
int getSettingParm(void *parmhandle, pnni_611_functions nni_funcs, char **name, int pos)
{
    struct parameter_description pd;
    int ret = 0;

    ret = nni_funcs->pf_nni_get_parm_info(nni_funcs, pos, parmhandle, &pd);
    if(checkNNIReturnCode(ret) != NNI_RC_OK)
        return(ret);

    if(pd.format != NNI_TYPE_ALPHA || pd.dimensions != 0)
        return(-1);
    else if(pd.length_all == 0)
        return(RH4N_NULL_LENGTH_ERR);

    if((*name = malloc((pd.length_all+1)*sizeof(char))) == NULL)
        return(RH4N_NO_MEMORY);

    memset(*name, 0x00, (pd.length_all+1)*sizeof(char));

    ret = nni_funcs->pf_nni_get_parm(nni_funcs, pos, parmhandle,
            (pd.length_all)*sizeof(char), *name);
    if(checkNNIReturnCode(ret) != NNI_RC_OK)
    {
        free(*name);
        return(ret);
    }

    return(NNI_RC_OK);
}

int parseSettings(void *parmhandle, pnni_611_functions nni_funcs,
    struct settings_s *settings)
{
    char *settings_str = NULL, *split_ptr = NULL, *equals_split = NULL,
         *s_ptr_sym = NULL, *s_ptr_eq = NULL;
    int ret = 0;

    settings->length = 0;
    settings->key = malloc(sizeof(char*));
    settings->value= malloc(sizeof(char*));

    if((ret = getSettingParm(parmhandle, nni_funcs, &settings_str, SETTINGSPOS)) == -1)
    {
        free(settings_str);
        return(RH4N_SETTING_FORMAT_ERR);
    }
    else if(ret != NNI_RC_OK)
    {
        free(settings_str);
        return(ret);
    }

    trimSpaces(settings_str);

    split_ptr = strtok_r(settings_str, ";", &s_ptr_sym);
    while(split_ptr != NULL)
    {
        equals_split = strtok_r(split_ptr, "=", &s_ptr_eq);
        settings->key[settings->length] = malloc((strlen(equals_split)+1)*sizeof(char));
        strcpy(settings->key[settings->length], equals_split);

        equals_split = strtok_r(NULL, "=", &s_ptr_eq);
        settings->value[settings->length] = malloc((strlen(equals_split)+1)*sizeof(char));
        strcpy(settings->value[settings->length], equals_split);

        settings->length++;
        settings->key = realloc(settings->key, (settings->length+1)*sizeof(char*));
        settings->value = realloc(settings->value, (settings->length+1)*sizeof(char*));
        split_ptr = strtok_r(NULL, ";", &s_ptr_eq);
    }

    free(settings_str);
    return(NNI_RC_OK);
}

int checkNNIReturnCode(int ret)
{
    if(ret == NNI_RC_OK)
        return(NNI_RC_OK);

    fprintf(logfile, "A internal NaturalNativeInterface Error occurred:\n");
    fprintf(logfile, "-------------------------------------------------\n");
    switch(ret)
    {
        case NNI_RC_ILL_PNUM:
            fprintf(logfile, "Invalid parameter number\n");
            break;
        case NNI_RC_INT_ERROR:
            fprintf(logfile, "Internal NNI error\n");
            break;
        case NNI_RC_DATA_TRUNC:
            fprintf(logfile, "Data has been truncated during parameter value access\n");
            break;
        case NNI_RC_NOT_ARRAY:
            fprintf(logfile, "Parameter is not an array\n");
            break;
        case NNI_RC_WRT_PROT:
            fprintf(logfile, "Parameter is write protected\n");
            break;
        case NNI_RC_NO_MEMORY:
            fprintf(logfile, "Memory allocation failed\n");
            break;
        case NNI_RC_BAD_FORMAT:
            fprintf(logfile, "Invalid Natural data type\n");
            break;
        case NNI_RC_BAD_LENGTH:
            fprintf(logfile, "Invalid length or precision\n");
            break;
        case NNI_RC_BAD_DIM:
            fprintf(logfile, "Invalid dimension count\n");
            break;
        case NNI_RC_BAD_BOUNDS:
            fprintf(logfile, "Invalid x-array bound definition\n");
            break;
        case NNI_RC_NOT_RESIZABLE:
            fprintf(logfile, "Array cannot be resized in the requested way\n");
            break;
        case NNI_RC_BAD_INDEX_0:
            fprintf(logfile, "Index for array dimension 0 out of range\n");
            break;
        case NNI_RC_BAD_INDEX_1:
            fprintf(logfile, "Index for array dimension 1 out of range\n");
            break;
        case NNI_RC_BAD_INDEX_2:
            fprintf(logfile, "Index for array dimension 2 out of range\n");
            break;
        case NNI_RC_VERSION_ERROR:
            fprintf(logfile, "Requested interface version not supported\n");
            break;
        case NNI_RC_NOT_INIT:
            fprintf(logfile, "No Natural session initialized in this interface instance\n");
            break;
        case NNI_RC_NOT_IMPL:
            fprintf(logfile, "Function not implemented in this interface version\n");
            break;
        case NNI_RC_PARM_ERROR:
            fprintf(logfile, "Mandatory parameter not specified\n");
            break;
        case NNI_RC_LOCKED:
            fprintf(logfile, "Interface instance is locked by another thread\n");
            break;
        case IF4_RC_BAD_VERSION:
            fprintf(logfile, "invalid version of interface struct (or trad. CALL)\n");
            break;
        default:
            fprintf(logfile, "Unkown Error: [%d]\n", ret);
    }
    fprintf(logfile, "-------------------------------------------------\n");
    return(ret);
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

pnni_611_functions initNNI(void *lib)
{
    pnni_611_functions s_funcs;
    PF_NNI_GET_INTERFACE pf_nni_get_interface = 0;
    int rc = 0;
    char *error = NULL;

    if(!(pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(lib, GET_INTERFACE_FUNC)))
    {
        error = dlerror();
        fprintf(logfile, "Error while loading Function [%s]: [%s]\n", GET_INTERFACE_FUNC,
            error);
        return(NULL);
    }

    if(((pf_nni_get_interface)(NNI_VERSION_611, (void**)&s_funcs)) != NNI_RC_OK)
    {
        fprintf(logfile, "...Error while gettings Function Table\n");
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
        error = dlerror();
        fprintf(logfile, "Error while loading Module [%s]: [%s]\n", name, error);
        return(-1);
    }
    return(0);
}

void CloseLib(void **shLib)
{
    dlclose(*shLib);
}

void trimSpaces(char *input)
{
    int length = strlen(input);

    for(; length >= -1; length--)
    {
        if(input[length] != 0x20 && input[length] != 0x00)
        {
            input[length+1] = '\0';
            break;
        }
    }
}

void trimleadingSpaces(char *input)
{
    int length = strlen(input), i;

    for(i=0; i < length; i++)
    {
        if(input[i] != 0x20)
            break;
    }

    length++;

    memmove(input, input+i, length-i);
}

void trimSpaces4B(wchar_t *input)
{
    int byte_length = wcslen(input)*sizeof(wchar_t);

    void *start = (void*)input,
         *end = ((void*)input)+byte_length;
    char c_null = 0x00,
         c_space = 0x20;

    while(end != start)
    {
        if(memcmp(end, &c_null, 1) != 0 && memcmp(end, &c_space, 1) != 0)
            break;
        memset(end, 0x00, 1);
        end--;
    }

    
}
