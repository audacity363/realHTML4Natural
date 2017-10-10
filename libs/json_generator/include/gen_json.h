#ifndef RH4N_GENJSON
#define RH4N_GENJSON

#ifdef DEV_OUTPUT
#define DEV_PRINT(x) printf x
#else
#define DEV_PRINT(x) do {} while(0)
#endif

int startJSONgenerator(vars_t *var_anker, char *output_path, FILE *logfile);
bool checkGroupArray(vars_t *grp);
int printGroupArray(vars_t *grp, FILE *output);
int getGroupArrayLength(vars_t *grp);
void printEntry(vars_t *var, int x, int y, int z, FILE *output);
int printGroupEntries(vars_t *grp, FILE *output);

void J_printInteger(vars_t *var, FILE *output);
void J_print1DIntegerX(vars_t *var, int x, FILE *output);
void J_print2DIntegerXY(vars_t *var, int x, int y, FILE *output);
void J_print3DIntegerXYZ(vars_t *var, int x, int y, int z, FILE *output);

void J_printString(vars_t *var, FILE *output);
void J_print1DStringX(vars_t *var, int x, FILE *output);
void J_print2DStringXY(vars_t *var, int x, int y, FILE *output);
void J_print3DStringXYZ(vars_t *var, int x, int y, int z, FILE *output);

void J_printsingleBoolean(bool val, FILE *fp);
void J_printBool(vars_t *var, FILE *output);
void J_print1DBoolX(vars_t *var, int x, FILE *output);
void J_print2DBoolXY(vars_t *var, int x, int y, FILE *output);
void J_print3DBoolXYZ(vars_t *var, int x, int y, int z, FILE *output);

void J_printFloat(vars_t *var, FILE *output);
void J_print1DFloatX(vars_t *var, int x, FILE *output);
void J_print2DFloatXY(vars_t *var, int x, int y, FILE *output);
void J_print3DFloatXYZ(vars_t *var, int x, int y, int z, FILE *output);

void J_print1DArray(vars_t *var, FILE *output);
void J_print1DArrayX(vars_t *var, int x, FILE *output);

void J_print2DArray(vars_t * var, FILE *output);
void J_print2DArrayX(vars_t *var, int x, FILE *output);
void J_print2DArrayXY(vars_t *var, int x, int y, FILE *output);

void J_print3DArray(vars_t *var, FILE *output);
void J_print3DArrayX(vars_t *var, int x, FILE *output);
void J_print3DArrayXY(vars_t *var, int x, int y, FILE *output);
void J_print3DArrayXYZ(vars_t *var, int x, int y, int z, FILE *output);

#endif
