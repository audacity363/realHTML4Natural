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

int handleIntEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    jclass integerclass = NULL;
    jmethodID getintID = NULL;

    int value = 0;

    if((integerclass = (*env)->FindClass(env, "java/lang/Integer")) == NULL) {
        printf("Could not get Integer class\n");
        return(-1);
    }

    if((getintID = (*env)->GetMethodID(env, integerclass, "intValue", "()I")) == NULL) {
        printf("Could not find intValue() method\n");
        return(-2);
    }

    value = (*env)->CallIntMethod(env, target, getintID);

    switch(args.vartype) {
        case JVAR_INT:
            addInteger(args.var_anker, (char*)args.parent, (char*)args.varname, value);
            break;
        case JVAR_INT1D:
            edit1DIntegerArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0]);
            break;
        case JVAR_INT2D:
            edit2DIntegerArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0], index[1]);
            break;
        case JVAR_INT3D:
            edit3DIntegerArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0], index[1], index[2]);
            break;
    }
}

int handleFloatEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    jclass floatclass = NULL;
    jmethodID doubleValueID = NULL;

    double value = 0;

    if((floatclass = (*env)->FindClass(env, "java/lang/Float")) == NULL) {
        printf("Could not get Float class\n");
        return(-1);
    }

    if((doubleValueID = (*env)->GetMethodID(env, floatclass, "doubleValue", "()D")) == NULL) {
        printf("Could not find doubleValue() method\n");
        return(-2);
    }

    value = (*env)->CallDoubleMethod(env, target, doubleValueID);
   
    switch(args.vartype) {
        case JVAR_FLOAT:
            addFloat(args.var_anker, (char*)args.parent, (char*)args.varname, value);
            break;
        case JVAR_FLOAT1D:
            edit1DFloatArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0]);
            break;
        case JVAR_FLOAT2D:
            edit2DFloatArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0], index[1]);
            break;
        case JVAR_FLOAT3D:
            edit3DFloatArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0], index[1], index[2]);
            break;
    }
}

int handleBooleanEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    jclass booleanclass = NULL;
    jmethodID booleanValueID = NULL;

    jboolean value = 0;

    if((booleanclass = (*env)->FindClass(env, "java/lang/Boolean")) == NULL) {
        printf("Could not get Boolean class\n");
        return(-1);
    }

    if((booleanValueID = (*env)->GetMethodID(env, booleanclass, "booleanValue", "()Z")) == NULL) {
        printf("Could not find booleanValue() method\n");
        return(-2);
    }

    value = (*env)->CallBooleanMethod(env, target, booleanValueID);

    switch(args.vartype) {
        case JVAR_BOOLEAN:
            addBoolean(args.var_anker, (char*)args.parent, (char*)args.varname, value);
            break;
        case JVAR_BOOLEAN1D:
            edit1DBooleanArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0]);
            break;
        case JVAR_BOOLEAN2D:
            edit2DBooleanArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0], index[1]);
            break;
        case JVAR_BOOLEAN3D:
            edit3DBooleanArray(args.var_anker, (char*)args.parent, (char*)args.varname, value, index[0], index[1], index[2]);
            break;
    }
}

int handleStringEntry(JNIEnv *env, HandlerArgs args, jobject target, int index[3]) {
    const char *value = NULL;
    wchar_t *w_value = NULL;

    if((value = (*env)->GetStringUTFChars(env, (jstring)target, NULL)) == NULL) {
        printf("String value is == NULL\n");
        return(-1);
    }

    if((w_value = malloc(sizeof(wchar_t)*(strlen(value)+1))) == NULL) {
        printf("Malloc for wchar failed\n");
        return(-2);
    }

    swprintf(w_value, strlen(value)+1, L"%hs", value);

    switch(args.vartype) {
        case JVAR_STRING:
            addString(args.var_anker, (char*)args.parent, (char*)args.varname, w_value, strlen(value)+1);
            break;
        case JVAR_STRING1D:
            edit1DStringArray(args.var_anker, (char*)args.parent, (char*)args.varname, w_value, index[0]);
            break;
        case JVAR_STRING2D:
            edit2DStringArray(args.var_anker, (char*)args.parent, (char*)args.varname, w_value, index[0], index[1]);
            break;
        case JVAR_STRING3D:
            edit3DStringArray(args.var_anker, (char*)args.parent, (char*)args.varname, w_value, index[0], index[1], index[2]);
            break;
    }

    free(w_value);
    (*env)->ReleaseStringUTFChars(env, (jstring)target, value);
}
