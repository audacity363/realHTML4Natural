#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vars.h"

#include "realHTML_tomcat_connector_JNINatural.h"
#include "natni.h"
#include "jni_rh4n.h"
#include "standard.h"

#define VERSIONSTR "realHTML4Natural Tomcat Connector JNILibrary Version 2.0.1"

int stringHandle(JNIEnv *env, jobject, struct naturalparms*);
int arrayHandle(JNIEnv *env, jobject, struct naturalparms*);

void *stringGetter(JNIEnv *env, struct naturalparms *pparmtarget, int index);
void *arrayGetter(JNIEnv *env, struct naturalparms *pparmtarget, int index);

void printAll(JNIEnv *, struct naturalparms[], int, FILE*);
void freeAll(JNIEnv *, FILE*, struct naturalparms*, int, char ***, int);
jobject createReturnObj(JNIEnv *env, int exit_code, char*, FILE*);

int createNaturalProcess(JNIEnv *env, struct naturalparms *parms, int length, char *error, FILE *logfile, 
    char***, int, vars_t*);
int setEnvVars(JNIEnv *env, jobjectArray envvars_o, int *length, char **vars[], FILE *logfile, char *error_msg);


JNIEXPORT jint JNICALL Java_realHTML_tomcat_connector_JNINatural_jni_1printVersion
  (JNIEnv *env, jobject parent)
{
    printf("%s\n", VERSIONSTR);
    return(0);
}

int readOutParms(JNIEnv *env, jobject jnatparms, RH4nProperties *properties, char *error_str);

JNIEXPORT jobject JNICALL Java_realHTML_tomcat_connector_JNINatural_jni_1callNatural
  (JNIEnv *env, jclass in_cls, jobject jnatparams, jobjectArray envvars_o, jobject bodyvars) {  
      char error_str[2048];
      RH4nProperties props;

      if(readOutParms(env, jnatparams, &props, error_str) != 0) {
          printf("Error: [%s]\n", error_str);
      } else {
          printf("OK\n");
      }
      return(NULL);
  }

