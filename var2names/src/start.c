#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "lda.h"
#include "var2name.h"
#include "rh4n.h"


vars_t *resolveName(vars_t *cur_var, vars_t *group, vars_t *var_anker, bool debug, FILE *logfile);

int error_num = 0;
int error = false;

int startvar2name(vars_t *var_anker, char *lda_path, bool debug, FILE *logfile)
{
    vars_t *tmp = NULL, *lda_anker = NULL;

    initVarAnker(&lda_anker);

    start(lda_path, lda_anker);

    tmp = resolveName(var_anker, lda_anker, var_anker);
    return(0);
}

//Redefines are not supported because the Interface4 API just gives the original value
//and not the redefines ones. I can redefine the variable values by myself based on the
//values from the parsed LDA. I must investigate the behavor from the natural redefine further.
//What happen if a intger or a dynamic variable get redefined?
vars_t *resolveName(vars_t *nat_set, vars_t *lda_set, vars_t *var_anker, bool debug, FILE *logfile)
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
        }
        else
        {
            free(cur_nat_set->name);
            if((cur_nat_set->name = malloc(strlen(cur_lda_set->name)+1)) == NULL)
            {
                error = RH4N_NO_MEMORY;
                return(NULL)
            }
            strcpy(cur_nat_set->name, cur_lda_set->name);

            if(in_grp == true)
            
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
