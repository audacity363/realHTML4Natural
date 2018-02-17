#ifndef RH4N_JSONHANDLING
#define RH4N_JSONHANDLING

#define JVAR_GROUP 0
#define JVAR_STRING 1 
#define JVAR_STRING1D 2
#define JVAR_STRING2D 3
#define JVAR_STRING3D 4
#define JVAR_BOOLEAN 5
#define JVAR_BOOLEAN1D 6
#define JVAR_BOOLEAN2D 7
#define JVAR_BOOLEAN3D 8
#define JVAR_INT 9
#define JVAR_INT1D 10
#define JVAR_INT2D 11
#define JVAR_INT3D 12
#define JVAR_FLOAT 13
#define JVAR_FLOAT1D 14
#define JVAR_FLOAT2D 15
#define JVAR_FLOAT3D 16
#define JVAR_UNKNOWN 17


typedef struct general_infos {
    jfieldID nextentry;
    jfieldID nextlvl;
    jfieldID name;
    jfieldID type;
    jfieldID value;
    jmethodID type_method;
    jobject anker;
    RH4nLogrule *logging;
} GeneralInfos;

typedef struct handlerargs {
    jobject curptr;
    GeneralInfos *infos;
    const char *varname;
    int level;
    jint vartype;
    const char *parent;
    vars_t *var_anker;
} HandlerArgs;


struct handlerfuncs {
    jint vartype;
    /*
     * parm 1: Java env
     * parm 2: curptr from varlist
     * parm 3: general infos
     */
    int (*handlerfunc)(JNIEnv*, HandlerArgs);
};

#endif
