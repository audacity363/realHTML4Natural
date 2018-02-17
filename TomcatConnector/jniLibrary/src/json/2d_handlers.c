#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "standard.h"

#include "vars.h"

#include <jni.h>
#include "json/jsonhandling.h"
#include "json/json_utils.h"
#include "json/handlers.h"

int Handler2D(JNIEnv *env, HandlerArgs args) {
    int x = 0, y = 0, max_strlen = 0, index[3] = {0, 0, 0};
    jint x_length = 0, y_length = 0;

    jobject arraylist_x = NULL, arraylist_y = NULL, entry = NULL;
    LLClassInfo *llclass = NULL;

    if((llclass = getLLClassInfos(env, args.infos->logging)) == NULL) {
        return(-1);
    }

    if((arraylist_x = (*env)->GetObjectField(env, args.curptr, args.infos->value)) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not get field value");
        return(-2);
    }
    
    x_length = (*env)->CallIntMethod(env, arraylist_x, llclass->sizeID);
    rh4n_log_debug(args.infos->logging, "X Length: [%d]", x_length);

    arraylist_y = (*env)->CallObjectMethod(env, arraylist_x, llclass->getID, 0);
    y_length = (*env)->CallIntMethod(env, arraylist_y, llclass->sizeID);
    rh4n_log_debug(args.infos->logging, "Y Length: [%d]", y_length);

    switch(args.vartype) {
        case JVAR_STRING2D:
            rh4n_log_debug(args.infos->logging, "Creating new String array [%s] under [%s]", args.varname, args.parent);
            max_strlen = getMaxStrLen2DString(env, args, arraylist_x, llclass, x_length, y_length);
            add2DStringArray(args.var_anker, (char*)args.parent, (char*)args.varname, max_strlen+1, x_length, y_length);
            break;
        case JVAR_INT2D:
            add2DIntegerArray(args.var_anker, (char*)args.parent, (char*)args.varname, x_length, y_length);
            break;
        case JVAR_BOOLEAN2D:
            add2DBooleanArray(args.var_anker, (char*)args.parent, (char*)args.varname, x_length, y_length);
            break;
        case JVAR_FLOAT2D:
            add2DFloatArray(args.var_anker, (char*)args.parent, (char*)args.varname, x_length, y_length);
            break;
    }

    for(; x < x_length; x++) {
        index[0] = x;
        if((arraylist_y = (*env)->CallObjectMethod(env, arraylist_x, llclass->getID, x)) == NULL) {
            rh4n_log_error(args.infos->logging, "Index x: [%d] of [%s] is null", x, args.varname);
            return(-3);
        }
        for(y=0; y < y_length; y++) {
            index[1] = y;
            if((entry = (*env)->CallObjectMethod(env, arraylist_y, llclass->getID, y)) == NULL) {
                rh4n_log_error(args.infos->logging, "Index x: [%d] y: [%d] of [%s] is null", x, y, args.varname);
                return(-3);
            }
            switch(args.vartype) {
                case JVAR_INT2D:
                    handleIntEntry(env, args, entry, index);
                    break;
                case JVAR_FLOAT2D:
                    handleFloatEntry(env, args, entry, index);
                    break;
                case JVAR_BOOLEAN2D:
                    handleBooleanEntry(env, args, entry, index);
                    break;
                case JVAR_STRING2D:
                    handleStringEntry(env, args, entry, index);
                    break;
            }

        }
    }
}

int getMaxStrLen2DString(JNIEnv *env, HandlerArgs args, jobject target, LLClassInfo *llinfos, int x_length, int y_length) {
    jobject entry = NULL, y_arraylist = NULL;
    int i = 0, y = 0, length = 0;
    jsize jlength = 0;

    for(;i < x_length; i++) {
        if((y_arraylist = (*env)->CallObjectMethod(env, target, llinfos->getID, i)) == NULL) {
            rh4n_log_error(args.infos->logging, "Element in array is == NULL");
            return(-3);
        }

        for(y = 0; y < y_length; y++) {
            if((entry = (*env)->CallObjectMethod(env, y_arraylist, llinfos->getID, y)) == NULL) {
                rh4n_log_error(args.infos->logging, "Element in array is == NULL");
                return(-3);
            }
            jlength = (*env)->GetStringLength(env, (jstring)entry);
            if(jlength > length)
                length = jlength;
        }
    }

    return(length);
}
