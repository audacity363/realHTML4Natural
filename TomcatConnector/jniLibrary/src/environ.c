#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jni.h"

#include "standard.h"
#include "jni_rh4n.h"
#include "environ.h"

void rh4nEnvironInit(RH4NEnvirons *environs) {
    if(environs == NULL) { return; }

    environs->length = 0;
    environs->variables = NULL;
}

int rh4nEnvironNew(RH4NEnvirons *environs, const char *name, const char *value, bool append, char *error_str) {
    int i = 0, completelength = 0, variablesindex = 0;
    char *originalvalue = NULL;

    if(environs->length == 0) {
        if((environs->variables = malloc(sizeof(char*))) == NULL) {
            return(RH4N_RET_MEMORY_ERR);
        }
        environs->length++;
    } else {
        environs->length++;
        if((environs->variables = realloc(environs->variables, environs->length*sizeof(char*))) == NULL) {
            return(RH4N_RET_MEMORY_ERR);
        }
    }

    variablesindex = environs->length-1;

    if(append) {
        originalvalue = getenv(name);
    }

    completelength = strlen(name)+strlen(value)+2;

    if(originalvalue != NULL) {
        completelength += strlen(originalvalue);
    }

    if((environs->variables[variablesindex] = malloc(completelength*sizeof(char))) == NULL) {
            return(RH4N_RET_MEMORY_ERR);
    }
    sprintf(environs->variables[variablesindex], "%s=%s", name, value);

    if(originalvalue !=  NULL) {
        strcat(environs->variables[variablesindex], originalvalue);
    }
    
    return(RH4N_RET_OK);
}

int rh4nEnvironReadout(JNIEnv *env, jobject jenviron, RH4NEnvirons *environs, char *error_str) {
    if(jenviron == NULL) { return(RH4N_RET_OK); }

    jint jarraylength = 0;
    jclass jcenviron = NULL;
    jfieldID jfname = NULL, jfvalue = NULL, jfappend;
    jobject joname = NULL, jovalue = NULL, joappend = NULL, jotarget = NULL;
    jboolean jbappend = false;

    int i = 0, ret = 0;
    const char *cname = NULL, *cvalue = NULL;

    jarraylength = (*env)->GetArrayLength(env, jenviron);
    if(jarraylength == 0) { return(RH4N_RET_OK); }

    if((jcenviron = (*env)->FindClass(env, RH4N_CLASS_ENVIRON)) == NULL) {
        sprintf(error_str, "Could not find class [%s]", RH4N_CLASS_ENVIRON);
        return(RH4N_RET_JNI_ERR);
    }

    if((jfname = (*env)->GetFieldID(env, jcenviron, "name", RH4N_CLASS_STRING)) == NULL) {
        sprintf(error_str, "Could not get field [name] from class [%s]", RH4N_CLASS_ENVIRON);
        return(RH4N_RET_JNI_ERR);
    }

    if((jfvalue = (*env)->GetFieldID(env, jcenviron, "value", RH4N_CLASS_STRING)) == NULL) {
        sprintf(error_str, "Could not get field [value] from class [%s]", RH4N_CLASS_ENVIRON);
        return(RH4N_RET_JNI_ERR);
    }

    if((jfappend = (*env)->GetFieldID(env, jcenviron, "append", "Z")) == NULL) {
        sprintf(error_str, "Could not get field [append] from class [%s]", RH4N_CLASS_ENVIRON);
        return(RH4N_RET_JNI_ERR);
    }


    for(; i < jarraylength; i++) {
        if((jotarget = (*env)->GetObjectArrayElement(env, jenviron, i)) == NULL) {
            sprintf(error_str, "Could not get environ array element [%d]\n", i);
            return(RH4N_RET_JNI_ERR);
        }

        if((joname = (*env)->GetObjectField(env, jotarget, jfname)) == NULL) {
            sprintf(error_str, "Could not get environ name from array element [%d]", i);
            return(RH4N_RET_JNI_ERR);
        }

        if((cname = (*env)->GetStringUTFChars(env, (jstring)joname, NULL)) == NULL) {
            sprintf(error_str, "Could not get environ name string from array element [%d]", i);
            return(RH4N_RET_JNI_ERR);
        }

        if((jovalue = (*env)->GetObjectField(env, jotarget, jfvalue)) == NULL) {
            sprintf(error_str, "Could not get environ name from array element [%d]", i);
            return(RH4N_RET_JNI_ERR);
        }

        if((cvalue = (*env)->GetStringUTFChars(env, (jstring)jovalue, NULL)) == NULL) {
            (*env)->ReleaseStringUTFChars(env, (jstring)joname, cname);
            sprintf(error_str, "Could not get environ name string from array element [%d]", i);
            return(RH4N_RET_JNI_ERR);
        }

        jbappend = (*env)->GetBooleanField(env, jotarget, jfappend);

        if((ret = rh4nEnvironNew(environs, cname, cvalue, (bool)jbappend, error_str)) != RH4N_RET_OK) {
            (*env)->ReleaseStringUTFChars(env, (jstring)joname, cname);
            (*env)->ReleaseStringUTFChars(env, (jstring)jovalue, cvalue);
            return(ret);
        }

        (*env)->ReleaseStringUTFChars(env, (jstring)joname, cname);
        (*env)->ReleaseStringUTFChars(env, (jstring)jovalue, cvalue);

    }

    return(RH4N_RET_OK);
}

int rh4nEnvironPutAll(RH4NEnvirons *environs) {
    if(environs == NULL) { return(RH4N_RET_FORMAT_ERR); }

    int i = 0;

    for(; i < environs->length; i++) {
        if(putenv(environs->variables[i]) != 0) {
            return(RH4N_RET_MEMORY_ERR);
        }
    }

    return(RH4N_RET_OK);
}

void rh4nEnvironPrint(RH4NEnvirons *environs) {
    if(environs == NULL) { return; }

    int i = 0;

    for(; i < environs->length; i++) {
        printf("Environ [%d]: [%s]\n", i, environs->variables[i]);
    }
}

void rh4nEnvironFree(RH4NEnvirons *environs) {
    if(environs == NULL) { return; }

    int i = 0;

    for(; i < environs->length; i++) {
        free(environs->variables[i]);
    }

    free(environs->variables);
    environs->length = 0;
}

