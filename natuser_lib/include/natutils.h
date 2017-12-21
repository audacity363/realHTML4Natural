#ifndef RH4N_NATUTILS
#define RH$N_NATUTILS


#define NAT_LIB_NAME "libnatural.so"
#define GET_INTERFACE_FUNC "nni_get_interface"

char* OpenLib(void **shLib, char *name);
void CloseLib(void **shLib);
pnni_611_functions initNNI(void *lib);
void convert2Bto4BString(char*, wchar_t*, int);
void convert1Bto4BString(char*, wchar_t*, int);
void logError(int ret, char *outputpath, FILE *logfile);
char *getErrorMsg(int error);

#endif
