#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VAR2NAME

#include "vars.h"
#include "error_vars.h"
#include "lda.h"
#include "var2name.h"
#include "rh4n.h"
#include "rh4n_utils.h"

#define LOGDEBUG(debug, logfile, fmt, args...) if(debug == true) \
                                               {fprintf(logfile, fmt, ##args);}

vars_t *resolveName(vars_t *cur_var, vars_t *group, vars_t *var_anker);
vars_t *searchPageStructure(vars_t *lda_anker);

int error_num = 0;
int error = false;

int startvar2name(vars_t *var_anker, char *lda_path, bool debug, FILE *logfile)
{
    vars_t *tmp = NULL, *lda_anker = NULL, *page_grp = NULL;
    int ret = 0;

    initVarAnker(&lda_anker);

    if((ret = startLDAParser(lda_path, lda_anker, debug, logfile)) != LDA_OK)
    {
        return(-1);
    }

    printfork(lda_anker);

    if((page_grp  = searchPageStructure(lda_anker)) == NULL)
    {
        if(debug == true)
            fprintf(logfile, "ERROR: [%s] not found in LDA: [%s]\n", RH4N_GRP_HEAD, lda_path);
        return(-1);
    }
    page_grp = page_grp->next_lvl;

    tmp = resolveName(var_anker, page_grp, var_anker);
    if(error != 0)
    {
        switch(error)
        {
            case RH4N_VAR_LIBRARY_ERR:
                if(debug == true)
                    fprintf(logfile, "ERROR: [%s]\n", var_errorstrs[error_num]);
                break;
        }
        return(-1);
    }

    return(0);
}

//Redefines are not supported because the Interface4 API just gives the original value
//and not the redefines ones. I can redefine the variable values by myself based on the
//values from the parsed LDA. I must investigate the behavor from the natural redefine further.
//What happen if a intger or a dynamic variable get redefined?
vars_t *resolveName(vars_t *nat_set, vars_t *lda_set, vars_t *var_anker)
{
    vars_t *cur_lda_set = lda_set,
           *cur_nat_set = nat_set;
    bool in_grp = false;
    char *grp_name = NULL;
    int ret = 0;

    if(cur_lda_set->type == GROUP)
    {
        if(GrpisDefined(var_anker, cur_lda_set->name) == false)
        {
            if((ret = addGroup(var_anker, cur_lda_set->name, 0,0,0)) != 0)
            {
                error_num = ret;
                error = RH4N_VAR_LIBRARY_ERR;
                return(NULL);
            }
        }
        in_grp = true;
        grp_name = cur_lda_set->name;
        cur_lda_set = cur_lda_set->next_lvl;
    }

    while(cur_lda_set && cur_nat_set)
    {
        if(cur_lda_set->type == GROUP)
        {
            cur_nat_set = resolveName(cur_nat_set, cur_lda_set, var_anker);
            if(error != 0)
                return(NULL);
        }
        else
        {
            free(cur_nat_set->name);
            if((cur_nat_set->name = malloc(strlen(cur_lda_set->name)+1)) == NULL)
            {
                error = RH4N_NO_MEMORY;
                return(NULL);
            }
            strcpy(cur_nat_set->name, cur_lda_set->name);

            if(in_grp == true)
            {            
                if((ret = moveVariableToGroup(var_anker, cur_nat_set->name, grp_name, &cur_nat_set)) != 0)
                {
                    error_num = ret;
                    error = RH4N_VAR_LIBRARY_ERR;
                    return(NULL);
                }
            }
            else
            {
                cur_nat_set = cur_nat_set->next;
            }
        }
        cur_lda_set = cur_lda_set->next;
    }
    return(cur_nat_set);
}

vars_t *searchPageStructure(vars_t *lda_anker)
{
    vars_t *hptr = lda_anker->next;

    char *tmp = NULL;

    while(hptr)
    {
        tmp = str_toLower(hptr->name);
        if(strcmp(tmp, RH4N_GRP_HEAD) == 0)
        {
            return(hptr);
        }
        hptr = hptr->next;
    }

    return(NULL);
}
