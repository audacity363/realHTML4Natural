#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <sys/ioctl.h>

#define VAR2NAME

#include "vars.h"
#include "error_vars.h"
#include "lda.h"
#include "rh4n.h"
#include "rh4n_utils.h"

#define NAME "Name: "
#define MAX_VARNAME_LENGTH 32

#define BOX_WIDTH (strlen(NAME)+MAX_VARNAME_LENGTH+4)

vars_t *g_lda_anker = NULL, *g_nat_anker = NULL;
int error_num, error;

vars_t *resolveName(vars_t *nat_set, vars_t *lda_set, vars_t *var_anker);
int main()
{
    int ret;

    char error_buff[2024];

    initVarAnker(&g_nat_anker);
    initVarAnker(&g_lda_anker);

    addString(g_nat_anker, NULL, "var1", L"val", 5);
    addString(g_nat_anker, NULL, "var2", L"val", 5);
    addString(g_nat_anker, NULL, "var3", L"val", 5);
    addString(g_nat_anker, NULL, "var4", L"val", 5);
    addString(g_nat_anker, NULL, "var5", L"val", 5);
    addString(g_nat_anker, NULL, "var6", L"val", 5);
    addString(g_nat_anker, NULL, "var7", L"val", 5);

    if((ret = startLDAParser("./TEST.NGL", g_lda_anker, NULL, error_buff)) != LDA_OK)
    {
        printf("Something went wrong [%s]\n", error_buff);
        return(-1);
    }

    //printfork(g_lda_anker, stdout);
    
    resolveName(g_nat_anker, g_lda_anker, g_nat_anker);

    return(0);
}

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
            p_print(g_lda_anker, g_nat_anker, cur_lda_set, cur_nat_set);
            printf("Enter to continue...");
            getchar();
            printf("================================================================================\n");

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
        if(strcmp(str_toLower(hptr->name), RH4N_GRP_HEAD) == 0)
        {
            return(hptr);
        }
        hptr = hptr->next;
    }

    return(NULL);
}
