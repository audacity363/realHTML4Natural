#define NATPARMSCOUNT 7

#define RH4N_CLASS_STRING "Ljava/lang/String;"

struct naturalparms {
    char *jname;
    char *jtype;
    jobject target;
    void *value;
    int array_length;
    int (*handler)(JNIEnv*, jobject, struct naturalparms*);
    void *(*getter)(JNIEnv*, struct naturalparms*, int);
};

int callNatural(JNIEnv *env, struct naturalparms *parms, int length, char *error, FILE*, char***, int, RH4nVarList);
void debugFileprint(FILE *logfile, char *format, ...);
char *natErrno2Str(int naterrno);
struct naturalparms *getParmByName(struct naturalparms *parms, int length, char *searchname);
char *OpenLib(void **shLib, char *name);
void CloseLib(void **shLib);
int getVarlist(JNIEnv *env, jobject varlist, RH4nVarList *target, char *error_msg, RH4nLogrule*);
