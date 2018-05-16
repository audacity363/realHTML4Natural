#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh4n.h"

int rh4nvarCreateNewArray(RH4nVarList *varlist, char *pgroupname, char *pname, int dimensions, int length[3], int vartype) {
    int createret = 0, i = 0, x = 0;
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;
    RH4nVarObj *ytarget = NULL, *ztarget = NULL;

    if(dimensions < 1 && dimensions > 3) { return(RH4N_RET_VAR_BAD_DIM); }
    if((createret = rh4nvarCheckDimLength(dimensions, length)) != RH4N_RET_OK) { return(createret); }

    if((createret = rh4nvarCreatenewVariable(varlist, pgroupname, pname, RH4NVARTYPEARRAY, &_refvar)) != RH4N_RET_OK) { 
        return(createret); 
    }

    if(length[0] == 0) {
        _refvar.var->var.length = 0;
        _refvar.var->var.value = NULL;
        return(RH4N_RET_OK);
    }
    if((createret = rh4nvarInitArray(&_refvar.var->var, length[0], 
       RH4NVAR_ARRAY_INIT_TYPE(dimensions, 1, vartype))) != RH4N_RET_OK) { return(createret); }

    if(dimensions == 1) { return(RH4N_RET_OK); }

    for(i = 0; i < length[0]; i++) {
        ytarget = &((RH4nVarObj*)_refvar.var->var.value)[i];
        if(length[1] == 0) {
            ytarget->length = 0;
            ytarget->value = NULL;
            continue;
        }
        if((createret = rh4nvarInitArray(ytarget, length[1], 
           RH4NVAR_ARRAY_INIT_TYPE(dimensions, 2, vartype))) != RH4N_RET_OK) { return(createret); }
        if(dimensions == 3) {
            for(x = 0; x < length[1]; x++) {
                ztarget = &((RH4nVarObj*)ytarget->value)[x];
                if(length[2] == 0) {
                    ztarget->length = 0;
                    ztarget->value = NULL;
                    continue;
                }
                if((createret = rh4nvarInitArray(ztarget, length[2], 
                    RH4NVAR_ARRAY_INIT_TYPE(dimensions, 3, vartype))) != RH4N_RET_OK) { return(createret); }
            }
        }
    }

    return(RH4N_RET_OK);
}

int rh4nvarInitArray(RH4nVarObj *target, int length, int vartype) {
    int i = 0;

    if((target->value = malloc(sizeof(RH4nVarObj)*length)) == NULL) { return(RH4N_RET_MEMORY_ERR); }

    target->length = length;

    for(; i < length; i++) {
        ((RH4nVarObj*)target->value)[i].type = vartype;
        ((RH4nVarObj*)target->value)[i].value = NULL;
        ((RH4nVarObj*)target->value)[i].length = -1;
    }

    return(RH4N_RET_OK);
}

int rh4nvarCheckDimLength(int dimensions, int length[3]) {
    if(dimensions == 3 && (length[2] == 0 && length[1] == 0 && length[0] == 0)) return(RH4N_RET_OK);
    if(dimensions == 2 && (length[1] == 0 && length[0] == 0)) return(RH4N_RET_OK);
    if(dimensions == 1 && length[0] == 0) return(RH4N_RET_OK);

    if(dimensions == 1 && (length[0] < 0)) { return(RH4N_RET_VAR_BAD_LENGTH); }
    if(dimensions == 2 && (length[0] <= 0 || length[1] < 0)) { return(RH4N_RET_VAR_BAD_LENGTH); }
    if(dimensions == 3 && (length[0] <= 0 || length[1] <= 0 || length[2] < 0)) { return(RH4N_RET_VAR_BAD_LENGTH); }
    return(RH4N_RET_OK);
}

int rh4nvarGetArrayEntry(RH4nVarObj *target, int index[3], RH4nVarObj **paentry) {
    if(target->type != RH4NVARTYPEARRAY) { return(RH4N_RET_VAR_NOT_ARRAY); }
    if(index[0] > target->length || index[0] < 0) { return(RH4N_RET_VAR_BAD_INDEX); }

    RH4nVarObj *xtarget = NULL, *ytarget = NULL;

    xtarget = &((RH4nVarObj*)target->value)[index[0]];
    if(index[1] == -1) {
        *paentry = xtarget;
        return(RH4N_RET_OK);
    }

    if(index[1] > xtarget->length || index[1] < 0) { return(RH4N_RET_VAR_BAD_INDEX); }
    ytarget = &((RH4nVarObj*)xtarget->value)[index[1]];
    if(index[2] == -1) {
        *paentry = ytarget;
        return(RH4N_RET_OK);
    }

    if(index[2] > ytarget->length || index[2] < 0) { return(RH4N_RET_VAR_BAD_INDEX); }
    *paentry = &((RH4nVarObj*)ytarget->value)[index[2]];
    return(RH4N_RET_OK);
}

int rh4nvarGetArrayLength(RH4nVarObj *target, int length[3]) {
    if(target->type != RH4NVARTYPEARRAY) { return(RH4N_RET_FORMAT_ERR); }

    RH4nVarObj *entry = NULL;

    length[0] = length[1] = length[2] = -1;
    
    length[0] = target->length;
    if(target->length == 0 || ((RH4nVarObj*)target->value)[0].type != RH4NVARTYPEARRAY) { return(RH4N_RET_OK); }
   
    entry = &((RH4nVarObj*)target->value)[0];
    length[1] = entry->length;
    if(entry->length == 0 || ((RH4nVarObj*)entry->value)[0].type != RH4NVARTYPEARRAY) { return(RH4N_RET_OK); }
    
    entry = &((RH4nVarObj*)entry->value)[0];
    length[2] = entry->length;
    return(RH4N_RET_OK);
}

int rh4nvarGetArrayDimension(RH4nVarObj *target, int *dimensions) {
    int length[3] = { -1, -1, -1 }, varlibret = 0;

    if((varlibret = rh4nvarGetArrayLength(target, length)) != RH4N_RET_OK) { return(varlibret); }

    if(length[0] != -1 && length[1] != -1 && length[2] != -1) { *dimensions = 3; }
    else if(length[0] != -1 && length[1] != -1) { *dimensions = 2; }
    else if(length[0] != -1) { *dimensions = 1; }

    return(RH4N_RET_OK);
}

int rh4nvarGetArrayVarType(RH4nVarList *varlist, char *pgroupname, char *pname, int *vartype) {
    RH4nVarRef _refvar = RH4NVAR_REF_INIT;
    RH4nVarObj *arrayentry = NULL;
    int rc = 0, dimensions = 0, index[3] = { -1, -1, -1 };

    if((rc = rh4nvarGetRef(varlist, pgroupname, pname, &_refvar)) != RH4N_RET_OK) return(rc);
    if((rc = rh4nvarGetArrayDimension(&_refvar.var->var, &dimensions)) != RH4N_RET_OK) return(rc);

    switch(dimensions) {
        case 3:
            index[2] = 0;
        case 2:
            index[1] = 0;
        case 1:
            index[0] = 0;
            break;
    }

    if((rc = rh4nvarGetArrayEntry(&_refvar.var->var, index, &arrayentry)) != RH4N_RET_OK) return(rc);
    *vartype = arrayentry->type;
    return(RH4N_RET_OK);
}
