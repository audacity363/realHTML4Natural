#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rh4n.h"

#include <jni.h>
#include "json/jsonhandling.h"
#include "json/json_utils.h"
#include "json/handlers.h"

int handleIntEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    jclass integerclass = NULL;
    jmethodID getintID = NULL;

    int value = 0, rc = 0;

    if((integerclass = (*env)->FindClass(env, "java/lang/Integer")) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not get Integer class");
        return(-1);
    }

    if((getintID = (*env)->GetMethodID(env, integerclass, "intValue", "()I")) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not find intValue() method");
        return(-2);
    }

    value = (*env)->CallIntMethod(env, target, getintID);


    switch(args.vartype) {
        case JVAR_INT:
            if((rc = rh4nvarCreateNewInt(args.var_anker, (char*)args.parent, (char*)args.varname, value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create integer [%s].[%s]. Varlib return: %d", args.parent, 
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_INT1D:
        case JVAR_INT2D:
        case JVAR_INT3D:
            if((rc = rh4nvarSetIntArrayEntry(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    index, value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, 
                    "Could not set set Integer array entry [%s].[%s] X: %d Y: %d Z: %d. Varlib return: %d", args.parent, 
                    args.varname, index[0], index[1], index[2], index[3], rc);
                return(rc);
            }
            break;
    }
    return(RH4N_RET_OK);
}

int handleFloatEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    jclass floatclass = NULL;
    jmethodID doubleValueID = NULL;
    int rc = 0;

    double value = 0;

    if((floatclass = (*env)->FindClass(env, "java/lang/Float")) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not get Float class");
        return(-1);
    }

    if((doubleValueID = (*env)->GetMethodID(env, floatclass, "doubleValue", "()D")) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not find doubleValue() method");
        return(-2);
    }

    value = (*env)->CallDoubleMethod(env, target, doubleValueID);
   
    switch(args.vartype) {
        case JVAR_FLOAT:
            if((rc = rh4nvarCreateNewFloat(args.var_anker, (char*)args.parent, (char*)args.varname, value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Float [%s].[%s]. Varlib return: %d", args.parent, 
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_FLOAT1D:
        case JVAR_FLOAT2D:
        case JVAR_FLOAT3D:
            if((rc = rh4nvarSetFloatArrayEntry(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    index, value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, 
                    "Could not set set Integer array entry [%s].[%s] X: %d Y: %d Z: %d. Varlib return: %d", args.parent, 
                    args.varname, index[0], index[1], index[2], index[3], rc);
                return(rc);
            }
    }

    return(RH4N_RET_OK);
}

int handleBooleanEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    jclass booleanclass = NULL;
    jmethodID booleanValueID = NULL;
    int rc = 0;

    jboolean value = 0;

    if((booleanclass = (*env)->FindClass(env, "java/lang/Boolean")) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not get Boolean class");
        return(-1);
    }

    if((booleanValueID = (*env)->GetMethodID(env, booleanclass, "booleanValue", "()Z")) == NULL) {
        rh4n_log_error(args.infos->logging, "Could not find booleanValue() method");
        return(-2);
    }

    value = (*env)->CallBooleanMethod(env, target, booleanValueID);

    switch(args.vartype) {
        case JVAR_BOOLEAN:
            if((rc = rh4nvarCreateNewBool(args.var_anker, (char*)args.parent, (char*)args.varname, value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create Bool [%s].[%s]. Varlib return: %d", args.parent, 
                    args.varname, rc);
                return(rc);
            }
            break;
        case JVAR_BOOLEAN1D:
        case JVAR_BOOLEAN2D:
        case JVAR_BOOLEAN3D:
            if((rc = rh4nvarSetBoolArrayEntry(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    index, value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, 
                    "Could not set set Bool array entry [%s].[%s] X: %d Y: %d Z: %d. Varlib return: %d", args.parent, 
                    args.varname, index[0], index[1], index[2], index[3], rc);
                return(rc);
            }
    }
    return(RH4N_RET_OK);
}

int handleStringEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    const char *value = NULL;
    int rc = 0;

    if((value = (*env)->GetStringUTFChars(env, (jstring)target, NULL)) == NULL) {
        rh4n_log_error(args.infos->logging, "String value is == NULL\n");
        return(-1);
    }

    switch(args.vartype) {
        case JVAR_STRING:
            if((rc = rh4nvarCreateNewString(args.var_anker, (char*)args.parent, (char*)args.varname, (char*)value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, "Could not create String [%s].[%s]. Varlib return: %d", args.parent, 
                    args.varname, rc);
                (*env)->ReleaseStringUTFChars(env, (jstring)target, value);
                return(rc);
            }
            break;
        case JVAR_STRING1D:
        case JVAR_STRING2D:
        case JVAR_STRING3D:
            if((rc = rh4nvarSetStringArrayEntry(args.var_anker, (char*)args.parent, (char*)args.varname, 
                    index, (char*)value)) != RH4N_RET_OK) {
                rh4n_log_error(args.infos->logging, 
                    "Could not set set String array entry [%s].[%s] X: %d Y: %d Z: %d. Varlib return: %d", args.parent, 
                    args.varname, index[0], index[1], index[2], index[3], rc);
                (*env)->ReleaseStringUTFChars(env, (jstring)target, value);
                return(rc);
            }
            
    }

    (*env)->ReleaseStringUTFChars(env, (jstring)target, value);
    return(RH4N_RET_OK);
}
