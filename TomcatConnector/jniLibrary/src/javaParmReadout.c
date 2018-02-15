#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "jni.h"

#include "vars.h"

#include "standard.h"


int rh4nParmReqTypeHandler(JNIEnv *env, jobject joreqtype, RH4nProperties *properties, char *error_str);
int rh4nParmNatLibraryHandler(JNIEnv *env, jobject jonatlib, RH4nProperties *properties, char *error_str);
int rh4nParmNatProgramHandler(JNIEnv *env, jobject jonatprogram, RH4nProperties *properties, char *error_str);
int rh4nParmNatParmsHandler(JNIEnv *env, jobject jonatparms, RH4nProperties *properties, char *error_str);
int rh4nParmOutputfileHandler(JNIEnv *env, jobject jooutputfile, RH4nProperties *properties, char *error_str);
int rh4nParmLoglevelHandler(JNIEnv *env, jobject jologlevel, RH4nProperties *properties, char *error_str);
int rh4nParmNatSrcPathHandler(JNIEnv *env, jobject jonatsrc, RH4nProperties *properties, char *error_str);

struct javaParm {
    char *jname;
    char *jtype;
    int (*handler)(JNIEnv*, jobject, RH4nProperties*, char*);
};

int readOutParms(JNIEnv *env, jobject jnatparams, RH4nProperties *properties, char *error_str) {
    struct javaParm parms[] = {
        {"reqType", "Ljava/lang/String;", rh4nParmReqTypeHandler},
        {"nat_library", "Ljava/lang/String;", rh4nParmNatLibraryHandler},
        {"nat_program", "Ljava/lang/String;", rh4nParmNatProgramHandler},
        {"natparms", "Ljava/lang/String;", rh4nParmNatParmsHandler},
        {"tmp_file", "Ljava/lang/String;", rh4nParmOutputfileHandler},
        {"loglevel", "Ljava/lang/String;", rh4nParmLoglevelHandler},
        {"natsrcpath", "Ljava/lang/String;", rh4nParmNatSrcPathHandler}
    };
    int i = 0;
    jclass jcrh4nparams;
    jfieldID jfid;
    jobject target;

    jcrh4nparams = (*env)->GetObjectClass(env, jnatparams);
    if(jcrh4nparams == NULL) {
        sprintf(error_str, "Could not get Class from parms");
        return(-1);
    }

    for(; i < sizeof(parms)/sizeof(struct javaParm); i++) {

        printf("Search for [%s] type [%s]\n", parms[i].jname, parms[i].jtype);
        jfid = (*env)->GetFieldID(env, jcrh4nparams, parms[i].jname, parms[i].jtype);

        if(jfid == NULL) {
            sprintf(error_str, "Could not get field ID from field [%s]", parms[i].jname);
            return(-1);
        }

        if((target = (*env)->GetObjectField(env, jnatparams, jfid)) == NULL) {
            sprintf(error_str, "Could not get field [%s]", parms[i].jname);
            return(-1);
        }

        if(parms[i].handler(env, target, properties, error_str) != 0) {
            return(-1);
        }

    }
    return(0);
}

int rh4nParmReqTypeHandler(JNIEnv *env, jobject joreqtype, RH4nProperties *properties, char *error_str) { 

    const char *creqtype = NULL;
    if((creqtype = (*env)->GetStringUTFChars(env, (jstring)joreqtype, NULL)) == NULL) {
        sprintf(error_str, "req_type String == NULL");
        return(-1);
    }

    strcpy(properties->httpreqtype, creqtype);

    (*env)->ReleaseStringUTFChars(env, (jstring)joreqtype, creqtype);
}

int rh4nParmNatLibraryHandler(JNIEnv *env, jobject jonatlib, RH4nProperties *properties, char *error_str) { 
    const char *creqtype = NULL;
    printf("in LibraryHandler\n");
    if((creqtype = (*env)->GetStringUTFChars(env, (jstring)jonatlib, NULL)) == NULL) {
        sprintf(error_str, "req_type String == NULL");
        return(-1);
    }

    strcpy(properties->httpreqtype, creqtype);

    (*env)->ReleaseStringUTFChars(env, (jstring)jonatlib, creqtype);
    return(0);
}

int rh4nParmNatProgramHandler(JNIEnv *env, jobject jonatprogram, RH4nProperties *properties, char *error_str) { }

int rh4nParmNatParmsHandler(JNIEnv *env, jobject jonatparms, RH4nProperties *properties, char *error_str) { }

int rh4nParmOutputfileHandler(JNIEnv *env, jobject jooutputfile, RH4nProperties *properties, char *error_str) { }

int rh4nParmLoglevelHandler(JNIEnv *env, jobject jologlevel, RH4nProperties *properties, char *error_str) { }

int rh4nParmNatSrcPathHandler(JNIEnv *env, jobject jonatsrc, RH4nProperties *properties, char *error_str) { }
