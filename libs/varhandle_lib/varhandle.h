/**
 * @file varhandle.h
 */

#define STRING 2
#define STRINGARRAY 3
#define TWO_DSTRINGARRAY 5
#define THREE_DSTRINGARRAY 5

#define INT 1
#define INTARRAY 4
#define TWO_DINTARRAY 6
#define THREE_DINTARRAY 6

#define U_STRING 7
#define U_STRINGARRAY 8
#define U_TWO_DSTRINGARRAY 9
#define U_THREE_DSTRINGARRAY 10

#define ARGS 1

#define NAME_LENGTH 200

typedef int bool;

struct variables{
    char name[NAME_LENGTH];
    int length;
    int full_length;
    int x_length;
    int y_length;
    int z_length;
    int type;
    int array;
    void *data;
    struct variables *next; //Pointer to next Variable in Stack
};

struct args {
        char *cargs;
        int iargs;
        int type;
};

extern char varhandle_error_str[2048];

int getVarType(struct variables *anker, char *name);
int getArrayLength(struct variables *anker, char *name, int *x, int *y);
int deleteVar(struct variables *anker, char *name);

struct variables *searchVar(struct variables *anker, char *name);


char* getStringValue(struct variables *anker, char *name);
char *getStringValuefromArray(struct variables *anker, char *name, int index);
char *getStringValuefrom2DArray(struct variables *anker, char *name, int x, int y);
void newStringVar(struct variables *anker, char *name, char *value);
void newStringArray(struct variables *anker, char *name, char *initvalue);
void new2DStringArray(struct variables *anker, char *name, int x_length, int y_length);
void appendStringArray(struct variables *anker, char *name, char *value);
int editStringVar(struct variables *anker, char *name, char *value);
int editStringVarArray(struct variables *anker, char *name, char *value, int x);
int editStringVar2DArray(struct variables *anker, char *name, char *value, int x, int y);
char *getStringValuefrom2DArray(struct variables *anker, char *name, int x, int y);

//-------------------Integer-------------------
int newIntVar(struct variables *anker, char *name, int value);
int getIntValue(struct variables *anker, char *name, int *value);
int editIntVar(struct variables *anker, char *name, int value);

int newIntArray(struct variables *anker, char *name, int x_length);
int getIntValuefromArray(struct variables *anker, char *name, int index, int *value);
int editIntVarArray(struct variables *anker, char *name, int value, int x);

int new2DIntArray(struct variables *anker, char *name, int x_length, int y_length);
int getIntValuefrom2DArray(struct variables *anker, char *name, int x, int y, int *value);
int editIntVar2DArray(struct variables *anker, char *name, int value, int x, int y);
//---------------------------------------------

//-------------------Unicode-------------------
int newUStringVar(struct variables *anker, char *name, wchar_t *value, int length);
int editUStringVar(struct variables *anker, char *name, wchar_t *value);
wchar_t* getUStringVal(struct variables *anker, char *name);

int newUStringArray(struct variables *anker, char *name, int var_length, int array_length);
int editUStringArray(struct variables *anker, char *name, wchar_t *value, int x);
wchar_t *getUStringArrayVal(struct variables *anker, char *name, int x);

int newU2DStringArray(struct variables *anker, char *name, int var_length, int x_length, int y_length);
int editU2DStringArray(struct variables *anker, char *name, wchar_t *value, int x, int y);
wchar_t *getUString2DArrayVal(struct variables *anker, char *name, int x, int y);

int newU3DStringArray(struct variables *anker, char *name, int var_length, int x_length, int y_length, int z_length);
int editU3DStringArray(struct variables *anker, char *name, wchar_t *value, int x, int y, int z);
wchar_t *getUString3DArrayVal(struct variables *anker, char *name, int x, int y, int z);
//---------------------------------------------

wchar_t *generateStringValuefromArray(struct variables *ankerchar, char *name);
void printArrayfromPtrtoFile(struct variables *ptr, FILE *output);
void printArrayfromPtr(struct variables *ptr);
struct variables* createTmpArrayOut2DArray(struct variables *anker, char *name,
                                           int x);
struct variables* getVarPointer(struct variables *anker, char *name);
void printArray(struct variables *anker, char *name, bool showname,
                    char *returnstring);
void printVars(struct variables *anker);
void printVar(struct variables *anker, char *name);

int getVarLength(struct variables *anker, char *name);


