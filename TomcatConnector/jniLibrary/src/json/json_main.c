#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "standard.h"
#include "vars.h"

#include <jni.h>
#include "realHTML_tomcat_connector_JNINatural.h"
#include "json/jsonhandling.h"
#include "json/json_utils.h"
#include "json/handlers.h"

#include "var2name.h"

void printFork(JNIEnv*, GeneralInfos*, jobject, const char*, int, vars_t*);
int getAnker(JNIEnv *env, jobject varlist, GeneralInfos *infos);
int callNatural_JSON(vars_t *var_anker);

int getVarlist(JNIEnv *env, jobject varlist, vars_t **target, char *error_msg, RH4nLogrule *logging) {
    jclass llhandlerclass = NULL;    
    jfieldID headID = NULL;
    jobject anker = NULL;
    GeneralInfos *infos;
    vars_t *var_anker = NULL;

    initVarAnker(&var_anker);

    if((infos = getFieldIDs(env, logging)) == NULL) {
        sprintf(error_msg, "Could not get field IDs");
        return(-1);
    }

    if(getAnker(env, varlist, infos) < 0) {
        sprintf(error_msg, "Could not get anker");
        return(-2);
    }

    rh4n_log_debug(logging, "Got anker object");
    
    if((infos->anker = (*env)->GetObjectField(env, infos->anker, infos->nextentry)) == NULL) {
        sprintf(error_msg, "varlist is empty");
        return(-2);
    }

    printFork(env, infos, infos->anker, NULL, 0, var_anker);

    //printAllVarsToFile(var_anker, stdout);

    free(infos);
    *target = var_anker;
    return(0);
}

void printFork(JNIEnv *env, GeneralInfos *infos, jobject curptr, const char *group, int level, vars_t *var_anker) {
    jobject nextentry = NULL, nextlvl = NULL, name_obj = NULL; 
    jstring j_name = NULL;
    const char *c_name;
    int i = 0;
    jint vartype;

    int (*varhandler)(JNIEnv*, HandlerArgs) = NULL;
    HandlerArgs args = {NULL, NULL, NULL, 0, 0 };

    args.infos = infos;
    args.parent = group;
    args.var_anker = var_anker;

    nextentry = curptr;
    while(1) {
        if((name_obj = (*env)->GetObjectField(env, nextentry, infos->name)) == NULL) {
            rh4n_log_error(infos->logging, "Name field == null");
            return;
        }
        args.varname = (*env)->GetStringUTFChars(env, (jstring)name_obj, NULL);

        rh4n_log_debug(infos->logging, "Name: [%s]", args.varname);
        args.vartype = getJSONVarType(env, infos, nextentry);
        printJSONVarType(args.vartype, infos->logging);

        if(args.vartype == JVAR_GROUP) {
            addGroup(var_anker, (char*) args.varname, -1, -1, -1);
            rh4n_log_debug(infos->logging, "Created Group [%s]", args.varname);
        } else {
            if((varhandler = getHandlerFuncton(args.vartype)) == NULL) {
                printTabs(level);
                rh4n_log_error(infos->logging, "No handler for vartype defined");
            
            } else {
                args.level = level;
                args.curptr = nextentry;
                varhandler(env, args);
            }
        }

        //printf("\n");

        if((nextlvl = (*env)->GetObjectField(env, nextentry, infos->nextlvl)) != NULL) {
            printFork(env, infos, nextlvl, args.varname, level+1, var_anker);
        }

        (*env)->ReleaseStringUTFChars(env, (jstring)j_name, args.varname);

        if((nextentry = (*env)->GetObjectField(env, nextentry, infos->nextentry)) == NULL) {
            return;
        }
    }
}


