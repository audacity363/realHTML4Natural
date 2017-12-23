/**
 * @file gen_page.c
 *
 * @brief Contains the C function of the user exit "GENPAGE"
 *
 * @author Tom Engemann
 */
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
#include "utils.h"
#include "var2name.h"
#include "parser.h"
#include "natutils.h"

#define LDAPOS 0
#define TEMPLATEPOS 1
#define DELIVERFILEPOS 2
#define SETTINGSPOS 3

char *must_set_settings[] = {
    "templates",
    "lib",
    "natsrc"
}; //!< Keys in the settings string that must exist
FILE *logfile = NULL; //!< when debug is set in settings string this points to /tmp/<LDAname>.log if not to /dev/null

void trimSpaces(char*);
int parseSettings(void *parmhandle, pnni_611_functions nni_funcs,
    struct settings_s *settings, int);
int readOutVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    vars_t *anker);
void signalHandler(int signal);

/**
 * @brief Function for the "GENPAGE" user exit
 *
 * Everything started with this function.
 * This function reads out the given natural variables and saves it in a linked list with a temporary name and all it's attributes (type, length, array size) these     list get matched with a list from a parsed LDA and the name gets taken off to the variable list.
 * 
 * This list get's passed to a HTML parser which parses the given template
 *
 * @param nparms number of natural parameters given
 * @param parmhandle pointer to an array of natural parameters
 * @param traditional is set when natural didn't call the function with the INTERFACE4 keyword
 *
 * @returns long
 */

long gen_page(WORD nparms, void *parmhandle, void *traditional)
{
    int ret = 0, i = 0, var_index = 0;

    struct rh4n_parms parms = {NULL, NULL, NULL, {NULL, NULL, 0}};

    pnni_611_functions nni_funcs;
    void *shlib = NULL;
    char *logfilepath = NULL, *errorstr = NULL;

    vars_t *var_anker = NULL;

    /*if((logfile = fopen("/tmp/t_logfile.log", "w")) == NULL)
    {
        return(100);
    }
    fprintf(logfile, "Lets Go\n");
    fflush(logfile);*/

    if(initVarAnker(&var_anker) != 0)
    {
        /*fprintf(logfile, "Error while init var anker\n");
        fflush(logfile);
        fclose(logfile);*/
        return(-1);
    }


    if((errorstr = OpenLib(&shlib, NAT_LIB_NAME)) != NULL)
    {
        /*fprintf(logfile, "Error while loading natural.so\n");
        fflush(logfile);
        fclose(logfile);*/
        return(-2);
    }

    /*fprintf(logfile, "Done loading SO. Pointer: [%p]\n", shlib);
    fflush(logfile);*/

    if((nni_funcs = initNNI(shlib)) == 0x00)
    {
        CloseLib(&shlib);
        //fflush(logfile);
        //fclose(logfile);
        return(-3);
    }

    //First read out the setting so we know if the logging should be enabled or not
    if((ret = parseSettings(parmhandle, nni_funcs, &parms.settings, SETTINGSPOS)) != NNI_RC_OK)
        goto cleanup;

    for(i=0; i < parms.settings.length; i++)
        fprintf(logfile, "[%s]=[%s]\n", parms.settings.key[i], parms.settings.value[i]);
    fflush(logfile);

    if(checkSettings(&parms.settings) < 0)
    {
        ret = -5;
        goto cleanup;
    }



    fprintf(logfile, "parmhandle:[%p]\n\n", parmhandle);

    //Read out the LDA name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.lda_name, LDAPOS)) == -1)
    {
        /*fprintf(logfile, "ret: [%d]\n", ret);
        fflush(logfile);*/
        ret = RH4N_LDA_FORMAT_ERR;
        goto cleanup;
    }
    else if(ret != NNI_RC_OK)
        goto cleanup;

    trimSpaces(parms.lda_name);

    if(strcmp(getSetting(&parms.settings, "debug"), "true") == 0)
    {
        //Debug enabled log into /tmp/<LDANAME>.log
        logfilepath = malloc((10+strlen(parms.lda_name))*sizeof(char));
        sprintf(logfilepath, "/tmp/%s.log", parms.lda_name);

        if((logfile = fopen(logfilepath, "w")) == NULL)
        {
            ret = -1;
            goto cleanup;
        }
    }
    else
    {
        if((logfile = fopen("/dev/null", "w")) == NULL)
        {
            ret = -1;
            goto cleanup;
        }
    }

    fprintf(logfile, "LDAname: [%s]\n", parms.lda_name);
    fflush(logfile);

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
    fflush(logfile);

    //Read out the tmp file name    
    if((ret = getSettingParm(parmhandle, nni_funcs, &parms.tmp_file, DELIVERFILEPOS)) == -1)
    {
        ret = RH4N_TMPFILE_FORMAT_ERR;
        goto cleanup;
    }
    else if(ret != NNI_RC_OK)
        goto cleanup;

    
    fflush(logfile);
    for(var_index=4; var_index < nparms; var_index++)
    {
        if((ret = readOutVariable(var_index, parmhandle, nni_funcs, var_anker)) != 0)
        {
            goto cleanup;
        }
    }
    printAllVarsToFile(var_anker, logfile);
    if((ret = getNames(var_anker, &parms, RH4N_GRP_HEAD)) < 0)
    {
        goto cleanup;
    }
    generatePage(var_anker, &parms);

    printAllVarsToFile(var_anker, logfile);

cleanup:

    if(logfilepath != NULL)
        free(logfilepath);

    free(parms.lda_name);
    free(parms.template_name);

    if(ret != 0)
        logError(ret, parms.tmp_file, logfile);

    free(parms.tmp_file);
    for(i=0; i < parms.settings.length; i++)
    {
        free(parms.settings.key[i]);
        free(parms.settings.value[i]);
    }
    free(parms.settings.key);
    free(parms.settings.value);

    freeVarAnker(var_anker);

    CloseLib(&shlib);
    fclose(logfile);

    return(ret);
}

