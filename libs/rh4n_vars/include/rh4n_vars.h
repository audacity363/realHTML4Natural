#ifndef RH4NLIBVARS
#define RH4NLIBVARS
typedef struct {
    int type;
    int length; 
    void *value;
} RH4nVarObj;

typedef struct RH4nVarEntry_s {
    char *name;
    RH4nVarObj var;
    struct RH4nVarEntry_s *prev;
    struct RH4nVarEntry_s *next;
    struct RH4nVarEntry_s *nextlvl;
} RH4nVarEntry_t;

typedef struct {
    RH4nVarEntry_t *anker;
} RH4nVarList;

typedef struct {
    char *name;
    RH4nVarEntry_t *var;
} RH4nVarRef;


typedef char bool;
#define true  1
#define false 0

#define RH4NVARTYPESTRING  1
#define RH4NVARTYPEUSTRING 2
#define RH4NVARTYPEINTEGER 3
#define RH4NVARTYPEFLOAT   4
#define RH4NVARTYPEBOOLEAN 5
#define RH4NVARTYPEGROUP   6
#define RH4NVARTYPEARRAY   7

#define RH4NVAR_CHECKVARLIST(list) if(!list) { return(RH4N_RET_VAR_PTR_ERR); }
#define RH4NVAR_CHECKVARANKER(list) if(!list->anker) { return(RH4N_RET_VAR_PTR_ERR); }
#define RH4NVAR_REF_INIT {NULL, NULL}
#define RH4NVAR_REF_RESET(varref) varref.name = NULL; varref.var = NULL;
#define RH4NVAR_NEW(vptr) if((vptr = malloc(sizeof(RH4nVarEntry_t))) == NULL) { return(RH4N_RET_MEMORY_ERR); }
#define RH4NVAR_ENTRY_INIT(varentry) varentry->name = NULL; \
                                     varentry->prev = NULL; \
                                     varentry->next = NULL; \
                                     varentry->nextlvl = NULL; \
                                     varentry->var.type = -1; \
                                     varentry->var.value = NULL; \
                                     varentry->var.length = -1;
#define RH4NVAR_ARRAY_INIT_TYPE(dims, dim, vartype) dims == dim ? vartype : RH4NVARTYPEARRAY



//Public
//Utils
int rh4nvarInitList(RH4nVarList*);
int rh4nvarGetRef(RH4nVarList*, char*, char*, RH4nVarRef*);
int rh4nvarExist(RH4nVarList*, char*, char*);
int rh4nvarGroupExist(RH4nVarList*, char*);
int rh4nvarGetVarType(RH4nVarList*, char*, char*, int*);
//String
int rh4nvarCreateNewString(RH4nVarList*, char*, char*, char*);
int rh4nvarGetString(RH4nVarList*, char*, char*, int, char*);
int rh4nvarGetStringLength(RH4nVarList*, char*, char*, int*);
int rh4nvarEditString(RH4nVarList*, char*, char*, char*);
int rh4nvarCreateNewStringArray(RH4nVarList*, char*, char*, int, int*);
int rh4nvarGetStringArrayEntry(RH4nVarList*, char*, char*, int*, int, char*);
int rh4nvarSetStringArrayEntry(RH4nVarList*, char*, char*, int*, char*);
int rh4nvarGetStringLengthArrayEntry(RH4nVarList*, char*, char*, int*, int*);
//UString
int rh4nvarCreateNewUString(RH4nVarList*, char*, char*, wchar_t*);
int rh4nvarGetUString(RH4nVarList*, char*, char*, int, wchar_t*);
int rh4nvarGetUStringLength(RH4nVarList*, char*, char*, int*);
int rh4nvarEditUString(RH4nVarList*, char*, char*, wchar_t*);
int rh4nvarCreateNewUStringArray(RH4nVarList*, char*, char*, int, int*);
int rh4nvarGetUStringArrayEntry(RH4nVarList*, char*, char*, int*, int, wchar_t*);
int rh4nvarSetUStringArrayEntry(RH4nVarList*, char*, char*, int*, wchar_t*);
int rh4nvarGetUStringLengthArrayEntry(RH4nVarList*, char*, char*, int*, int*);
//Bool
int rh4nvarCreateNewBool(RH4nVarList*, char*, char*, bool);
int rh4nvarEditBool(RH4nVarList*, char*, char*, bool);
int rh4nvarGetBool(RH4nVarList*, char*, char*, bool*);
int rh4nvarCreateNewBoolArray(RH4nVarList*, char*, char*, int, int*);
int rh4nvarSetBoolArrayEntry(RH4nVarList*, char*, char*, int*, bool);
int rh4nvarGetBoolArrayEntry(RH4nVarList*, char*, char*, int*, bool*);
//Integer
int rh4nvarCreateNewInt(RH4nVarList*, char*, char*, int);
int rh4nvarEditInt(RH4nVarList*, char*, char*, int);
int rh4nvarGetInt(RH4nVarList*, char*, char*, int*);
int rh4nvarCreateNewIntArray(RH4nVarList*, char*, char*, int, int*);
int rh4nvarSetIntArrayEntry(RH4nVarList*, char*, char*, int*, int);
int rh4nvarGetIntArrayEntry(RH4nVarList*, char*, char*, int*, int*);
//Float
int rh4nvarCreateNewFloat(RH4nVarList*, char*, char*, double);
int rh4nvarEditFloat(RH4nVarList*, char*, char*, double);
int rh4nvarGetFloat(RH4nVarList*, char*, char*, double*);
int rh4nvarCreateNewFloatArray(RH4nVarList*, char*, char*, int, int*);
int rh4nvarSetFloatArrayEntry(RH4nVarList*, char*, char*, int*, double);
int rh4nvarGetFloatArrayEntry(RH4nVarList*, char*, char*, int*, double*);
//Group
int rh4nvarCreateNewGroup(RH4nVarList*, char*);
int rh4nvarMoveVarToGroup(RH4nVarList*, char*, char*);
//Array
int rh4nvarGetArrayVarType(RH4nVarList*, char*, char*, int*);
//Free
void rh4nvarFreeList(RH4nVarList*);

//Private
//Utils
int rh4nvarCreatenewVariable(RH4nVarList*, char*, char*, int, RH4nVarRef*);
int rh4nvarSearchVarRef(RH4nVarEntry_t*, char*, RH4nVarRef*);
//String
int rh4nvarSetString(RH4nVarObj *target, char *value);
//String
int rh4nvarSetUString(RH4nVarObj *target, wchar_t *value);
//Bool
int rh4nvarSetBool(RH4nVarObj*, bool); 
//Integer
int rh4nvarSetInt(RH4nVarObj*, int); 
//Float
int rh4nvarSetFloat(RH4nVarObj*, double); 
//Array
int rh4nvarCheckDimLength(int, int*);
int rh4nvarInitArray(RH4nVarObj*, int, int);
int rh4nvarCreateNewArray(RH4nVarList*, char*, char*, int, int*, int);
int rh4nvarGetArrayLength(RH4nVarObj*, int*);
int rh4nvarGetArrayDimension(RH4nVarObj*, int*);
int rh4nvarGetArrayEntry(RH4nVarObj*, int[3], RH4nVarObj**);
//Free
void rh4nvarFreeArray(RH4nVarEntry_t*);
void rh4nvarFreeFork(RH4nVarEntry_t*);
#endif

