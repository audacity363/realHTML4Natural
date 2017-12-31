#ifndef RH4N_JSONUTILS
#define RH4N_JSONUTILS

typedef struct llclassinfo {
    jclass llclass;
    jmethodID sizeID;
    jmethodID getID;
} LLClassInfo;

GeneralInfos *getFieldIDs(JNIEnv *env);
jint getJSONVarType(JNIEnv *env, GeneralInfos *infos, jobject curptr);
void printVarType(jint vartype);
void _printTabs(int level);
LLClassInfo *getLLClassInfos(JNIEnv *env);

#endif