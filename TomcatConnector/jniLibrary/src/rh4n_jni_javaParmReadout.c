#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "jni.h"

#include "standard.h"
#include "rh4n_jni_javaParmReadout.h"

int rh4nReadOutParms(JNIEnv *env, jobject jnatparams, RH4nProperties *properties, char *error_str) {
    struct javaParm parms[] = {
        {"reqType", "Ljava/lang/String;", rh4nParmReqTypeHandler},
        {"natLibrary", "Ljava/lang/String;", rh4nParmNatLibraryHandler},
        {"natProgram", "Ljava/lang/String;", rh4nParmNatProgramHandler},
        {"natparms", "Ljava/lang/String;", rh4nParmNatParmsHandler},
        {"outputfile", "Ljava/lang/String;", rh4nParmOutputfileHandler},
        {"loglevel", "Ljava/lang/String;", rh4nParmLoglevelHandler},
        {"natsrcpath", "Ljava/lang/String;", rh4nParmNatSrcPathHandler},
        {"logpath", "Ljava/lang/String;", rh4nParmLogPathHandler},
        {"errorRepresentation", "Ljava/lang/String;", rh4nParmErrorRepresentationHandler}
    };
    int i = 0, ret = 0;
    jclass jcrh4nparams;
    jfieldID jfid;
    jobject target;
    const char *strvalue = NULL;

    jcrh4nparams = (*env)->GetObjectClass(env, jnatparams);
    if(jcrh4nparams == NULL) {
        sprintf(error_str, "Could not get Class from parms");
        return(RH4N_RET_JNI_ERR);
    }

    for(; i < sizeof(parms)/sizeof(struct javaParm); i++) {

        //printf("Search for [%s] type [%s]\n", parms[i].jname, parms[i].jtype);
        jfid = (*env)->GetFieldID(env, jcrh4nparams, parms[i].jname, parms[i].jtype);

        if(jfid == NULL) {
            sprintf(error_str, "Could not get field ID from field [%s]", parms[i].jname);
            return(RH4N_RET_JNI_ERR);
        }

        if((target = (*env)->GetObjectField(env, jnatparams, jfid)) == NULL) {
            sprintf(error_str, "Could not get field [%s]", parms[i].jname);
            return(RH4N_RET_JNI_ERR);
        }
        if((strvalue = (*env)->GetStringUTFChars(env, (jstring)target, NULL)) == NULL) {
            sprintf(error_str, "Field [%s] == NULL", parms[i].jname);
            return(RH4N_RET_JNI_ERR);
        }

        if((ret = parms[i].handler(env, strvalue, properties, error_str)) != RH4N_RET_OK) {
            (*env)->ReleaseStringUTFChars(env, (jstring)target, strvalue);
            return(ret);
        }

        (*env)->ReleaseStringUTFChars(env, (jstring)target, strvalue);

    }
    if((ret = rh4NUrlVariableHandler(env, jcrh4nparams, jnatparams, properties, error_str)) != RH4N_RET_OK) { return(ret); }
        
    return(RH4N_RET_OK);
}

