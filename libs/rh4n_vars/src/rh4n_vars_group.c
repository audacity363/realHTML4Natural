#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"

int rh4nvarCreateNewGroup(RH4nVarList *varlist, char *pgroupname) {
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;
    int varlibret = 0;

    if((varlibret = rh4nvarCreatenewVariable(varlist, NULL, pgroupname, RH4NVARTYPEGROUP, &_refvar)) != RH4N_RET_OK) { 
        return(varlibret); 
    }
    return(RH4N_RET_OK);
}


int rh4nvarMoveVarToGroup(RH4nVarList *varlist, char *pvarname, char *pgroupname) {
    RH4nVarRef _groupref = RH4NVAR_REF_INIT, _varref = RH4NVAR_REF_INIT;
    RH4nVarEntry_t *targetvar = NULL, *hptr = NULL;
    int varlibret = 0;

    if((varlibret = rh4nvarSearchVarRef(varlist->anker, pgroupname, &_groupref)) != RH4N_RET_OK) { return(varlibret); }
    if(_groupref.var->var.type != RH4NVARTYPEGROUP) { return(RH4N_RET_FORMAT_ERR); }

    if((varlibret = rh4nvarSearchVarRef(varlist->anker, pvarname, &_varref)) != RH4N_RET_OK) { return(varlibret); }
    
    targetvar = _varref.var;

    if(targetvar->prev == NULL) varlist->anker = targetvar->next;
    else targetvar->prev->next = targetvar->next;

    targetvar->prev = targetvar->next = NULL;

    if(_groupref.var->nextlvl == NULL) {
        _groupref.var->nextlvl = targetvar;
        targetvar->prev = _groupref.var;
    } else {
        for(hptr = _groupref.var->nextlvl; hptr->next != NULL; hptr = hptr->next);
        hptr->next = targetvar;
        targetvar->prev = hptr;
    }
    
    return(RH4N_RET_OK);
}

int rh4nvarGroupExist(RH4nVarList *varlist, char *pgroupname) {
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;

    if(rh4nvarGetRef(varlist, NULL, pgroupname, &_refvar) != RH4N_RET_OK) return(0);
    if(_refvar.var->var.type != RH4NVARTYPEGROUP) return(0);
    return(1);
}
