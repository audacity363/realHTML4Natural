#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "standard.h"
#include "vars.h"

#include <jni.h>
#include "json/jsonhandling.h"
#include "json/json_utils.h"

GeneralInfos *getFieldIDs(JNIEnv *env, RH4nLogrule *logging) {
    GeneralInfos *infos;

    if((infos = malloc(sizeof(GeneralInfos))) == NULL) {
        rh4n_log_error(logging, "Could not allocate mem for GeneralInfos");
        return(NULL);
    }

    jclass nodeclass = NULL, typesclass = NULL;

    if((nodeclass = (*env)->FindClass(env, "realHTML/tomcat/JSONMatcher/LLNode")) == NULL) {
        rh4n_log_error(logging, "Could not find class LLNode");
        return(NULL);
    }

    if((infos->nextentry = (*env)->GetFieldID(env, nodeclass, "next", "LrealHTML/tomcat/JSONMatcher/LLNode;")) == NULL) {
        rh4n_log_error(logging, "Couldn't find field \"next\"");
        return(NULL);
    }

    if((infos->nextlvl = (*env)->GetFieldID(env, nodeclass, "nextlvl", "LrealHTML/tomcat/JSONMatcher/LLNode;")) == NULL) {
        rh4n_log_error(logging, "Couldn't find field \"nextlvl\"");
        return(NULL);
    }

    if((infos->type = (*env)->GetFieldID(env, nodeclass, "type", "LrealHTML/tomcat/JSONMatcher/Types;")) == NULL) {
        rh4n_log_error(logging, "Couldn't find field \"type\"");
        return(NULL);
    }

    if((infos->name = (*env)->GetFieldID(env, nodeclass, "name", "Ljava/lang/String;")) == NULL) {
        rh4n_log_error(logging, "Couldn't find field \"name\"");
        return(NULL);
    }

    if((infos->value = (*env)->GetFieldID(env, nodeclass, "value", "Ljava/lang/Object;")) == NULL) {
        rh4n_log_error(logging, "Couldn't find field \"value\"");
        return(NULL);
    }

    if((typesclass = (*env)->FindClass(env, "realHTML/tomcat/JSONMatcher/Types")) == NULL) {
        rh4n_log_error(logging, "Could not find class Types");
        return(NULL);
    }

    if((infos->type_method = (*env)->GetMethodID(env, typesclass, "getNumberRep", "()I")) == NULL) {
        rh4n_log_error(logging, "Could not find method \"getNumberRep\"");
        return(NULL);
    }

    infos->logging = logging;

    return(infos);
}

int getAnker(JNIEnv *env, jobject varlist, GeneralInfos *infos) {
    jclass llhandlerclass = NULL;
    jfieldID headID = NULL;

    if((llhandlerclass = (*env)->FindClass(env, "realHTML/tomcat/JSONMatcher/LLHandler")) == NULL) {
        rh4n_log_error(infos->logging, "Could find class LLHandler");
        return(-1);
    }
    
    if((headID = (*env)->GetFieldID(env, llhandlerclass, "head", "LrealHTML/tomcat/JSONMatcher/LLNode;")) == NULL) {
        rh4n_log_error(infos->logging, "Couldn't find field \"head\"");
        return(-2);
    }   

    if((infos->anker = (*env)->GetObjectField(env, varlist, headID)) == NULL) {
        rh4n_log_error(infos->logging, "Couldn't get anker field");
        return(-4);
    }
    return(0);
}

jint getJSONVarType(JNIEnv *env, GeneralInfos *infos, jobject curptr) {
    jobject typefield = NULL;
    jint vartype;

    if((typefield = (*env)->GetObjectField(env, curptr, infos->type)) == NULL) {
        rh4n_log_error(infos->logging, "Couldn't get type field");
        return(-1);
    }

    vartype = (*env)->CallIntMethod(env, typefield, infos->type_method);
    rh4n_log_debug(infos->logging, "Found vartype: [%d]", vartype);
    return(vartype);
}

void printJSONVarType(jint vartype, RH4nLogrule *logging) {
    switch(vartype) {
        case JVAR_GROUP:
            rh4n_log_debug(logging, "Group");
            break;
        case JVAR_STRING:
            rh4n_log_debug(logging, "String");
            break;
        case JVAR_STRING1D:
            rh4n_log_debug(logging, "String 1D Array");
            break;
        case JVAR_STRING2D:
            rh4n_log_debug(logging, "String 2D Array");
            break;
        case JVAR_STRING3D:
            rh4n_log_debug(logging, "String 3D Array");
            break;
        case JVAR_BOOLEAN:
            rh4n_log_debug(logging, "Boolean");
            break;
        case JVAR_BOOLEAN1D:
            rh4n_log_debug(logging, "Boolean 1D Array");
            break;
        case JVAR_BOOLEAN2D:
            rh4n_log_debug(logging, "Boolean 2D Array");
            break;
        case JVAR_BOOLEAN3D:
            rh4n_log_debug(logging, "Boolean 3D Array");
            break;
        case JVAR_INT:
            rh4n_log_debug(logging, "Integer");
            break;
        case JVAR_INT1D:
            rh4n_log_debug(logging, "Integer 1D Array");
            break;
        case JVAR_INT2D:
            rh4n_log_debug(logging, "Integer 2D Array");
            break;
        case JVAR_INT3D:
            rh4n_log_debug(logging, "Integer 3D Array");
            break;
        case JVAR_FLOAT:
            rh4n_log_debug(logging, "Float");
            break;
        case JVAR_FLOAT1D:
            rh4n_log_debug(logging, "Float 1D Array");
            break;
        case JVAR_FLOAT2D:
            rh4n_log_debug(logging, "Float 2D Array");
            break;
        case JVAR_FLOAT3D:
            rh4n_log_debug(logging, "Float 3D Array");
            break;
        case JVAR_UNKNOWN:
            rh4n_log_debug(logging, "Unknown");
            break;
    }
}

#if 0
void _printTabs(int level) {
    int i = 0;
    for(; i < level; i++)
        rh4n_log_debug(logging, "\t");
}
#endif

LLClassInfo *getLLClassInfos(JNIEnv *env, RH4nLogrule *logging) {
    static LLClassInfo infos = { NULL, NULL, NULL };

    if((infos.llclass = (*env)->FindClass(env, "java/util/LinkedList")) == NULL) {
        rh4n_log_error(logging, "Could not find LinkedList Class");
        return(NULL);
    }

    if((infos.sizeID = (*env)->GetMethodID(env, infos.llclass, "size", "()I")) == NULL) {
        rh4n_log_error(logging, "Could not find site function");
        return(NULL);
    }

    if((infos.getID = (*env)->GetMethodID(env, infos.llclass, "get", "(I)Ljava/lang/Object;")) == NULL) {
        rh4n_log_error(logging, "Could not find get function");
        return(NULL);
    }

    return(&infos);
}

