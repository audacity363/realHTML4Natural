#ifndef RH4NENVIRON
#define RH4NENVIRON

//Public
typedef struct {
    int length;
    char **variables;
} RH4NEnvirons;

void rh4nEnvironInit(RH4NEnvirons*);
int rh4nEnvironNew(RH4NEnvirons*, const char*, const char*, bool, char*, RH4nLogrule*);
int rh4nEnvironReadout(JNIEnv *, jobject, RH4NEnvirons*, char*, RH4nLogrule*);
void rh4nEnvironPrint(RH4NEnvirons*, RH4nLogrule*);
void rh4nEnvironFree(RH4NEnvirons *environs);

//Private
#define RH4N_CLASS_ENVIRON "realHTML/tomcat/connector/Environ"


#endif
