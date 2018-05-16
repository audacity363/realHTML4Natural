#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rh4n.h"

#include <jni.h>
#include "json/jsonhandling.h"
#include "json/json_utils.h"
#include "json/handlers.h"

int Handler2D(JNIEnv *env, HandlerArgs args) {
    int x = 0, y = 0, index[3] = {0, 0, 0}, arrlength[3] = {0, 0, 0}, rc = 0;

    jobject arraylist_x = NULL, arraylist_y = NULL, entry = NULL;
    LLClassInfo *llclass = NULL;

    if((llclass = getLLClassInfos(env, args.infos->logging)) == NULL) {
        return(-1);
    }

    if((arraylist_x = (*env)->GetObjectField(env, args.curptr, args.infos->value)) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not get field value");
        return(-2);
    }
    
    arrlength[0] = (*env)->CallIntMethod(env, arraylist_x, llclass->sizeID);

    arraylist_y = (*env)->CallObjectMethod(env, arraylist_x, llclass->getID, 0);
    arrlength[1] = (*env)->CallIntMethod(env, arraylist_y, llclass->sizeID);

    rh4n_log_debug(args.infos->logging, "X len: [%d] Y len: [%d]", arrlength[0], arrlength[1]);
    rh4n_log_debug(args.infos->logging, "Creating new 2D array [%s] under [%s]", args.varname, args.parent);

    switch(args.vartype) {
        case JVAR_STRING2D:
            if((rc = rh4nvarCreateNewStringArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    2, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create String array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_INT2D:
            if((rc = rh4nvarCreateNewIntArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    2, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Integer array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_BOOLEAN2D:
            if((rc = rh4nvarCreateNewBoolArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    2, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Bool array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_FLOAT2D:
            if((rc = rh4nvarCreateNewFloatArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    2, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Float array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
    }

    for(; x < arrlength[0]; x++) {
        index[1] = index[2] = -1;
        index[0] = x;
        if((arraylist_y = (*env)->CallObjectMethod(env, arraylist_x, llclass->getID, x)) == NULL) {
            rh4n_log_error(args.infos->logging, "Index x: [%d] of [%s] is null", x, args.varname);
            return(-3);
        }
        for(y=0; y < arrlength[1]; y++) {
            index[2] = -1;
            index[1] = y;

            if((entry = (*env)->CallObjectMethod(env, arraylist_y, llclass->getID, y)) == NULL) {
                rh4n_log_error(args.infos->logging, "Index x: [%d] y: [%d] of [%s] is null", x, y, args.varname);
                return(-3);
            }
            switch(args.vartype) {
                case JVAR_INT2D:
                    if((rc = handleIntEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                    break;
                case JVAR_FLOAT2D:
                    if((rc = handleFloatEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                    break;
                case JVAR_BOOLEAN2D:
                    if((rc = handleBooleanEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                    break;
                case JVAR_STRING2D:
                    if((rc = handleStringEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                    break;
            }

        }
    }
    return(RH4N_RET_OK);
}