#if 0
JNIEXPORT jobject JNICALL Java_realHTML_tomcat_connector_JNINatural_jni_1callNatural
  (JNIEnv *env, jclass in_cls, jobject jnatparams, jobjectArray envvars_o, jobject bodyvars)
{
    const char *reg_type;
    jclass jrh4nparams;
    jfieldID jfid;
    jobject jobj;
    jobjectArray jarr;
    struct naturalparms *target;
    int i, rc = 0, envvars_len = 0;
    char error_msg[2084],
         filename[10+NNI_LEN_LIBRARY+NNI_LEN_MEMBER],
         *natprogram, *natlibrary;
    char **envvars = NULL;
    vars_t *var_anker = NULL;
    

    

    struct naturalparms params[] = 
    {
        {"keys", "[Ljava/lang/String;", NULL, NULL, -1, arrayHandle, arrayGetter},
        {"vals", "[Ljava/lang/String;", NULL, NULL, -1, arrayHandle, arrayGetter},
        {"reg_type", "Ljava/lang/String;", NULL, NULL, -1, stringHandle, stringGetter},
        {"nat_library", "Ljava/lang/String;", NULL, NULL, -1, stringHandle, stringGetter},
        {"nat_program", "Ljava/lang/String;", NULL, NULL, -1, stringHandle, stringGetter},
        {"natparams", "Ljava/lang/String;", NULL, NULL, -1, stringHandle, stringGetter},
        {"tmp_file", "Ljava/lang/String;", NULL, NULL, -1, stringHandle, stringGetter},
        {"settings", "Ljava/lang/String;", NULL, NULL, -1, stringHandle, stringGetter},
        {"debug", "Ljava/lang/String;", NULL, NULL, -1, stringHandle, stringGetter}
    };

    int parm_length = sizeof(params)/sizeof(struct naturalparms);


    FILE *logfile = NULL;

    //Get reference to the RH4NParams Class
    jrh4nparams = (*env)->GetObjectClass(env, jnatparams);

    for(i=0; i < parm_length; i++)
    {
        jfid = (*env)->GetFieldID(env, jrh4nparams, params[i].jname,
                params[i].jtype);
        if((params[i].target = (*env)->GetObjectField(env, jnatparams, jfid)) == NULL)
        {
            sprintf(error_msg, "Error while proccessing field [%s]: Field is NULL", params[i].jname);
            return(createReturnObj(env, -1, error_msg, logfile));
        }
        if(params[i].handler(env, jrh4nparams, &params[i]) < 0)
        {
            sprintf(error_msg, "Error while proccessing field [%s]: unkown", params[i].jname);
            return(createReturnObj(env, -2, error_msg, logfile));
        }
    }

    target = getParmByName(params, parm_length, "debug");

    if(strcmp(target->getter(env, target, 0), "true") == 0)
    {
        target = getParmByName(params, parm_length, "nat_library");
        natlibrary = target->getter(env, target, 0);

        target = getParmByName(params, parm_length, "nat_program");
        natprogram = target->getter(env, target, 0);

        sprintf(filename, "/tmp/%s_%s.log", natlibrary, natprogram);
        if((logfile = fopen(filename, "w")) == NULL)
        {
            freeAll(env, logfile, params, parm_length, &envvars, envvars_len);
            fclose(logfile);
            return(createReturnObj(env, -3, strerror(errno), logfile));
        }
    }

    if(setEnvVars(env, envvars_o, &envvars_len, &envvars, logfile, error_msg) < 0) {
        freeAll(env, logfile, params, parm_length, &envvars, envvars_len);
        return(createReturnObj(env, -4, error_msg, logfile));
    }

    if(bodyvars != NULL) {
        if(getVarlist(env, bodyvars, &var_anker, error_msg) < 0) {
            return(createReturnObj(env, -5, error_msg, logfile));
        }
        printAll(env, params, parm_length, logfile);
    } else {
        var_anker = NULL;
    }

    createNaturalProcess(env, params, parm_length, error_msg, logfile, &envvars, envvars_len, var_anker);

    debugFileprint(logfile, "Freeing Java res...\n");
    freeAll(env, logfile, params, parm_length, &envvars, envvars_len);
    debugFileprint(logfile, "...Done\n");

    fclose(logfile);

    return(createReturnObj(env, 0, " ", logfile));
}

#endif

int createNaturalProcess(JNIEnv *env, struct naturalparms *parms, int length, 
    char *error, FILE *logfile, char ***envvars, int envvars_len, vars_t *bodyvars)
{
    pid_t natpid = 0, ret_pid = 0;
    int status = 0, rc = 0;

    fflush(logfile);

    natpid = fork();
    if(natpid == 0)
    {
        if((rc = callNatural(env, parms, length, error, logfile, envvars, envvars_len, bodyvars)) != 0)
        {
            //freeAll(env, logfile);
            //return(createReturnObj(env, rc, error_msg, logfile));
        }
        fflush(logfile);
        fclose(logfile);
        exit(rc);
    }
    debugFileprint(logfile, "Waiting for PID: [%d]....\n", natpid);
    ret_pid = waitpid(natpid, &status, 0);
    debugFileprint(logfile, "...PID [%d] exited\n", ret_pid);
    return(status);
}

jobject createReturnObj(JNIEnv *env, int exit_code, char *error_msg, FILE* logfile)
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

void printAll(JNIEnv *env, struct naturalparms params[], int length, FILE *logfile)
{
    int i,x;
    for(i=0; i < length; i++)
    {
        debugFileprint(logfile, "Name: [%s]\n", params[i].jname);
        if(params[i].array_length != -1)
        {
            for(x=0; x < params[i].array_length; x++)
            {
                debugFileprint(logfile, "\t%.2d [%s]\n", x, (char*)params[i].getter(env, &params[i], x));
            }
        }
        else
            debugFileprint(logfile, "\t [%s]\n", (char*)params[i].getter(env, &params[i], 0));
    }
}

