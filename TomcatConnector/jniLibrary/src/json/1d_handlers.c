#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rh4n.h"

#include <jni.h>
#include "json/jsonhandling.h"
#include "json/json_utils.h"
#include "json/handlers.h"


int Handler1D(JNIEnv *env, HandlerArgs args) {
    int x = 0, i = 0, max_strlen = 0, index[3] = {0, 0, 0}, arrlength[3] = { 0, 0, 0 }, rc = 0;
    jobject arraylist = NULL, entry = NULL;

    LLClassInfo *llclass = NULL;

    if((arraylist = (*env)->GetObjectField(env, args.curptr, args.infos->value)) == NULL) {
        rh4n_log_error(args.infos->logging, "Couldn't get field value");
        return(-1);
    }

    if((llclass = getLLClassInfos(env, args.infos->logging)) == NULL) {
        return(-2);
    }

    arrlength[0] = (*env)->CallIntMethod(env, arraylist, llclass->sizeID);

    rh4n_log_debug(args.infos->logging, "X_length: [%d]", arrlength[0]);
    rh4n_log_debug(args.infos->logging, "Creating new array [%s] under [%s]", args.varname, args.parent);

    switch(args.vartype) {
        case JVAR_STRING1D:
            if((rc = rh4nvarCreateNewStringArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    1, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create String array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_INT1D:
            if((rc = rh4nvarCreateNewIntArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    1, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Integer array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_BOOLEAN1D:
            if((rc = rh4nvarCreateNewBoolArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                1, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Bool array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_FLOAT1D:
            if((rc = rh4nvarCreateNewFloatArray(args.var_anker, (char*)args.parent, (char*)args.varname, 
                1, arrlength)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Float array [%s].[%s]. Varlib rc: %d", args.parent,
                    args.varname, rc);
                return(rc);
            }
            break;
    }

    for(;i < arrlength[0]; i++) {
        if((entry = (*env)->CallObjectMethod(env, arraylist, llclass->getID, i)) == NULL) {
            rh4n_log_error(args.infos->logging, "Element [%d] in array is == NULL", i);
            return(-3);
        }
        index[1] = index[2] = -1;
        index[0] = i;
        switch(args.vartype) {
            case JVAR_INT1D:
                if((rc = handleIntEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                break;
            case JVAR_FLOAT1D:
                if((rc = handleFloatEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                break;
            case JVAR_BOOLEAN1D:
                if((rc = handleBooleanEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                break;
            case JVAR_STRING1D:
                if((rc = handleStringEntry(env, args, entry, index)) != RH4N_RET_OK) return(rc);
                break;
        }
    }

    return(RH4N_RET_OK);
}
