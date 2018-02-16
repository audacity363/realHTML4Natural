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


int Handler1D(JNIEnv *env, HandlerArgs args) {
    int x = 0, i = 0, max_strlen = 0, index[3] = {0, 0, 0};
    jobject arraylist = NULL, entry = NULL;
    jint x_length = 0;

    LLClassInfo *llclass = NULL;

    if((arraylist = (*env)->GetObjectField(env, args.curptr, args.infos->value)) == NULL) {
        printf("Couldn't get field value\n");
        return(-1);
    }

    if((llclass = getLLClassInfos(env)) == NULL) {
        return(-2);
    }

    x_length = (*env)->CallIntMethod(env, arraylist, llclass->sizeID);
    //printTabs(args.level);
    //printf("X_length: [%d]\n", x_length);

    switch(args.vartype) {
        case JVAR_STRING1D:
            //printf("Creating new String array [%s] under [%s]\n", args.varname, args.parent);
            max_strlen = getMaxStrLen1DString(env, args, arraylist, llclass, x_length);
            add1DStringArray(args.var_anker, (char*)args.parent, (char*)args.varname, max_strlen+1, x_length);
            break;
        case JVAR_INT1D:
            add1DIntegerArray(args.var_anker, (char*)args.parent, (char*)args.varname, x_length);
            break;
        case JVAR_BOOLEAN1D:
            add1DBooleanArray(args.var_anker, (char*)args.parent, (char*)args.varname, x_length);
            break;
        case JVAR_FLOAT1D:
            add1DFloatArray(args.var_anker, (char*)args.parent, (char*)args.varname, x_length);
            break;
    }

    //printTabs(args.level);
    for(;i < x_length; i++) {
        if((entry = (*env)->CallObjectMethod(env, arraylist, llclass->getID, i)) == NULL) {
            printf("Element in array is == NULL");
            return(-3);
        }
        index[0] = i;
        switch(args.vartype) {
            case JVAR_INT1D:
                handleIntEntry(env, args, entry, index);
                break;
            case JVAR_FLOAT1D:
                handleFloatEntry(env, args, entry, index);
                break;
            case JVAR_BOOLEAN1D:
                handleBooleanEntry(env, args, entry, index);
                break;
            case JVAR_STRING1D:
                handleStringEntry(env, args, entry, index);
                break;
        }
    }

    return(0);
}

int getMaxStrLen1DString(JNIEnv *env, HandlerArgs args, jobject target, LLClassInfo *llinfos, int x_length) {
    jobject entry = NULL;
    int i = 0, length = 0;
    jsize jlength = 0;

    for(;i < x_length; i++) {
        if((entry = (*env)->CallObjectMethod(env, target, llinfos->getID, i)) == NULL) {
            printf("Element in array is == NULL");
            return(-3);
        }
        jlength = (*env)->GetStringLength(env, (jstring)entry);
        if(jlength > length)
            length = jlength;
    }

    return(length);
}
