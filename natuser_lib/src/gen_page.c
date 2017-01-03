#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <wchar.h>
#include <string.h>

#include "natuser.h"
#include "natni.h"
#include "vars.h"

#define LDAPOS 0
#define TEMPLATEPOS 1
#define DELIVERFILEPOS 2
#define SETTINGSPOS 3

#define RH4N_NULL_LENGTH_ERR 100
#define RH4N_NO_MEMORY 200
#define RH4N_LDA_FORMAT_ERR 300
#define RH4N_TEMPLATE_FORMAT_ERR 400
#define RH4N_TMPFILE_FORMAT_ERR 500
#define RH4N_SETTING_FORMAT_ERR 500

struct settings_s {
    char **key;
    char **value;
    int length;
};

struct rh4n_parms {
    char *lda_name;
    char *template_name;
    char *tmp_file;
    struct settings_s settings;
};

long gen_page(WORD nparms, void *parmhandle, void *traditional)
{
    int ret = 0;

    struct rh4n_parms parms = {NULL, NULL, NULL, {NULL, NULL, 0}};

    pnni_611_functions nni_funcs;

    //Read out the LDA name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.lda_name, LDAPOS)) != -1)
    {
        return(RH4N_LDA_FORMAT_ERR);
    }
    else if(ret != NNI_RC_OK)
        return(ret);

    //Read out the template name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.template_name, TEMPLATEPOS)) != -1)
    {
        return(RH4N_TEMPLATE_FORMAT_ERR);
    }
    else if(ret != NNI_RC_OK)
        return(ret);

    //Read out the tmp file name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.tmp_file, DELIVERFILEPOS)) != -1)
    {
        return(RH4N_TMPFILE_FORMAT_ERR);
    }
    else if(ret != NNI_RC_OK)
        return(ret);

    
    return(0);
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

    if((ret = getSettingParm(parmhandle, nni_funcs, &settings_str, SETTINGSPOS)) != -1)
    {
        free(settings_str);
        return(RH4N_SETTING_FORMAT_ERR);
    }
    else if(ret != NNI_RC_OK)
    {
        free(settings_str);
        return(ret);
    }

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
    }

    free(settings_str);
    return(NNI_RC_OK);
}

int checkNNIReturnCode(int ret)
{
    if(ret == NNI_RC_OK)
        return(NNI_RC_OK);

    printf("A internal NaturalNativeInterface Error occurred:\n");
    printf("-------------------------------------------------\n");
    switch(ret)
    {
        case NNI_RC_ILL_PNUM:
            printf("Invalid parameter number\n");
            break;
        case NNI_RC_INT_ERROR:
            printf("Internal NNI error\n");
            break;
        case NNI_RC_DATA_TRUNC:
            printf("Data has been truncated during parameter value access\n");
            break;
        case NNI_RC_NOT_ARRAY:
            printf("Parameter is not an array\n");
            break;
        case NNI_RC_WRT_PROT:
            printf("Parameter is write protected\n");
            break;
        case NNI_RC_NO_MEMORY:
            printf("Memory allocation failed\n");
            break;
        case NNI_RC_BAD_FORMAT:
            printf("Invalid Natural data type\n");
            break;
        case NNI_RC_BAD_LENGTH:
            printf("Invalid length or precision\n");
            break;
        case NNI_RC_BAD_DIM:
            printf("Invalid dimension count\n");
            break;
        case NNI_RC_BAD_BOUNDS:
            printf("Invalid x-array bound definition\n");
            break;
        case NNI_RC_NOT_RESIZABLE:
            printf("Array cannot be resized in the requested way\n");
            break;
        case NNI_RC_BAD_INDEX_0:
            printf("Index for array dimension 0 out of range\n");
            break;
        case NNI_RC_BAD_INDEX_1:
            printf("Index for array dimension 1 out of range\n");
            break;
        case NNI_RC_BAD_INDEX_2:
            printf("Index for array dimension 2 out of range\n");
            break;
        case NNI_RC_VERSION_ERROR:
            printf("Requested interface version not supported\n");
            break;
        case NNI_RC_NOT_INIT:
            printf("No Natural session initialized in this interface instance\n");
            break;
        case NNI_RC_NOT_IMPL:
            printf("Function not implemented in this interface version\n");
            break;
        case NNI_RC_PARM_ERROR:
            printf("Mandatory parameter not specified\n");
            break;
        case NNI_RC_LOCKED:
            printf("Interface instance is locked by another thread\n");
            break;
        default:
            printf("Unkown Error: [%d]\n", ret);
    }
    printf("-------------------------------------------------\n");
    return(ret);
}
