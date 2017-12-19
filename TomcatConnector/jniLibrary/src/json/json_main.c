#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vars.h"

#include <jni.h>
#include "Test.h"
#include "jsonhandling.h"
#include "json_utils.h"
#include "handlers.h"

void printFork(JNIEnv*, GeneralInfos*, jobject, const char*, int, vars_t*);
int getAnker(JNIEnv *env, jobject varlist, GeneralInfos *infos);
int callNatural_JSON(vars_t *var_anker);

JNIEXPORT jint JNICALL Java_Test_dumpVars
  (JNIEnv *env, jobject in_cls, jobject varlist) {
    jclass llhandlerclass = NULL;    
    jfieldID headID = NULL;
    jobject anker = NULL;
    GeneralInfos *infos;
    vars_t *var_anker = NULL;

    initVarAnker(&var_anker);

    if((infos = getFieldIDs(env)) == NULL) {
        return(-1);
    }

    if(getAnker(env, varlist, infos) < 0) {
        return(-2);
    }

    printf("Got anker object\n");
    
    if((infos->anker = (*env)->GetObjectField(env, infos->anker, infos->nextentry)) == NULL) {
        printf("varlist is empty\n");
        return(-5);
    }

    printFork(env, infos, infos->anker, NULL, 0, var_anker);

    printAllVarsToFile(var_anker, stdout);


    callNatural_JSON(var_anker);
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
            printf("Name field == null\n");
            return;
        }
        args.varname = (*env)->GetStringUTFChars(env, (jstring)name_obj, NULL);

        printTabs(level);
        printf("Name: [%s]\n", args.varname);
        printTabs(level);
        args.vartype = getJSONVarType(env, infos, nextentry);
        printTabs(level);
        printVarType(args.vartype);

        if(args.vartype == JVAR_GROUP) {
            addGroup(var_anker, (char*) args.varname, -1, -1, -1);
            //printf("Created Group [%s]\n", args.varname);
        } else {
            if((varhandler = getHandlerFuncton(args.vartype)) == NULL) {
                printTabs(level);
                printf("No handler for vartype defined\n");
            
            } else {
                args.level = level;
                args.curptr = nextentry;
                varhandler(env, args);
            }
        }

        printf("\n");

        if((nextlvl = (*env)->GetObjectField(env, nextentry, infos->nextlvl)) != NULL) {
            printFork(env, infos, nextlvl, args.varname, level+1, var_anker);
        }

        (*env)->ReleaseStringUTFChars(env, (jstring)j_name, args.varname);

        if((nextentry = (*env)->GetObjectField(env, nextentry, infos->nextentry)) == NULL) {
            return;
        }
    }
}


