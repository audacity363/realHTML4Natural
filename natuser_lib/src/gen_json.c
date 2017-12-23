/**
 * @file gen_json.c
 * @brief Contains the C function of the user exit "GENJSON"
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

extern char *must_set_settings[]; //!< is defined in gen_page.c
extern FILE *logfile; //!< is defined in gen_page.c

void trimSpaces(char*);
int parseSettings(void *parmhandle, pnni_611_functions nni_funcs,
    struct settings_s *settings, int);
int readOutVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    vars_t *anker);
void signalHandler(int signal);

/**
 * @brief Function for the "GENJSON" user exit
 *
 * This function reads out the given natural variables and saves it in a linked list with a temporary name and all it's attributes (type, length, array size) these     list get matched with a list from a parsed LDA and the name gets taken off to the variable list.
 * 
 * From this list JSON get's generated.
 *
 * @param nparms number of natural parameters given
 * @param parmhandle pointer to an array of natural parameters
 * @param traditional is set when natural didn't call the function with the INTERFACE4 keyword
 *
 * @returns long
 */
long gen_json(WORD nparms, void *parmhandle, void *traditional)
{
    int ret = 0, i = 0, var_index = 0;

    struct rh4n_parms parms = {NULL, NULL, NULL, {NULL, NULL, 0}};

    pnni_611_functions nni_funcs;
    void *shlib = NULL;
    char *logfilepath = NULL, *errorstr = NULL;

    vars_t *var_anker = NULL;

    FILE *outputfile = NULL;

    /*if((logfile = fopen("/tmp/t_logfile.log", "w")) == NULL)
    {
        return(100);
    }
    fprintf(logfile, "Lets Go\n");
    fflush(logfile);*/

    if(initVarAnker(&var_anker) != 0)
    {
        return(1);
    }


    if((errorstr = OpenLib(&shlib, NAT_LIB_NAME)) !=  NULL)
    {
        logfile = fopen("/tmp/rh4n_panic", "w");
        fprintf(logfile, "Error loading SO for NNI: [%s]\n", errorstr);
        fclose(logfile);
        return(2);
    }

    /*fprintf(logfile, "Done loading SO. Pointer: [%p]\n", shlib);
    fflush(logfile);*/

    if((nni_funcs = initNNI(shlib)) == 0x00)
    {
        CloseLib(&shlib);
        //fflush(logfile);
        //fclose(logfile);
        return(3);
    }

    //First read out the setting so we know if the logging should be enabled or not
    if((ret = parseSettings(parmhandle, nni_funcs, &parms.settings, SETTINGSPOS)) != NNI_RC_OK)
        goto cleanup;

    /*for(i=0; i < parms.settings.length; i++)
        fprintf(logfile, "[%s]=[%s]\n", parms.settings.key[i], parms.settings.value[i]);
    fflush(logfile);*/

    if(checkSettings(&parms.settings) < 0)
    {
        ret = 5;
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
    fprintf(logfile, "Group name: [%s]\n", parms.template_name);
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
    if((ret = getNames(var_anker, &parms, parms.template_name)) < 0)
    {
        goto cleanup;
    }
    //generatePage(var_anker, &parms);

    outputfile = fopen(parms.tmp_file, "w");
    printAllVarsToFileJSON(var_anker->next, outputfile);

    fclose(outputfile);

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
 * @brief Finally calls the JSON generator
 *
 * @param anker anker to the linked link list with natural variables (names included)
 * @param parms struct with settings for the framework
 *
 */
int generateJSON(vars_t *anker, struct rh4n_parms *parms)
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
