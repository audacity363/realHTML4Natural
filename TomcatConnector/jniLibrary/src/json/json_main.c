#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rh4n.h"

#include <jni.h>
#include "realHTML_tomcat_connector_JNINatural.h"
#include "json/jsonhandling.h"
#include "json/json_utils.h"
#include "json/handlers.h"

int printFork(JNIEnv*, GeneralInfos*, jobject, const char*, int, RH4nVarList*);
int getAnker(JNIEnv *env, jobject varlist, GeneralInfos *infos);

int getVarlist(JNIEnv *env, jobject jvarlist, RH4nVarList *varlist, char *error_msg, RH4nLogrule *logging) {
    GeneralInfos *infos = NULL;
    int rc = 0;

    if((infos = getFieldIDs(env, logging)) == NULL) {
        sprintf(error_msg, "Could not get field IDs");
        return(-1);
    }

    if(getAnker(env, jvarlist, infos) < 0) {
        sprintf(error_msg, "Could not get anker");
        return(-2);
    }

    rh4n_log_debug(logging, "Got anker object");
    
    if((infos->anker = (*env)->GetObjectField(env, infos->anker, infos->nextentry)) == NULL) {
        sprintf(error_msg, "varlist is empty");
        return(-2);
    }

    rc = printFork(env, infos, infos->anker, NULL, 0, varlist);

    //printAllVarsToFile(var_anker, stdout);

    free(infos);
    return(rc);
}

int printFork(JNIEnv *env, GeneralInfos *infos, jobject curptr, const char *group, int level, RH4nVarList *varlist) {
    jobject nextentry = NULL, nextlvl = NULL, name_obj = NULL; 
    jstring j_name = NULL;
    int rc = 0;

    int (*varhandler)(JNIEnv*, HandlerArgs) = NULL;
    HandlerArgs args = {NULL, NULL, NULL, 0, 0 };

    args.infos = infos;
    args.parent = group;
    args.var_anker = varlist;

    nextentry = curptr;
    while(1) {
        if((name_obj = (*env)->GetObjectField(env, nextentry, infos->name)) == NULL) {
            rh4n_log_error(infos->logging, "Name field == null");
            return(RH4N_RET_JNI_ERR);
        }
        args.varname = (*env)->GetStringUTFChars(env, (jstring)name_obj, NULL);

        rh4n_log_debug(infos->logging, "Name: [%s]", args.varname);
        args.vartype = getJSONVarType(env, infos, nextentry);
        printJSONVarType(args.vartype, infos->logging);

        if(args.vartype == JVAR_GROUP) {
            if((rc = rh4nvarCreateNewGroup(varlist, (char*)args.varname)) != RH4N_RET_OK) {
                rh4n_log_error(infos->logging, "Could not create group [%s]. Varlib return: %d", args.varname, rc);
                return(rc);
            }
            rh4n_log_debug(infos->logging, "Created Group [%s]", args.varname);
        } else {
            if((varhandler = getHandlerFuncton(args.vartype)) == NULL) {
                //printTabs(level);
                rh4n_log_error(infos->logging, "No handler for vartype defined");
            } else {
                args.level = level;
                args.curptr = nextentry;
                if((rc = varhandler(env, args)) != RH4N_RET_OK) return(rc);
            }
        }

        //printf("\n");

        if((nextlvl = (*env)->GetObjectField(env, nextentry, infos->nextlvl)) != NULL) {
            if((rc = printFork(env, infos, nextlvl, args.varname, level+1, varlist)) != RH4N_RET_OK) return(rc);
        }

        (*env)->ReleaseStringUTFChars(env, (jstring)j_name, args.varname);

        if((nextentry = (*env)->GetObjectField(env, nextentry, infos->nextentry)) == NULL) {
            return(RH4N_RET_OK);
        }
    }
    return(RH4N_RET_OK);
}


