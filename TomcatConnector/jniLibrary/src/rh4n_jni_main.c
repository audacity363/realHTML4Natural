#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"

#include "realHTML_tomcat_connector_JNINatural.h"
#include "rh4n_jni.h"
#include "rh4n_jni_javaParmReadout.h"
#include "rh4n_jni_environ.h"
#include "natural/rh4n_natcall.h"

#define VERSIONSTR "realHTML4Natural Tomcat Connector JNILibrary Version 2.0.1"

jobject rh4nJNIcreateReturnObj(JNIEnv *env, int exit_code, char*);

JNIEXPORT jint JNICALL Java_realHTML_tomcat_connector_JNINatural_jni_1printVersion
  (JNIEnv *env, jobject parent)
{
    printf("%s\n", VERSIONSTR);
    return(0);
}

JNIEXPORT jobject JNICALL Java_realHTML_tomcat_connector_JNINatural_jni_1callNatural
  (JNIEnv *env, jclass in_cls, jobject jnatparams, jobjectArray joenvirons, jobject bodyvars) {  
        char error_str[2048];
        RH4nProperties props;
        RH4NEnvirons environs;
        int naturalret = 0;

        rh4nInitPropertiesStruct(&props);
        if(rh4nReadOutParms(env, jnatparams, &props, error_str) != 0) {
            printf("Error: [%s]\n", error_str);
            rh4nFreePropertiesStruct(&props);
            return(rh4nJNIcreateReturnObj(env, -1, error_str));
        }
        if((props.logging = rh4nLoggingCreateRule(props.natlibrary, props.natprogram, props.i_loglevel, props.logpath)) == NULL) {
            rh4nFreePropertiesStruct(&props);
            printf("Could not create  Loggingrule\n");
            return(rh4nJNIcreateReturnObj(env, -2, "Could not create Loggingrule"));
        }
        rh4nPrintPropertiesStruct(&props);

        rh4n_log_info(props.logging, "Starting reading out Envrions");
        rh4nEnvironInit(&environs);
        if(rh4nEnvironReadout(env, joenvirons, &environs, error_str, props.logging) != RH4N_RET_OK) {
            rh4n_log_error(props.logging, "%s", error_str);
            return(rh4nJNIcreateReturnObj(env, -3, error_str));
        }
        rh4nEnvironPrint(&environs, props.logging);
        rh4n_log_info(props.logging, "Finished reading out Envrions");

        if(bodyvars != NULL) {
            rh4n_log_info(props.logging, "Start parsing body variables");
            if(getVarlist(env, bodyvars, &props.bodyvars, error_str, props.logging) < 0) {
                rh4n_log_error(props.logging, "getVarlist() - [%s]", error_str);
                return(rh4nJNIcreateReturnObj(env, -4, error_str));
            }
            rh4n_log_info(props.logging, "Finished parsing body variables");
        } else { rh4n_log_info(props.logging, "No body variables are provided"); }

        naturalret = rh4nNaturalcreateProcess(&props, &environs, error_str);

        rh4nFreePropertiesStruct(&props);
        rh4nEnvironFree(&environs);
        return(rh4nJNIcreateReturnObj(env, naturalret, ""));
  }

jobject rh4nJNIcreateReturnObj(JNIEnv *env, int exit_code, char *error_msg)
{
    jclass jrh4nreturn;
    jmethodID jmid;
    jobject jretobj;
    jfieldID jfid;

    jrh4nreturn = (*env)->FindClass(env, "realHTML/tomcat/connector/RH4NReturn");
    jmid = (*env)->GetMethodID(env, jrh4nreturn, "<init>", "()V");

    jretobj = (*env)->NewObject(env, jrh4nreturn, jmid);

    jfid = (*env)->GetFieldID(env, jrh4nreturn, "natprocess_ret", "I");
    (*env)->SetIntField(env, jretobj, jfid, exit_code);


    jfid = (*env)->GetFieldID(env, jrh4nreturn, "error_msg", "Ljava/lang/String;");
    (*env)->SetObjectField(env, jretobj, jfid, (*env)->NewStringUTF(env, error_msg));

    return(jretobj);
}

char *natErrno2Str(int naterrno)
{
    static char *const natstrerror[] = {
        "Illegal parameter number",
        "Internal Error",
        "Data have been truncated",
        "Parameter is not an array",
        "Parameter is write protcted",
        "Out of memory when dynamic parameter is resized",
        "Invalid version of interface struct",
        "Invalid data format",
        "Invalid length or precision",
        "Invalid dimension count",
        "Invalid x-array bound definition",
        "Array cannot be resized in the way requested",
        "Index for dimension 0 out of range",
        "Index for dimension 1 out of range",
        "Index for dimension 2 out of range"
    };
    static char startuperror[1024];

    if(naterrno < NNI_RC_SERR_OFFSET)
    {
        sprintf(startuperror, "Natural Startup Error [%d]", naterrno-NNI_RC_SERR_OFFSET);
        return(startuperror);
    }

    if(naterrno == NNI_RC_BAD_INDEX_0)
        naterrno = 14;
    else if(naterrno == NNI_RC_BAD_INDEX_1)
        naterrno = 15;
    else if(naterrno == NNI_RC_BAD_INDEX_2)
        naterrno = 16;
    else 
        naterrno = naterrno * -1;

    return(natstrerror[naterrno]);
}