void freeAll(JNIEnv *env, FILE *logfile, struct naturalparms *parms, int parms_length, char **envvars[], int envvars_len)
{
    int i,x;
    jstring jstr;
    jobjectArray jarr;

    if((*envvars) != NULL) {
        for(i=0; i < envvars_len; i++) {
            if((*envvars)[i] != NULL)
                free((*envvars)[i]);
        }
        free((*envvars));
    }

    for(i=0; i < parms_length; i++)
    {
        debugFileprint(logfile, "\tFreeing [%s]\n", parms[i].jname);
        if(parms[i].array_length != -1)
        {
            jarr = (jobjectArray)parms[i].target;
            for(x=0; x < parms[i].array_length; x++)
            {
                jstr = (jstring)(*env)->GetObjectArrayElement(env, jarr, x);
                (*env)->ReleaseStringUTFChars(env, jstr, ((char**)parms[i].value)[x]);
            }
            //free(params[i].value);
        }
        else
        {
            (*env)->ReleaseStringUTFChars(env, (jstring)parms[i].target, (char*)parms[i].value);
        }
    }
}


int stringHandle(JNIEnv *env, jobject target, struct naturalparms *pparmtarget)
{
    jstring jstr = (jstring)pparmtarget->target;

    if((pparmtarget->value = (*env)->GetStringUTFChars(env, jstr, NULL)) == NULL)
    {
        return(-1);
    }
    return(0);
}

void *stringGetter(JNIEnv *env, struct naturalparms *pparmtarget, int index)
{
    if(pparmtarget->array_length != -1)
        return(NULL);
    return(pparmtarget->value);
}


int arrayHandle(JNIEnv *env, jobject target, struct naturalparms *pparmtarget)
{
    jobjectArray arrtarget = (jobjectArray)pparmtarget->target;
    jstring jstr;
    int i;

    pparmtarget->array_length =  (*env)->GetArrayLength(env, arrtarget);

    pparmtarget->value = malloc(sizeof(char*)*pparmtarget->array_length);

    for(i=0; i < pparmtarget->array_length; i++)
    {
        jstr = (jstring)(*env)->GetObjectArrayElement(env, arrtarget, i);
        ((const char**)pparmtarget->value)[i] = (*env)->GetStringUTFChars(env, jstr, NULL);
    }

    return(0);
}

void *arrayGetter(JNIEnv *env, struct naturalparms *pparmtarget, int index)
{
    if(index >= pparmtarget->array_length || pparmtarget->array_length == -1) 
    {
        return(NULL);
    }

    return(((char**)pparmtarget->value)[index]);
}

