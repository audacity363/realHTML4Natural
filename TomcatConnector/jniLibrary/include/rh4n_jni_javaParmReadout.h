#ifndef RH4NJNIPARMREADOUT
#define RH4NJNIPARMREADOUT

//Public functions
int rh4nReadOutParms(JNIEnv*, jobject, RH4nProperties*, char*);
void rh4nPrintPropertiesStruct(RH4nProperties*);
void rh4nFreePropertiesStruct(RH4nProperties*);
void rh4nInitPropertiesStruct(RH4nProperties*);

//Private functions
struct javaParm {
    char *jname;
    char *jtype;
    int (*handler)(JNIEnv*, const char*, RH4nProperties*, char*);
};

int rh4nParmReqTypeHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmNatLibraryHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmNatProgramHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmNatParmsHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmOutputfileHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmLoglevelHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmNatSrcPathHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4NUrlVariableHandler(JNIEnv*, jclass, jobject, RH4nProperties*, char*);
int rh4nParmLogPathHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmErrorRepresentationHandler(JNIEnv*, const char*, RH4nProperties*, char*);
int rh4nParmUsernameHandler(JNIEnv*, const char*, RH4nProperties*, char*);
#endif
