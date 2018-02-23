#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "standard.h"

void rh4nvarPrintList(RH4nVarList *varlist) {
    if(!varlist) { return; }
    
    rh4nvarPrintFork(varlist->anker, -1, 1, NULL, stdout);
}

int rh4nvarPrintListToFile(RH4nVarList *varlist, char *filename, RH4nProperties *props) {
    FILE *outputfile = NULL;
    
    if((outputfile = fopen(filename, "w")) == NULL) {
        return(RH4N_RET_INTERNAL_ERR);
    }

    rh4nvarPrintFork(varlist->anker, -1, 1, props, outputfile);

    fclose(outputfile);
}

void rh4nvarPrintFork(RH4nVarEntry_t *forkanker, int mode, int level, RH4nProperties *props, FILE *outputfile) {
    if(!forkanker) { return; }

    RH4nVarEntry_t *hptr = forkanker;
    int i = 1;

    if(checkArrayGroup(forkanker)) {
        rh4nvarPrintGroupArray(forkanker, mode, level, props, outputfile); 
        return;
    }

    if(level == 1) fprintf(outputfile, "{");
    for(; hptr != NULL; hptr = hptr->next) {
        fprintf(outputfile, "\"%s\":", hptr->name);
        if(hptr->var.type == RH4NVARTYPEARRAY) { rh4nvarPrintArray(&hptr->var, mode, level, props, outputfile); }
        else { rh4nvarPrintVar(&hptr->var, props, outputfile); }
        if(hptr->var.type == RH4NVARTYPEGROUP) { 
            if(checkArrayGroup(hptr->nextlvl)) {
                rh4nvarPrintGroupArray(hptr->nextlvl, mode, level, props, outputfile); 
            }
            else {
                fprintf(outputfile, "{");
                rh4nvarPrintFork(hptr->nextlvl, mode, level+1, props, outputfile); 
                fprintf(outputfile, "}");
            }
        }

        if(hptr->next) fprintf(outputfile, ",");
    }
    if(level == 1) fprintf(outputfile, "}");
}

void rh4nvarPrintArray(RH4nVarObj *variable, int mode, int level,RH4nProperties *props, FILE *outputfile) {
    int x = 0, y = 0, z = 0, i = 0, index[3] = {0, 0, 0};
    RH4nVarObj *xtarget = NULL, *ytarget = NULL, *ztarget = NULL;
    
    rh4nvarPrintArrayDim(variable, mode, level, props, outputfile);
}

void rh4nvarPrintArrayDim(RH4nVarObj *variable, int mode, int level, RH4nProperties *props, FILE *outputfile) {
    int i = 0;
    RH4nVarObj *target = NULL;

    fprintf(outputfile, "[");

    for(; i < variable->length; i++) {
        target = &((RH4nVarObj*)variable->value)[i];
        if(target->type == RH4NVARTYPEARRAY) {
            rh4nvarPrintArrayDim(target, mode, level+1, props, outputfile);
        } else {
            rh4nvarPrintVar(target, props, outputfile);
        }
        if(i+1 < variable->length) {
            fprintf(outputfile, ",");
        }
    }

    fprintf(outputfile, "]");

}

void rh4nvarPrintGroupArray(RH4nVarEntry_t *variable, int mode, int level, RH4nProperties *props, FILE *outputfile) {
    int dimension = -1, length[3] = { -1, -1, -1 }, varlibret = 0, index[3] = { 0, 0, 0 };
    RH4nVarEntry_t *hptr = NULL;
    RH4nVarObj *target = NULL;

    if((varlibret = rh4nvarGetArrayDimension(&variable->var, &dimension)) != RH4N_RET_OK) { return; }
    if((varlibret = rh4nvarGetArrayLength(&variable->var, length)) != RH4N_RET_OK) { return; }

    switch(dimension) {
        case(1):
            rh4nvarPrintGroup1DArray(variable, mode, level, props, length, outputfile);
            break;
        case(2):
            rh4nvarPrintGroup2DArray(variable, mode, level, props, length, outputfile);
            break;
        case(3):
            rh4nvarPrintGroup3DArray(variable, mode, level, props, length, outputfile);
            break;
    }
}

void rh4nvarPrintGroup1DArray(RH4nVarEntry_t *variable, int mode, int level, RH4nProperties *props, int length[3], FILE *outputfile) {
    int index[3] = { 0, 0, 0}, varlibret = 0;
    RH4nVarEntry_t *hptr = NULL;
    RH4nVarObj *target = NULL;

    index[1] = index[2] = -1;
    fprintf(outputfile, "[");
    for(; index[0] < length[0]; index[0]++) {
        fprintf(outputfile, "{");
        for(hptr = variable; hptr != NULL; hptr = hptr->next) {
            if((varlibret = rh4nvarGetArrayEntry(&hptr->var, index, &target)) != RH4N_RET_OK) { return; }
            fprintf(outputfile, "\"%s\":", hptr->name);
            rh4nvarPrintVar(target, props, outputfile);
            if(hptr->next) fprintf(outputfile, ",");
        }
        fprintf(outputfile, "}");
        if(index[0]+1 < length[0]) fprintf(outputfile, ",");
    }
    fprintf(outputfile, "]");
}