void debugFileprint(FILE *logfile, char *format, ...)
{
    va_list args;
    va_start(args, format);

    if(logfile == NULL)
    {
        vprintf(format, args);
    }
    else
    {
        vfprintf(logfile, format, args);
    }
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

int setEnvVars(JNIEnv *env, jobjectArray envvars_o, int *envvar_len, char **vars[], FILE *logfile, char *error_msg) {
    jsize array_len = (*env)->GetArrayLength(env, envvars_o), name_len = 0, value_len = 0;
    jfieldID name_id, value_id, append_id;
    jclass environ_class, boolean_class;
    jobject array_entry, name_field, value_field, append_field;
    jboolean j_append; 
    jmethodID getboolean_id;

    int i = 0, complete_len = 0;
    char *cur_env_value = NULL;
    const char *java_string, *java_env_name;

    (*vars) = malloc(sizeof(char*)*array_len);
    memset(*vars, 0x00, sizeof(char*)*array_len);

    *envvar_len = array_len;

    if((environ_class = (*env)->FindClass(env, "realHTML/tomcat/connector/Environ")) == NULL) {
        strcpy(error_msg, "Find Class on realHTML/tomcat/connector/Environ returned null");
        free(*vars);
        *vars = NULL;
        return(-1);
    }

    /*if((boolean_class = (*env)->FindClass(env, "java/lang/Boolean")) == NULL) {
        strcpy(error_msg, "Find Class on java/lang/Boolean returned null");
        free(*vars);
        *vars = NULL;
        return(-1);
    }

    if((getboolean_id = (*env)->GetMethodID(env, boolean_class, "booleanValue", "()Z;")) == NULL) {
        strcpy(error_msg, "Could not find method booleanValue in class Boolean");
        free(*vars);
        *vars = NULL;
        return(-1);
    }*/

    //Get field IDs
    if((name_id = (*env)->GetFieldID(env, environ_class, "name", "Ljava/lang/String;")) == NULL) {
        strcpy(error_msg, "GetFieldID on name returned null");
        free(*vars);
        *vars = NULL;
        return(-1);
    }
    if((value_id = (*env)->GetFieldID(env, environ_class, "value", "Ljava/lang/String;")) == NULL) {
        strcpy(error_msg, "GetFieldID on value returned null");
        free(*vars);
        *vars = NULL;
        return(-1);
    }
    if((append_id = (*env)->GetFieldID(env, environ_class, "append", "Z")) == NULL) {
        strcpy(error_msg, "GetFieldID on append returned null");
        free(*vars);
        *vars = NULL;
        return(-1);
    }

    printf("Got %d vars to set\n", array_len);

    for(; i < array_len; i++) {
        if((array_entry = (*env)->GetObjectArrayElement(env, envvars_o, i)) == NULL) {
            sprintf(error_msg, "Envvar array element %d retured null", i);
            return(-1);
        }

        //get name length
        if((name_field = (*env)->GetObjectField(env, array_entry, name_id)) == NULL) {
            sprintf(error_msg, "Name field on position %d returned null", i);
            return(-1);
        }
        name_len = (*env)->GetStringLength(env, (jstring)name_field);

        //get the name value
        java_env_name = (*env)->GetStringUTFChars(env, (jstring)name_field, NULL);

        //get value length
        if((value_field = (*env)->GetObjectField(env, array_entry, value_id)) == NULL) {
            sprintf(error_msg, "Value field on position %d returned null", i);
            (*env)->ReleaseStringUTFChars(env, (jstring)name_field, java_env_name);
            return(-1);
        }
        value_len = (*env)->GetStringLength(env, (jstring)value_field);

        //calculate the complete length. The plus two is for th equals char and the null at the end
        complete_len = name_len + value_len + 2;

        //Check if the vars gets appended;
        j_append = (*env)->GetBooleanField(env, array_entry, append_id);

        if(j_append == JNI_TRUE) {
            cur_env_value = getenv(java_env_name);
            if(cur_env_value == NULL) {
                j_append = JNI_FALSE;
            } else {
                complete_len += strlen(cur_env_value);
            }
        }


        //allocate space for the complete variable (name=value)
        (*vars)[i] = malloc(sizeof(char)*complete_len);

        //put the env var name at the beginning of your complete string
        strcpy((*vars)[i], java_env_name);
        (*env)->ReleaseStringUTFChars(env, (jstring)name_field, java_env_name);

        strcat((*vars)[i], "=");

        //If the variable should get appended, copy in the original value
        if(j_append == JNI_TRUE) {
            strcat((*vars)[i], cur_env_value);
        }

        //get new var value and append your complete string
        java_string = (*env)->GetStringUTFChars(env, (jstring)value_field, NULL);
        strcat((*vars)[i], java_string);
        (*env)->ReleaseStringUTFChars(env, (jstring)value_field, java_string);

        //Write the var to the enviroment
        //putenv((*vars)[i]);

        printf("Setting env: [%s]\n", (*vars)[i]);

    }

}
