#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VAR2NAME

#include "standard.h"
#include "vars.h"

#include "error_vars.h"
#include "lda.h"
#include "var2name.h"
#include "rh4n.h"
#include "rh4n_utils.h"

vars_t *searchPageStructure(vars_t *lda_anker, char*);
vars_t *matchNames(vars_t *, vars_t *, vars_t *, char *, bool, FILE*);
int compareVarCount(vars_t *, vars_t *, bool, FILE *, char *);
int countVarsinGroup(vars_t *grp, bool debug, FILE *logfile);

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

    /*if(debug)
        printfork(lda_anker, logfile);*/

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

    if(compareVarCount(var_anker, page_grp, debug, logfile, error_buff) != 0)
    {
        if(debug)
            fprintf(logfile, "%s\n", error_buff);
        return(-1);
    }

    fprintf(logfile, "Start Resolve name\n");
    fflush(logfile);
    //tmp = resolveName(var_anker->next, page_grp, var_anker);
    matchNames(var_anker->next, page_grp, var_anker, NULL, debug, logfile);
    /*if(error != 0)
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
    }*/

    freeLDAAnker(lda_anker);
    return(0);
}

/*
 * Counts how many variables are defined in a group. When it finds a new group
 * the function calls itself and returns the sum of alle variables in subgroups
 */
int countVarsinGroup(vars_t *grp, bool debug, FILE *logfile)
{
    if(debug)
        fprintf(logfile, "Count vars in %s\n", grp->name);

    int var_count = 0;
    vars_t *hptr = NULL;

    for(hptr=grp->next_lvl; hptr != NULL; hptr=hptr->next)
    {
        if(hptr->type == REDEFINE)
            continue;
        else if(hptr->type == GROUP)
            var_count += countVarsinGroup(hptr, debug, logfile);
        else
            var_count++;
    }
    if(debug)
        fprintf(logfile, "Found %d vars in %s\n", var_count, grp->name);

    return(var_count);
}

/*
 * Reads out how much variables are defined in each group and compares these numbers.
 *
 * @param nat_set vars from natural with the real values
 hptr->type == GROUP* @param lda_set vars freom the lda without value but with the real names
 * @param debug true = debug output false = no debug output
 * @param logfile file for the debug output
 *
 * @returns < 0 Error
 * @returns = 0 Variable count are equals
 * @returns > 0 Variable count are equals
 */
int compareVarCount(vars_t *nat_set, vars_t *lda_set, bool debug, FILE *logfile, char *error_buff)
{
    int lda_var_count = 0, nat_var_count = 0;
    vars_t *hptr = NULL;

    //in the original list from natural sould no group be defined
    for(hptr = nat_set->next; hptr != NULL; hptr=hptr->next)
    {
        if(hptr->next_lvl != NULL)
        {
            sprintf(error_buff, "variable on position %d has a next level\n", nat_var_count);
            return(-1);
        }
        nat_var_count++;
    }
    //nat_var_count--;

    //Count variables in the LDA Redefines and Group headers gets ignored
    for(hptr=lda_set; hptr != NULL; hptr=hptr->next) 
    {
        if(hptr->type == REDEFINE)
            continue;
        else if(hptr->type == GROUP)
            lda_var_count += countVarsinGroup(hptr, debug, logfile);
        else
            lda_var_count++;
    }

    if(debug)
        fprintf(logfile, "Found following vars: LDA: %d NAT: %d\n", lda_var_count, nat_var_count);


    if(lda_var_count != nat_var_count)
    {
        sprintf(error_buff, "Variable count not equals: Nat: %d LDA: %d\n", nat_var_count, lda_var_count);
        return(1);
    }
    return(0);
}