/**
 * @brief saves a natural variable in a linked list
 *
 * @param index specifies which natural parameter should get read out
 * @param parmhandle pointer to an array of natural parameters
 * @param nni_funcs an instance of loaded libnatural with loaded functions
 * @param anker points to the first element of the linked list with the variables
 *
 * @return int
 */
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
    fflush(logfile);

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
    fflush(logfile);

    return(ret);
}

/**
 * @brief Finally calls the HTML parser
 *
 * @param anker anker to the linked link list with natural variables (names included)
 * @param parms struct with settings for the framework
 *
 */
int generatePage(vars_t *anker, struct rh4n_parms *parms)
{
    char *template_path = NULL,
         *template_src_path = NULL;

    
    if((template_src_path = getSetting(&parms->settings, "templates")) == NULL)
    {
        fprintf(logfile, "templates not set\n");
        fflush(logfile);
        return(-1);
    }

    template_path = malloc((strlen(template_src_path)+strlen(parms->template_name)+1)*sizeof(char));

    sprintf(template_path, "%s/%s", template_src_path, parms->template_name);

    fprintf(logfile, "Using template [%s]\n", template_path);
    fflush(logfile);

    start(anker, template_path, parms->tmp_file, logfile);

    free(template_path);
}

/**
 * @brief calls the function which parses the LDA and matches the names
 *
 * @param anker anker to the linked link list with natural variables (names included)
 * @param parms struct with settings for the framework
 * @param groupname the name of the group which was hand over from natural
 *
 * @return int
 */
