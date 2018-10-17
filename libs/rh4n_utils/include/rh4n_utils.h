#ifndef RH4NLIBUTILS
#define RH4NLIBUTILS
void rh4nUtilsTrimSpaces(char *input);
void rh4nUtilsTrimSpaces4B(wchar_t *input);
char *str_toLower(char *input);
char *str_toUpper(char *input);
void rh4nUtilscloseSharedLibrary(void *psharedLibrary);
void convert1Bto4BString(char*, wchar_t*, int);

#ifdef RH4N_STD
int rh4nUtilsloadSharedLibrary(RH4nProperties*, char*, void**, char*);
pnni_611_functions rh4nUtilsgetNNIFunctions(RH4nProperties*, void*, char*);
#endif
#endif
