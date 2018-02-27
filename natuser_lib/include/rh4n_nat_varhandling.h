#ifndef RH4NNATUSER_VARHANDLING
#define RH4NNATUSER_VARHANDLING

typedef struct {
    void *parmhandle;
    int parmindex;
    struct parameter_description *desc;
    char *tmpname;
    pnni_611_functions nnifuncs;
    RH4nVarList *varlist;
    RH4nProperties *props;
    char *errorstr;
} RH4nNatVarHandleParms;

struct RH4nNatVarHandling {
    char vartype;
    int (*callback)(RH4nNatVarHandleParms*);
};

//Var handling
//String
int rh4nnatStringHandler(RH4nNatVarHandleParms*);
int rh4nnatStringArrayHandler(RH4nNatVarHandleParms*);
int rh4nnatSaveStringArrayEntry(RH4nNatVarHandleParms*,int*);
//Integer
int rh4nnatIntegerHandler(RH4nNatVarHandleParms*);
int rh4nnatIntegerArrayHandler(RH4nNatVarHandleParms*);
int rh4nnatSaveIntegerArrayEntry(RH4nNatVarHandleParms*,int*);
//Boolean
int rh4nnatBoolHandler(RH4nNatVarHandleParms*);
int rh4nnatBoolArrayHandler(RH4nNatVarHandleParms*);
int rh4nnatSaveBoolArrayEntry(RH4nNatVarHandleParms*,int*);
//Boolean
int rh4nnatFloatHandler(RH4nNatVarHandleParms*);
int rh4nnatFloatArrayHandler(RH4nNatVarHandleParms*);
int rh4nnatSaveFloatArrayEntry(RH4nNatVarHandleParms*,int*);
double rh4nnatConvertFloat(float);
//DateTime
int rh4nnatDateTimeHandler(RH4nNatVarHandleParms*);
int rh4nnatDateTimeArrayHandler(RH4nNatVarHandleParms*);
int rh4nnatSaveDateTimeArrayEntry(RH4nNatVarHandleParms*,int*);
//Numeric/Packed
int rh4nnatNumericPackedHandler(RH4nNatVarHandleParms*);
int rh4nnatNumericPackedArrayHandler(RH4nNatVarHandleParms*);
int rh4nnatSaveNumericPackedArrayEntry(RH4nNatVarHandleParms*,int*);
//UString
int rh4nnatUnicodeHandler(RH4nNatVarHandleParms*);
int rh4nnatUnicodeArrayHandler(RH4nNatVarHandleParms*);
int rh4nnatSaveUnicodeArrayEntry(RH4nNatVarHandleParms*,int*);
void rh4nnatConvert2BTo4BString(char*, wchar_t*, int);
#endif
