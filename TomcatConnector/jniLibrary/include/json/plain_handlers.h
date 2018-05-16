#ifndef RH4N_JSONPLAINHANDLERS
#define RH4N_JSONPLAINHANDLERS

int handleIntEntry(JNIEnv*, HandlerArgs, jobject, int[3]);
int handleFloatEntry(JNIEnv*, HandlerArgs, jobject, int[3]);
int handleBooleanEntry(JNIEnv*, HandlerArgs, jobject, int[3]);
int handleStringEntry(JNIEnv*, HandlerArgs, jobject, int[3]);

#endif
