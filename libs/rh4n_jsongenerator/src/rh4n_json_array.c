#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

#include "rh4n.h"
#include "rh4n_json.h"


bool rh4njsonCheckIfJSONArray(RH4nVarEntry_t *target, RH4nProperties *props) {
    RH4nVarEntry_t *hptr = NULL;

    for(hptr = target; hptr != NULL; hptr = hptr->next) {
        rh4n_log_debug(props->logging, "Checking [%s]", hptr->name);
        if(hptr->var.type == RH4NVARTYPEGROUP) {
            if(rh4njsonCheckIfJSONArray(hptr->nextlvl, props) == false) {
                return(false);
            }
        } else if(hptr->var.type != RH4NVARTYPEARRAY) {
            rh4n_log_debug(props->logging, "JSON Array Check = false. [%s] is not an array", hptr->name);
            return(false);
        }
    }
    return(true);
}

void rh4njsonPrintJSONArray(RH4nVarObj *target, FILE *outputfile, RH4nProperties *props) {
    RH4nVarObj *hptr = NULL;
    int i = 0;
   
    fprintf(outputfile, "[") ;
    for(; i < target->length; i++) {
        hptr = &((RH4nVarObj*)target->value)[i];
        if(hptr->type == RH4NVARTYPEARRAY) {
            rh4njsonPrintJSONArray(hptr, outputfile, props);
        } else {
            rh4nvarPrintVar(hptr, props, outputfile); 
        }
        if(i+1 < target->length) { fprintf(outputfile, ","); }
    }
    fprintf(outputfile, "]") ;
}

void rh4njsonPrintJSONObjectArray(RH4nVarEntry_t *target, FILE *outputfile, RH4nProperties *props) {
    int commonDim = 0, dimOK[3] = {-1, -1, -1};
    RH4nJSONObjectArrayParms_t args = {
            -1, 
            {-1, -1, -1},
            {-1, -1, -1}
        };

    rh4njsonpregetCommonDimension(target, args.length, dimOK, props);
    args.aimdim = rh4njsongetCommonDimension(dimOK);

    rh4n_log_develop(props->logging, "Found the common dimension %d", args.aimdim);
    rh4n_log_develop(props->logging, "Length of all arrays: %d, %d, %d", args.length[0], args.length[1], args.length[2]);
    rh4n_log_develop(props->logging, "Start generating array with objects");
    fprintf(outputfile, "[");
    rh4njsonPrintObjectArrayEntry(target, outputfile, 1, args, props);
    fprintf(outputfile, "]");
}


int rh4njsonPrintObjectArrayEntry(RH4nVarEntry_t *target, FILE *outputfile, int curdim, RH4nJSONObjectArrayParms_t args, RH4nProperties *props) {
    RH4nVarEntry_t * hptr;
    RH4nVarObj *arrentry = NULL;
    int i = 0, varlibret = 0, arraydims = 0, loopgoal = 0;

    if(args.length[curdim-1] == -2) {
        loopgoal = 1;
    } else {
        loopgoal = args.length[curdim-1];
    }

    for(; i < loopgoal; i++) {
        if(args.length[curdim-1] != -2) {
            args.index[curdim-1] = i;
        }

        if(curdim < args.aimdim) {
            fprintf(outputfile, "[");
            rh4njsonPrintObjectArrayEntry(hptr, outputfile, curdim+1, args, props);
            fprintf(outputfile, "]");
            if(i+1 < args.length[curdim-1]) { fprintf(outputfile, ","); }
            continue;
        }
        fprintf(outputfile, "{");
        for(hptr = target; hptr != NULL; hptr = hptr->next) {
            fprintf(outputfile, "\"%s\":", hptr->name);
            if(hptr->var.type == RH4NVARTYPEGROUP) {
                args.length[curdim] = -2;
                rh4njsonPrintObjectArrayEntry(hptr->nextlvl, outputfile, curdim+1, args, props);
                if(hptr->next != NULL) { fprintf(outputfile, ","); }
                continue;
            } 
            
            rh4nvarGetArrayDimension(&hptr->var, &arraydims);

            rh4n_log_develop(props->logging, "Printing %s with index X: %d Y: %d Z: %d", hptr->name, args.index[0], args.index[1], args.index[2]);
            if((varlibret = rh4nvarGetArrayEntry(&hptr->var, args.index, &arrentry)) != RH4N_RET_OK) {
                rh4n_log_error(props->logging, "Could not get entry (%d, %d, %d) from %s. Varlib return: %d", 
                    args.index[0], args.index[1], args.index[2], hptr->name, varlibret);
                return(varlibret);
            }
            switch(arrentry->type) {
                case RH4NVARTYPEARRAY:
                    rh4njsonPrintJSONArray(arrentry, outputfile, props);
                    break;
                default:
                    rh4nvarPrintVar(arrentry, props, outputfile);
            }
            if(hptr->next != NULL) { fprintf(outputfile, ","); }

        }
        fprintf(outputfile, "}");
        if(i+1 < args.length[curdim-1]) { fprintf(outputfile, ","); }
    }

}

int rh4njsongetCommonDimension(int dimOK[3]) {
    int i = 0;
    for(; i < 3; i++) {
        if(dimOK[i] != 1) { break; }
    }

    return(i);
}

void rh4njsonpregetCommonDimension(RH4nVarEntry_t *target, int plength[3], int pdimOK[3], RH4nProperties *props) {
    int curlength[3] = { -1, -1, -1},
        mylength[3] = {-1, -1, -1 },
        childlength[3] = {-1, -1, -1 };

    int dimOK[3] = { -1, -1, -1 },
        childdimOK[3] = {-1, -1, -1 };
    RH4nVarEntry_t *hptr = NULL;

    for(hptr = target; hptr != NULL; hptr = hptr->next) {
        if(hptr->var.type == RH4NVARTYPEGROUP) { 
            rh4njsonpregetCommonDimension(hptr->nextlvl, childlength, childdimOK, props);
            rh4njsonMergeCommonDimResults(mylength, dimOK, childlength, childdimOK);
            continue; 
        }

        rh4nvarGetArrayLength(&hptr->var, curlength);
        rh4njsoncheckCommonDimension(curlength, mylength, dimOK);
    }
    
    rh4n_log_develop(props->logging, "Dims check: [%d] [%d] [%d]", dimOK[0], dimOK[1], dimOK[2]);

    if(plength != NULL) { memcpy(plength, mylength, sizeof(plength)); }
    if(pdimOK != NULL) { memcpy(pdimOK, dimOK, sizeof(pdimOK)); }
}

void rh4njsonMergeCommonDimResults(int length1[3], int dimOK1[3], int length2[3], int dimOK2[3]) {
    int i = 0;
    for(; i < 3; i++) {
        if((dimOK1[i] == -1 && dimOK2[i] != -1) && dimOK1[i] != dimOK2[i]) {
            dimOK1[i] = dimOK2[i];
        } else if(dimOK1[i] != dimOK2[i]) {
            dimOK1[i] = 0;
        } else if((dimOK1[i] != -1 && dimOK2[i] != -1) && dimOK1[i] == dimOK2[i]) {
            dimOK1[i] = length1[i] == length2[i] ? 1 : 0;
        } 
    }
}

void rh4njsoncheckCommonDimension(int curlength[3], int length[3], int dimOK[3]) {
    int i = 0;

    for(; i < 3; i++) {
        if(length[i] == -1) {
            length[i] = curlength[i];
            continue;
        }
        if(curlength[i] != length[i]) {
            dimOK[i] = 0;
        } else {
            if(dimOK[i] == -1) {
                dimOK[i] = 1;
            }
        }
    }
}