vars_t *matchNames(vars_t *nat_set, vars_t *lda_set, vars_t *anker, char *grpname, bool debug, FILE *logfile)
{
    vars_t *cur_lda_set = lda_set,
           *cur_nat_set = nat_set, *tmp;
    int ret;

    if(grpname)
    {
        if(GrpisDefined(anker, grpname) == false)
        {
            if((ret = addGroup(anker, grpname, 0,0,0)) != 0)
            {
                error_num = ret;
                error = RH4N_VAR_LIBRARY_ERR;
                return(NULL);
            }
        }
    }

    for(;cur_lda_set != NULL; cur_lda_set=cur_lda_set->next)
    {
        if(cur_lda_set->type == REDEFINE)
        {
            if(debug)
                fprintf(logfile, "Ignoring redefine\n");
            continue;
        }
        else if(cur_lda_set->type == GROUP)
        {
            if(debug)
                fprintf(logfile, "found Group. forking.... [%s]\n", cur_lda_set->name);
            cur_nat_set = matchNames(cur_nat_set, cur_lda_set->next_lvl, anker, cur_lda_set->name, debug, logfile);
        }
        else
        {
            if(debug)
                fprintf(logfile, "Rename %s to ", cur_nat_set->name);
            free(cur_nat_set->name);
            if((cur_nat_set->name = malloc(strlen(cur_lda_set->name)+1)) == NULL)
            {
                error = RH4N_NO_MEMORY;
                return(NULL);
            }
            if(debug) 
                fprintf(logfile, "%s\n", cur_lda_set->name);
            strcpy(cur_nat_set->name, cur_lda_set->name);

            //save next position
            tmp = cur_nat_set->next;

            //The function is in a recursion call. lda_set points to group fork
            if(grpname)
            {
                if(debug)
                    fprintf(logfile, "Moving %s to grp %s\n", cur_nat_set->name, grpname);
                if((ret = moveVariableToGroup(anker, cur_nat_set->name, grpname, &cur_nat_set)) != 0)
                {
                    error_num = ret;
                    error = RH4N_VAR_LIBRARY_ERR;
                    return(NULL);
                }
            }

            cur_nat_set = tmp;
        }
    }
    if(debug)
        fprintf(logfile, "going back\n\n");
    return(cur_nat_set);
}


int startmatchPosition(char *ldapath, vars_t *varanker, char *json_group, positioninfo_t *postab) {
    vars_t *lda_anker = NULL, *json_object_anker = NULL;
    char errorbuff[2048];
    int rc = 0, i = 0;

    initVarAnker(&lda_anker);

    if((rc = startLDAParser(ldapath, lda_anker, stdout, errorbuff)) != LDA_OK)
    {
        //TODO: Report Error
       //printf("Error: [%s]\n", errorbuff);
        return(RH4N_RET_LDA_PARSE_ERR);
    }

    if((json_object_anker = searchPageStructure(lda_anker, json_group)) == NULL) {
        //printf("Could not find structure: [%s]\n", json_group);
        freeLDAAnker(lda_anker);
        //TODO: Report Error
        return(RH4N_RET_UNKOWN_VAR);
    }
#if 0    
    printf("LDA:\n");
    printfork(json_object_anker, stdout);
    printf("Vars: \n");
    printAllVarsToFile(varanker, stdout);
#endif

    if((rc = matchPosition(json_object_anker->next_lvl, varanker, NULL, 0, postab)) < 0) {
        freeLDAAnker(lda_anker);
        //TODO: Report Error
        return(rc*-1);
    }

#if 0
    printf("Found following positions:\n");
    for(; i < postab.length; i++) {
        printf("Pos %d name: [%s]\n", postab.tab[i].parm_position, postab.tab[i].vartarget->name);
    }
#endif

    freeLDAAnker(lda_anker);
    return(RH4N_RET_OK);
}

int matchPosition(vars_t *ldalist, vars_t *varanker, char *group, int position, positioninfo_t *postab) {
    vars_t *hptr = ldalist, *target = NULL;

    for(; hptr != NULL; hptr=hptr->next) {
        if(hptr->type == REDEFINE)
            continue;
        else if(hptr->type == GROUP) {
            if((position = matchPosition(hptr->next_lvl, varanker, hptr->name, position, postab)) < 0) {
                return(position);
            }
        } else {
            if((target = isDefinedGrp(varanker, group, hptr->name)) != NULL) {
                //printf("Var [%s] on position [%d]\n", hptr->name, position);
                if(postab->length == -1) {
                    if((postab->tab = malloc(sizeof(varposition_t))) == NULL) {
                        return(RH4N_RET_MEMORY_ERR*-1);
                    }
                    postab->length = 1;
                } else {
                    if((postab->tab = realloc(postab->tab, sizeof(varposition_t) * (++postab->length))) == NULL) {
                        return(RH4N_RET_MEMORY_ERR*-1);
                    }
                }
                postab->tab[postab->length-1].vartarget = target;
                postab->tab[postab->length-1].parm_position = position;
            } 
#if 0
            else {
                if(group) { printf("Var [%s] [%s] does not exist in json object\n", group, hptr->name); }
                else { printf("Var [%s] does not exist in json object\n", hptr->name); }
            }
#endif
            position++;
        }
    }
    return(position);
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
