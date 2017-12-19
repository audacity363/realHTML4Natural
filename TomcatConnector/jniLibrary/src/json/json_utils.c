#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vars.h"

#include <jni.h>
#include "jsonhandling.h"
#include "json_utils.h"

GeneralInfos *getFieldIDs(JNIEnv *env) {
    static GeneralInfos infos;

    jclass nodeclass = NULL, typesclass = NULL;

    if((nodeclass = (*env)->FindClass(env, "realHTML/tomcat/JSONMatcher/LLNode")) == NULL) {
        printf("Could not find class LLNode\n");
        return(NULL);
    }

    if((infos.nextentry = (*env)->GetFieldID(env, nodeclass, "next", "LrealHTML/tomcat/JSONMatcher/LLNode;")) == NULL) {
        printf("Couldn't find field \"next\"\n");
        return(NULL);
    }

    if((infos.nextlvl = (*env)->GetFieldID(env, nodeclass, "nextlvl", "LrealHTML/tomcat/JSONMatcher/LLNode;")) == NULL) {
        printf("Couldn't find field \"nextlvl\"\n");
        return(NULL);
    }

    if((infos.type = (*env)->GetFieldID(env, nodeclass, "type", "LrealHTML/tomcat/JSONMatcher/Types;")) == NULL) {
        printf("Couldn't find field \"type\"\n");
        return(NULL);
    }

    if((infos.name = (*env)->GetFieldID(env, nodeclass, "name", "Ljava/lang/String;")) == NULL) {
        printf("Couldn't find field \"name\"\n");
        return(NULL);
    }

    if((infos.value = (*env)->GetFieldID(env, nodeclass, "value", "Ljava/lang/Object;")) == NULL) {
        printf("Couldn't find field \"value\"\n");
        return(NULL);
    }

    if((typesclass = (*env)->FindClass(env, "realHTML/tomcat/JSONMatcher/Types")) == NULL) {
        printf("Could not find class Types\n");
        return(NULL);
    }

    if((infos.type_method = (*env)->GetMethodID(env, typesclass, "getNumberRep", "()I")) == NULL) {
        printf("Could not find method \"getNumberRep\"\n");
        return(NULL);
    }

    return(&infos);
}

int getAnker(JNIEnv *env, jobject varlist, GeneralInfos *infos) {
    jclass llhandlerclass = NULL;
    jfieldID headID = NULL;

    if((llhandlerclass = (*env)->FindClass(env, "realHTML/tomcat/JSONMatcher/LLHandler")) == NULL) {
        printf("Could find class LLHandler\n");
        return(-1);
    }
    
    if((headID = (*env)->GetFieldID(env, llhandlerclass, "head", "LrealHTML/tomcat/JSONMatcher/LLNode;")) == NULL) {
        printf("Couldn't find field \"head\"\n");
        return(-2);
    }   

    if((infos->anker = (*env)->GetObjectField(env, varlist, headID)) == NULL) {
        printf("Couldn't get anker field\n");
        return(-4);
    }
    return(0);
}

jint getJSONVarType(JNIEnv *env, GeneralInfos *infos, jobject curptr) {
    jobject typefield = NULL;
    jint vartype;

    if((typefield = (*env)->GetObjectField(env, curptr, infos->type)) == NULL) {
        printf("Couldn't get type field\n");
        return(-1);
    }

    vartype = (*env)->CallIntMethod(env, typefield, infos->type_method);
    printf("Found vartype: [%d]\n", vartype);
    return(vartype);
}

void printVarType(jint vartype) {
    switch(vartype) {
        case JVAR_GROUP:
            printf("Group\n");
            break;
        case JVAR_STRING:
            printf("String\n");
            break;
        case JVAR_STRING1D:
            printf("String 1D Array\n");
            break;
        case JVAR_STRING2D:
            printf("String 2D Array\n");
            break;
        case JVAR_STRING3D:
            printf("String 3D Array\n");
            break;
        case JVAR_BOOLEAN:
            printf("Boolean\n");
            break;
        case JVAR_BOOLEAN1D:
            printf("Boolean 1D Array\n");
            break;
        case JVAR_BOOLEAN2D:
            printf("Boolean 2D Array\n");
            break;
        case JVAR_BOOLEAN3D:
            printf("Boolean 3D Array\n");
            break;
        case JVAR_INT:
            printf("Integer\n");
            break;
        case JVAR_INT1D:
            printf("Integer 1D Array\n");
            break;
        case JVAR_INT2D:
            printf("Integer 2D Array\n");
            break;
        case JVAR_INT3D:
            printf("Integer 3D Array\n");
            break;
        case JVAR_FLOAT:
            printf("Float\n");
            break;
        case JVAR_FLOAT1D:
            printf("Float 1D Array\n");
            break;
        case JVAR_FLOAT2D:
            printf("Float 2D Array\n");
            break;
        case JVAR_FLOAT3D:
            printf("Float 3D Array\n");
            break;
        case JVAR_UNKNOWN:
            printf("Unknown\n");
            break;
    }
}

void printTabs(int level) {
    int i = 0;
    for(; i < level; i++)
        printf("\t");
}

LLClassInfo *getLLClassInfos(JNIEnv *env) {
    static LLClassInfo infos = { NULL, NULL, NULL };

    if((infos.llclass = (*env)->FindClass(env, "java/util/LinkedList")) == NULL) {
        printf("Could not find LinkedList Class\n");
        return(NULL);
    }

    if((infos.sizeID = (*env)->GetMethodID(env, infos.llclass, "size", "()I")) == NULL) {
        printf("Could not find site function\n");
        return(NULL);
    }

    if((infos.getID = (*env)->GetMethodID(env, infos.llclass, "get", "(I)Ljava/lang/Object;")) == NULL) {
        printf("Could not find get function\n");
        return(NULL);
    }

    return(&infos);
}

