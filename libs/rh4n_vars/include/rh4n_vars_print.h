#ifndef RH4NVARSPRINT
#define RH4NVARSPRINT
typedef struct {
    int vartype;
    void (*printfunction)(RH4nVarObj*, RH4nProperties*, FILE*);
} RH4nVarPrint;

void rh4nvarPrintList(RH4nVarList*, RH4nProperties*);
int rh4nvarPrintJSONToFile(RH4nVarList*, char*, RH4nProperties*);
void rh4nvarPrintFork(RH4nVarEntry_t*, int, int, RH4nProperties*);
void rh4nvarPrintJSONFork(RH4nVarEntry_t*, int, int, RH4nProperties*, FILE*);
void rh4nvarPrintVar(RH4nVarObj*, RH4nProperties*, FILE*);
void rh4nvarPrintString(RH4nVarObj*, RH4nProperties*, FILE*);
void rh4nvarPrintUString(RH4nVarObj*, RH4nProperties*, FILE*);
void rh4nvarPrintBool(RH4nVarObj*, RH4nProperties*, FILE*);
void rh4nvarPrintInt(RH4nVarObj*, RH4nProperties*, FILE*);
void rh4nvarPrintFloat(RH4nVarObj*, RH4nProperties*, FILE*);
void rh4nvarPrintJSONArray(RH4nVarObj*, int, int, RH4nProperties*, FILE*);
void rh4nvarPrintJSONArrayDim(RH4nVarObj*, int, int, RH4nProperties*, FILE*);
bool checkArrayGroup(RH4nVarEntry_t*);
void rh4nvarPrintGroupJSONArray(RH4nVarEntry_t*, int, int, RH4nProperties*, FILE*);
void rh4nvarPrintGroup1DJSONArray(RH4nVarEntry_t*, int, int, RH4nProperties*, int*, FILE*);
void rh4nvarPrintGroup2DJSONArray(RH4nVarEntry_t*, int, int, RH4nProperties*, int*, FILE*);
void rh4nvarPrintGroup3DJSONArray(RH4nVarEntry_t*, int, int, RH4nProperties*, int*, FILE*);
char *rh4nvarPrintGetTypeString(int);

#define RH4NVARPRINTMINIJSON 1
#define RH4NVARPRINTBEAUTIJSON 2

#define RH4NVARPRINTNL(mode) (mode == RH4NVARPRINTBEAUTIJSON ? '\n' : '')

#endif
