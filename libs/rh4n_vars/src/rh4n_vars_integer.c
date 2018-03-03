#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"

int rh4nvarCreateNewInt(RH4nVarList *varlist, char *pgroupname, char *pname, int value) {
    int varlibret = 0, varsize = 0;
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;

    if((varlibret = rh4nvarCreatenewVariable(varlist, pgroupname, pname, RH4NVARTYPEINTEGER, &_refvar)) != RH4N_RET_OK) { 
        return(varlibret); 
    }

    if((varlibret = rh4nvarSetInt(&_refvar.var->var, value)) != RH4N_RET_OK) { return(varlibret); }
    return(RH4N_RET_OK);
}


int rh4nvarEditInt(RH4nVarList *varlist, char *pgroupname, char *pname, int value) {
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;
    int varlibret = 0;

    if((varlibret = rh4nvarGetRef(varlist, pgroupname, pname, &_refvar)) != RH4N_RET_OK) { return(varlibret); }
    if(_refvar.var->var.type != RH4NVARTYPEINTEGER) { return(RH4N_RET_FORMAT_ERR); }
    if((varlibret = rh4nvarSetInt(&_refvar.var->var, value)) != RH4N_RET_OK) { return(varlibret); }
    return(RH4N_RET_OK);
}

int rh4nvarGetInt(RH4nVarList *varlist, char *pgroupname, char *pname, int *outbuff) {
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;
    int varlibret = 0;

    if((varlibret = rh4nvarGetRef(varlist, pgroupname, pname, &_refvar)) != RH4N_RET_OK) { return(varlibret); }
    if(_refvar.var->var.type != RH4NVARTYPEINTEGER) { return(RH4N_RET_FORMAT_ERR); }
    *outbuff = *((int*)_refvar.var->var.value);
    return(RH4N_RET_OK);
}

int rh4nvarCreateNewIntArray(RH4nVarList *varlist, char *pgroupname, char *pname, int dimensions, int length[3]) {
    return(rh4nvarCreateNewArray(varlist, pgroupname, pname, dimensions, length, RH4NVARTYPEINTEGER));
}

int rh4nvarSetIntArrayEntry(RH4nVarList *varlist, char *pgroupname, char *pname, int index[3], int value) {
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;
    RH4nVarObj *arrayentry = NULL;
    int varlibret = 0, varsize = 0;

    if((varlibret = rh4nvarGetRef(varlist, pgroupname, pname, &_refvar)) != RH4N_RET_OK) { return(varlibret); }
    if(_refvar.var->var.type != RH4NVARTYPEARRAY) { return(RH4N_RET_VAR_NOT_ARRAY); }
    if((varlibret = rh4nvarGetArrayEntry(&_refvar.var->var, index, &arrayentry)) != RH4N_RET_OK) { return(varlibret); }

    if(arrayentry->type != RH4NVARTYPEINTEGER) { return(RH4N_RET_FORMAT_ERR); }
    if((varlibret = rh4nvarSetInt(arrayentry, value)) != RH4N_RET_OK) { return(varlibret); }
    return(RH4N_RET_OK);
}

int rh4nvarGetIntArrayEntry(RH4nVarList *varlist, char *pgroupname, char *pname, int index[3], int *outbuff) {
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;
    RH4nVarObj *arrayentry = NULL;
    int varlibret = 0, varsize = 0;

    if((varlibret = rh4nvarGetRef(varlist, pgroupname, pname, &_refvar)) != RH4N_RET_OK) { return(varlibret); }
    if(_refvar.var->var.type != RH4NVARTYPEARRAY) { return(RH4N_RET_VAR_NOT_ARRAY); }
    if((varlibret = rh4nvarGetArrayEntry(&_refvar.var->var, index, &arrayentry)) != RH4N_RET_OK) { return(varlibret); }

    if(arrayentry->type != RH4NVARTYPEINTEGER) { return(RH4N_RET_FORMAT_ERR); }
    *outbuff = *((int*)arrayentry->value);
    return(RH4N_RET_OK);
}

int rh4nvarSetInt(RH4nVarObj *target, int value) {
    int varsize = 0;

    varsize = sizeof(int);
    if(target->value == NULL) { 
        if((target->value = malloc(varsize)) == NULL) { return(RH4N_RET_MEMORY_ERR); }
    }

    memcpy(target->value, &value, sizeof(int));
    target->length = varsize;
    return(RH4N_RET_OK);
}

