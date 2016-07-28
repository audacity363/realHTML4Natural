#define INT 1
#define STRING 2
#define STRINGARRAY 3
#define TWO_DSTRINGARRAY 5
#define INTARRAY 4
#define TWO_DINTARRAY 6

#define ARGS 1

typedef int bool;

struct variables{
    char name[200];
    int length;
    int full_length;
    int x_length;
    int y_length;
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
char *getStringValuefromArray(struct variables *anker, char *name, int index);
int deleteVar(struct variables *anker, char *name);

char* getStringValue(struct variables *anker, char *name);
char *getStringValuefromArray(struct variables *anker, char *name, int index);
char *getStringValuefrom2DArray(struct variables *anker, char *name, int x, 
                                int y);
void newStringVar(struct variables *anker, char *name, char *value);
void newStringArray(struct variables *anker, char *name, char *initvalue);
void new2DStringArray(struct variables *anker, char *name, int x_length, 
                      int y_length);
void appendStringArray(struct variables *anker, char *name, char *value);
int editStringVar(struct variables *anker, char *name, char *value);
int editStringVarArray(struct variables *anker, char *name, char *value, int x);
int editStringVar2DArray(struct variables *anker, char *name, char *value,
                         int x, int y);
char *getStringValuefrom2DArray(struct variables *anker, char *name, int x, 
                                int y);

int getIntValue(struct variables *anker, char *name);
int getIntValuefromArray(struct variables *anker, char *name, int index);
int getIntValuefrom2DArray(struct variables *anker, char *name, int x, 
                                int y, int *value);
void newIntVar(struct variables *anker, char *name, int value);
void newIntArray(struct variables *anker, char *name, int initvalue);
void appendIntArray(struct variables *anker, char *name, int value);

void new2DIntArray(struct variables *anker, char *name, int x_length,
                   int y_length);
int editIntVar(struct variables *anker, char *name, int value);
int editIntVarArray(struct variables *anker, char *name, int value, int x);
int editIntVar2DArray(struct variables *anker, char *name, int value, int x,
                      int y);

char *generateStringValuefromArray(struct variables *ankerchar, char *name);
void printArrayfromPtr(struct variables *ptr, FILE *output);
struct variables* createTmpArrayOut2DArray(struct variables *anker, char *name,
                                           int x);
struct variables* getVarPointer(struct variables *anker, char *name);
void printArray(struct variables *anker, char *name, bool showname,
                    char *returnstring);
void printVars(struct variables *anker);
void printVar(struct variables *anker, char *name);
