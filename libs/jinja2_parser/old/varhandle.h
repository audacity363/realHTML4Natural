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
int editIntVar(struct variables *anker, char *name, int value);

void printArrayfromPtr(struct variables *ptr);
struct variables* createTmpArrayOut2DArray(struct variables *anker, char *name,
                                           int x);
void printArray(struct variables *anker, char *name, bool showname,
                    char *returnstring);
void printVars(struct variables *anker);
void printVar(struct args word);