void rh4nvarPrintGroup2DArray(RH4nVarEntry_t *variable, int mode, int level, RH4nProperties *props, int length[3], FILE *outputfile) {
    int index[3] = { 0, 0, 0}, varlibret = 0;
    RH4nVarEntry_t *hptr = NULL;
    RH4nVarObj *target = NULL;

    index[2] = -1;
    fprintf(outputfile, "[");
    for(; index[0] < length[0]; index[0]++) {
        fprintf(outputfile, "[");
        for(index[1] = 0; index[1] < length[1]; index[1]++) {
            fprintf(outputfile, "{");
            for(hptr = variable; hptr != NULL; hptr = hptr->next) {
                if((varlibret = rh4nvarGetArrayEntry(&hptr->var, index, &target)) != RH4N_RET_OK) { return; }
                fprintf(outputfile, "\"%s\":", hptr->name);
                rh4nvarPrintVar(target, props, outputfile);
                if(hptr->next) fprintf(outputfile, ",");
            }
            fprintf(outputfile, "}");
            if(index[1]+1 < length[1]) fprintf(outputfile, ",");
        }
        fprintf(outputfile, "]");
        if(index[0]+1 < length[0]) fprintf(outputfile, ",");
    }
    fprintf(outputfile, "]");
}

void rh4nvarPrintGroup3DArray(RH4nVarEntry_t *variable, int mode, int level, RH4nProperties *props, int length[3], FILE *outputfile) {
    int index[3] = { 0, 0, 0}, varlibret = 0;
    RH4nVarEntry_t *hptr = NULL;
    RH4nVarObj *target = NULL;

    fprintf(outputfile, "[");
    for(; index[0] < length[0]; index[0]++) {
        fprintf(outputfile, "[");
        for(index[1] = 0; index[1] < length[1]; index[1]++) {
            fprintf(outputfile, "[");
            for(index[2] = 0; index[2] < length[2]; index[2]++) {
                fprintf(outputfile, "{");
                for(hptr = variable; hptr != NULL; hptr = hptr->next) {
                    if((varlibret = rh4nvarGetArrayEntry(&hptr->var, index, &target)) != RH4N_RET_OK) { return; }
                    fprintf(outputfile, "\"%s\":", hptr->name);
                    rh4nvarPrintVar(target, props, outputfile);
                    if(hptr->next) fprintf(outputfile, ",");
                }
                fprintf(outputfile, "}");
                if(index[2]+1 < length[2]) fprintf(outputfile, ",");
            }
            fprintf(outputfile, "]");
            if(index[1]+1 < length[1]) fprintf(outputfile, ",");
        }
        fprintf(outputfile, "]");
        if(index[0]+1 < length[0]) fprintf(outputfile, ",");
    }
    fprintf(outputfile, "]");
}

void rh4nvarPrintVar(RH4nVarObj *variable, RH4nProperties *props, FILE *outputfile) {
    RH4nVarPrint printTable[] = {
        {RH4NVARTYPESTRING, rh4nvarPrintString},
        {RH4NVARTYPEUSTRING, rh4nvarPrintUString},
        {RH4NVARTYPEBOOLEAN, rh4nvarPrintBool},
        {RH4NVARTYPEINTEGER, rh4nvarPrintInt},
        {RH4NVARTYPEFLOAT, rh4nvarPrintFloat}
    };

    int i = 0;
    for(; i < sizeof(printTable)/sizeof(RH4nVarPrint); i++) {
        if(variable->type == printTable[i].vartype) { printTable[i].printfunction(variable, props, outputfile); }
    }
}

bool checkArrayGroup(RH4nVarEntry_t *variable) {
    RH4nVarEntry_t *hptr = NULL;
    int length[3] = { -1, -1, -1}, curlength[3] = { -1, -1, -1}, dimension = -1, curdimension = -1,
        varlibret = 0;

    for(hptr = variable; hptr != NULL; hptr = hptr->next) {
        if(hptr->var.type != RH4NVARTYPEARRAY) { return(false); }

        if((varlibret = rh4nvarGetArrayDimension(&hptr->var, &curdimension)) != RH4N_RET_OK) { return(false); }
        if(dimension == -1) { dimension = curdimension; }
        else if(dimension != curdimension) { return(false); }

        
        if((varlibret = rh4nvarGetArrayDimension(&hptr->var, curlength)) != RH4N_RET_OK) { return(false); }
        if(length[0] == -1 && length[1] == -1 && length[2] == -1) { memcpy(length, curlength, sizeof(length)); }
        else if(memcmp(length, curlength, sizeof(length)) != 0) { return(false); }
    }

    return(true);
}

void rh4nvarPrintString(RH4nVarObj *variable, RH4nProperties *props, FILE *outputfile) {
    fprintf(outputfile, "\"%s\"", (char*)variable->value);
}

void rh4nvarPrintUString(RH4nVarObj *variable, RH4nProperties *props, FILE *outputfile) {
    fprintf(outputfile, "\"%S\"", (wchar_t*)variable->value);
}

void rh4nvarPrintBool(RH4nVarObj *variable, RH4nProperties *props, FILE *outputfile) {
    fprintf(outputfile, "%s", ((bool)variable->value) == 1 ? "true" : "false");
}

void rh4nvarPrintInt(RH4nVarObj *variable, RH4nProperties *props, FILE *outputfile) {
    fprintf(outputfile, "%d", (int)variable->value);
}

void rh4nvarPrintFloat(RH4nVarObj *variable, RH4nProperties *props, FILE *outputfile) {
    fprintf(outputfile, "%f", (double)variable->value);
}

void rh4nvarPrintTabs(int level, RH4nProperties *props) {
    int i = 0;
    for(; i < level; i++) { printf("\t"); }
}
