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

vars_t *resolveName(vars_t *cur_var, vars_t *group, vars_t *var_anker);
vars_t *searchPageStructure(vars_t *lda_anker, char*);

int error_num = 0;
int error = false;

int startvar2name(vars_t *var_anker, char *lda_path, bool debug, FILE *logfile, char *groupname, char *error_buff)
{
    vars_t *tmp = NULL, *lda_anker = NULL, *page_grp = NULL;
    int ret = 0;

    initVarAnker(&lda_anker);

    fprintf(logfile, "Start parsing LDA [%s]\n", lda_path);
    fprintf(logfile, "Debug = [%d]\n", debug);
    fflush(logfile);
    if((ret = startLDAParser(lda_path, lda_anker, logfile, error_buff)) != LDA_OK)
    {
        return(-1);
    }

    if(debug)
        printfork(lda_anker, logfile);

    fflush(logfile);
        

    if((page_grp  = searchPageStructure(lda_anker, groupname)) == NULL)
    {
        sprintf(error_buff, "ERROR: Structure [%s] not found in LDA: [%s]\n", groupname, lda_path);
        fprintf(logfile, "%s", error_buff);
        fflush(logfile);
        return(-2);
    }
    page_grp = page_grp->next_lvl;

    if(debug) {
        fprintf(logfile, "Page Struct\n");
        printfork(page_grp, logfile);
    }
    fflush(logfile);

    fprintf(logfile, "Start Resolve name\n");
    fflush(logfile);
    tmp = resolveName(var_anker->next, page_grp, var_anker);
    if(error != 0)
    {
        switch(error)
        {
            case RH4N_VAR_LIBRARY_ERR:
                sprintf(error_buff, "ERROR: [%s]\n", var_errorstrs[error_num]);
                fprintf(logfile, "%s", error_buff);
                fflush(logfile);
                break;
            case RH4N_NO_MEMORY:
                sprintf(error_buff, "Something went totaly wrong: Error while allocating Memory\n");
                fprintf(logfile, "%s", error_buff);
                fflush(logfile);
                break;
        }
        return(-3);
    }

    freeLDAAnker(lda_anker);
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
        else if(cur_lda_set->type == REDEFINE)
        {
            cur_lda_set = cur_lda_set->next;
            continue;
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
                printf("Move to group [%s]\n", grp_name);
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
        if(cur_lda_set->next == NULL && in_grp)
        {
            while(cur_lda_set->type != GROUP)
            {
                cur_lda_set = cur_lda_set->prev;
            }
            in_grp = false;
        }
        cur_lda_set = cur_lda_set->next;
    }
    return(cur_nat_set);
}

vars_t *searchPageStructure(vars_t *lda_anker, char *groupname)
{
    vars_t *hptr = lda_anker->next;

    char *tmp = NULL;

    while(hptr)
    {
        if(strcmp(str_toLower(hptr->name), groupname) == 0)
        {
            return(hptr);
        }
        hptr = hptr->next;
    }

    return(NULL);
}