int rh4NUrlVariableHandler(JNIEnv *env, jclass jcrh4nparms, jobject jorh4nparms, RH4nProperties *properties, char *error_str) {
    jfieldID jfnames = NULL, jfvalues = NULL;
    jobject jonames = NULL, jovalues = NULL, joname = NULL, jovalue = NULL;
    jint jnameslength = 0, jvalueslength = 0;

    const char *cname = NULL, *cvalue = NULL;
    int i = 0, rc = 0;


    if((jfnames = (*env)->GetFieldID(env, jcrh4nparms, "urlVarsKey", "[Ljava/lang/String;")) == NULL) {
        sprintf(error_str, "Could not get field ID from field [urlVarsKey]");
        return(RH4N_RET_JNI_ERR);
    }

    if((jonames = (*env)->GetObjectField(env, jorh4nparms, jfnames)) == NULL) {
        sprintf(error_str, "Could not get field [urlVarsKey]");
        return(RH4N_RET_JNI_ERR);

    }

    if((jfvalues = (*env)->GetFieldID(env, jcrh4nparms, "urlVarsValue", "[Ljava/lang/String;")) == NULL) {
        sprintf(error_str, "Could not get field ID from field [urlVarsValue]");
        return(RH4N_RET_JNI_ERR);
    }

    if((jovalues = (*env)->GetObjectField(env, jorh4nparms, jfvalues)) == NULL) {
        sprintf(error_str, "Could not get field [urlVarsKey]");
        return(RH4N_RET_JNI_ERR);

    }

    jnameslength = (*env)->GetArrayLength(env, jonames);
    jvalueslength = (*env)->GetArrayLength(env, jovalues);

    if(jnameslength != jvalueslength) {
        sprintf(error_str, "URLKey array and URLValue array are not the same length [%d] != [%d]", jnameslength, jvalueslength);
        return(RH4N_RET_VAR_MISSMATCH);
    }

    rh4nvarInitList(&properties->urlvars);

    for(; i < jnameslength; i++) {
        if((joname = (*env)->GetObjectArrayElement(env, jonames, i)) == NULL) {
            sprintf(error_str, "Could not get URLKey array element [%d]\n", i);
            return(RH4N_RET_JNI_ERR);
        }
        if((jovalue = (*env)->GetObjectArrayElement(env, jovalues, i)) == NULL) {
            sprintf(error_str, "Could not get URLValue array element [%d]\n", i);
            return(RH4N_RET_JNI_ERR);
        }

        if((cname = (*env)->GetStringUTFChars(env, (jstring)joname, NULL)) == NULL) {
            sprintf(error_str, "Could not get URLKey string from array element [%d]\n", i);
            return(RH4N_RET_JNI_ERR);
        }
        if((cvalue = (*env)->GetStringUTFChars(env, (jstring)jovalue, NULL)) == NULL) {
            sprintf(error_str, "Could not get URLValue string from array element [%d]\n", i);
            return(RH4N_RET_JNI_ERR);
        } 

        rh4n_log_debug(properties->logging, "URLVar: Name: [%s] Value: [%s]\n", cname, cvalue);

        if((rc = rh4nvarCreateNewString(&properties->urlvars, NULL, (char*)cname, (char*)cvalue)) != RH4N_RET_OK) {
            sprintf(error_str, "Could not create new String [%s]. Varlib return: %d", cname, rc);
            rh4n_log_error(properties->logging, "%s", error_str);
            (*env)->ReleaseStringUTFChars(env, (jstring)joname, cname);
            (*env)->ReleaseStringUTFChars(env, (jstring)jovalue, cvalue);
            return(rc);
        }

        (*env)->ReleaseStringUTFChars(env, (jstring)joname, cname);
        (*env)->ReleaseStringUTFChars(env, (jstring)jovalue, cvalue);
    }


    return(RH4N_RET_OK);

}

int rh4nParmReqTypeHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 

    if(strlen(strvalue) > sizeof(properties->httpreqtype)) {
        sprintf(error_str, "ReqType to big. (MaxLength = [%d])", sizeof(properties->httpreqtype));
        return(RH4N_RET_BUFFER_OVERFLOW);
    }

    strcpy(properties->httpreqtype, strvalue);
    return(RH4N_RET_OK);
}

int rh4nParmNatLibraryHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if(strlen(strvalue) > sizeof(properties->natlibrary)) {
        sprintf(error_str, "NatLibrary to big. (MaxLength = [%d])", sizeof(properties->natlibrary));
        return(RH4N_RET_BUFFER_OVERFLOW);
    }
    strcpy(properties->natlibrary, strvalue);
    return(RH4N_RET_OK);
}

int rh4nParmNatProgramHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if(strlen(strvalue) > sizeof(properties->natprogram)) {
        sprintf(error_str, "NatProgram to big. (MaxLength = [%d])", sizeof(properties->natprogram));
        return(RH4N_RET_BUFFER_OVERFLOW);
    }
    strcpy(properties->natprogram, strvalue);
    return(RH4N_RET_OK);
}

