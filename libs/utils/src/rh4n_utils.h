#ifndef RH4NLIBUTILS
#define RH4NLIBUTILS
void RemoveSpaces(char* source);
char *StripTrailingSpaces(char *str);
int str_isdigit(char *str);
int strn_isdigit(char *str, int length);
void char_strcat(char *str, char character);
void trimSpaces(char *input);
void trimleadingSpaces(char *input);
void trimSpaces4B(wchar_t *input);
char *str_toLower(char *input);
char *str_toUpper(char *input);

#ifdef RH4N_STD
int rh4nUtilsloadSharedLibrary(RH4nProperties*, char*, void**, char*);
pnni_611_functions rh4nUtilsgetNNIFunctions(RH4nProperties*, void*, char*);
#endif
#endif