int getNames(vars_t *anker, struct rh4n_parms *parms, char *groupname)
{
    char error_buffer[2024],
        *lda_path = NULL,
        *natsrc = NULL, *lib = NULL;

    if((natsrc = getSetting(&parms->settings, "natsrc")) == NULL)
    {
        fprintf(logfile, "natsrc not set\n");
        fflush(logfile);
        return(-1);
    }
    if((lib = getSetting(&parms->settings, "lib")) == NULL)
    {
        fprintf(logfile, "lib not set\n");
        fflush(logfile);
        return(-2);
    }


    lda_path = malloc((strlen(natsrc)+strlen(lib)+strlen(parms->lda_name)+20)*sizeof(char));

    sprintf(lda_path, "%s/%s/GP/%s.NGL", natsrc, lib, parms->lda_name);
    fprintf(logfile, "LDA path [%s]\n", lda_path);
    fflush(logfile);
    
    if(startvar2name(anker, lda_path, true, logfile, groupname, error_buffer) != 0)
    {
        fprintf(logfile, "Error while loading names [%s]\n", error_buffer);
        fflush(logfile);
        free(lda_path);
        return(-3);
    }
    fflush(logfile);
    
    free(lda_path);
    return(0);
}

/**
 * @brief Just goes trough the settings struct and searches for the given key and returns it's value
 *
 * @param settings the settings struct
 * @param name name of the key
 *
 * @return char* (value of the given key/when nothing was found NULL)
 */
char *getSetting(struct settings_s *settings, char *name)
{
    int i=0;
    for(;i < settings->length; i++)
    {
        if(strcmp(name, settings->key[i]) == 0)
        {
            return(settings->value[i]);
        }
    }
    return(NULL);
}

/*
 * Read out a parameter for the framework
 */
/**
 * @brief Reads out parameter for the framework
 *
 * This function is for reading out the first four parameter. These are the
 * parms for the LDA name, template file|root group for JSON, tmpfile, settingsstr.
 * It checks for the type and length.
 *
 * @param parmhandle pointer to an array of natural parameters
 * @param nni_funcs an instance of loaded libnatural with loaded functions
 * @param name this is the return value for this function. It gets allocated for the length of the variable
 * @param pos position of which variable should gets read out
 *
 * @return int
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

/**
 * @brief Parses the settings string generated from the tomcat servlet
 *
 * The string has the following format: key1=value1;key2=value2;...
 * This function splits first on every ';' and than on every '='
 * and reserving an char* array for the keys and values in the settings_s struct.
 *
 * @param parmhandle pointer to an array of natural parameters
 * @param nni_funcs an instance of loaded libnatural with loaded functions
 * @param settings pointer to a instance of a settings struct
 */
int parseSettings(void *parmhandle, pnni_611_functions nni_funcs,
    struct settings_s *settings, int position)
{
    char *settings_str = NULL, *split_ptr = NULL, *equals_split = NULL,
         *s_ptr_sym = NULL, *s_ptr_eq = NULL;
    int ret = 0;

    settings->length = 0;
    settings->key = malloc(sizeof(char*));
    settings->value= malloc(sizeof(char*));

    if((ret = getSettingParm(parmhandle, nni_funcs, &settings_str, position)) == -1)
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
    //fprintf(logfile, "Settings: [%s]\n", settings_str);

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
        split_ptr = strtok_r(NULL, ";", &s_ptr_sym);
    }

    free(settings_str);
    return(NNI_RC_OK);
}

/**
 * @brief Checks if specific key are set in the settings string
 *
 * @param setings pointer to a instance of a settings_s struct
 */
int checkSettings(struct settings_s *settings)
{
    int length_of_settings_arr = sizeof(must_set_settings)/sizeof(char*),
        i = 0, x = 0,
        found[length_of_settings_arr];

    for(x=0; x < length_of_settings_arr; x++)
    {
        found[x] = 0;
    }

    for(; i < length_of_settings_arr; i++)
    {
        for(x=0; x < settings->length; x++)
        {
            if(strcmp(must_set_settings[i], settings->key[x]) == 0)
                found[i] = 1;
        }
        if(found[i] == 0)
        {
            printf("Error: Setting [%s] was not found\n", must_set_settings[i]);
            return(-1);
        }
    }
    return(0);
}

/**
 * @brief Prints out a error string for NNI return codes
 *
 * @param ret NNI return code
 */
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
    fflush(logfile);
    return(ret);
}