int rh4nParmNatParmsHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if((properties->natparms = malloc(strlen(strvalue)+1)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    strcpy(properties->natparms, strvalue);
    return(RH4N_RET_OK);
}

int rh4nParmOutputfileHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if((properties->outputfile = malloc(strlen(strvalue)+1)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    strcpy(properties->outputfile, strvalue);
    return(RH4N_RET_OK);
}

int rh4nParmLoglevelHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if(strlen(strvalue) > sizeof(properties->c_loglevel)) {
        sprintf(error_str, "loglevel to big. (MaxLength = [%d])", sizeof(properties->c_loglevel));
        return(RH4N_RET_BUFFER_OVERFLOW);
    }

    strcpy(properties->c_loglevel, strvalue);
    if((properties->i_loglevel = rh4nLoggingConvertStrtoInt(properties->c_loglevel)) < 0) {
        sprintf(error_str, "Unkown logging level [%s]. Could not init logging.", properties->c_loglevel);
        return(RH4N_RET_LOGGING_ERR);
    }

    return(RH4N_RET_OK);
}

int rh4nParmNatSrcPathHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if((properties->natsrcpath = malloc(strlen(strvalue)+1)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    strcpy(properties->natsrcpath, strvalue);
    return(RH4N_RET_OK);
}

int rh4nParmLogPathHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if(strlen(strvalue) == 0) {
        sprintf(error_str, "Loggingpath is empty.");
        return(RH4N_RET_LOGGING_ERR);
    }
    if((properties->logpath = malloc(strlen(strvalue)+1)) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    strcpy(properties->logpath, strvalue);
    return(RH4N_RET_OK);
}

int rh4nParmErrorRepresentationHandler(JNIEnv *env, const char *strvalue, RH4nProperties *properties, char *error_str) { 
    if(strlen(strvalue) > sizeof(properties->errorrepresentation)) {
        sprintf(error_str, "loglevel to big. (MaxLength = [%d])", sizeof(properties->errorrepresentation));
        return(RH4N_RET_BUFFER_OVERFLOW);
    }

    strcpy(properties->errorrepresentation, strvalue);
    return(RH4N_RET_OK);
}

void rh4nPrintPropertiesStruct(RH4nProperties *properties) {
    if(properties == NULL) { return; }

    rh4n_log_debug(properties->logging, "NatLibrary:          [%s]", properties->natlibrary);
    rh4n_log_debug(properties->logging, "NatProgram:          [%s]", properties->natprogram);
    rh4n_log_debug(properties->logging, "NatSRCPath:          [%s]", properties->natsrcpath);
    rh4n_log_debug(properties->logging, "NatParms:            [%s]", properties->natparms);
    rh4n_log_debug(properties->logging, "ReqType:             [%s]", properties->httpreqtype);
    rh4n_log_debug(properties->logging, "Loglevel:            [%s]", properties->c_loglevel);
    rh4n_log_debug(properties->logging, "Outputfile:          [%s]", properties->outputfile);
    rh4n_log_debug(properties->logging, "Logpath:             [%s]", properties->logpath);
    rh4n_log_debug(properties->logging, "ErrorRepresentation: [%s]", properties->errorrepresentation);
    //TODO: rewrite the print function fromm the variable lib with the new logging lib
    //rh4n_log_debug("Url Variables:");
    //printAllVarsToFile(properties->urlvars, stdout);
    //rh4n_log_debug("Body Variables:");
    //printAllVarsToFile(properties->bodyvars, stdout);
}

void rh4nFreePropertiesStruct(RH4nProperties *properties) {
    if(properties == NULL) { return; }

    free(properties->natparms);
    free(properties->natsrcpath);
    free(properties->outputfile);
}

void rh4nInitPropertiesStruct(RH4nProperties *properties) {
    memset(properties, NULL, sizeof(RH4nProperties));
}
