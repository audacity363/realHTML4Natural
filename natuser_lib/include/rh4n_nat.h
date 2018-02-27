#ifndef RH4NATUSER
#define RH4NNATUSER

#define RH4NNATPROPPOS 0
#define RH4NNATFORMATPOS 1

typedef struct {
    void *sharedlibrary;
    pnni_611_functions nnifuncs;
    RH4nProperties *props;
    char *formatstr;
} RH4nNatInit_t;

struct RH4nNatLDAInfos{
    char struct_name[NNI_LEN_NAME+1];
    char library[NNI_LEN_LIBRARY+1];
    char ldaname[NNI_LEN_MEMBER+1];
};

int rh4nnatInit(void*, RH4nNatInit_t*, char*);
int rh4nnatGetParameter(pnni_611_functions, void*, RH4nProperties**, char**, char*);
int rh4nnatParseFormatStr(char*, struct RH4nNatLDAInfos*, RH4nProperties*);
int rh4nnatStartVariableReadout(WORD, void*, pnni_611_functions, RH4nVarList*, RH4nProperties*, char*);

#endif
