//Types of the function wich specify the loop length
enum {
    ARRAY,
    RANGE
};

#define RANGE_STR "range"

//saves the start and the end value from the range() command
typedef struct {
     int start;
     int end;
} range_parms;

//saves the attributes of the array
typedef struct {
    int var_count;
    int *type;
    int *length;
    char **grp;
    char **name;
    int **array_length;
    int *index_type;
    //If a x index is specified
    int *x_index;
    //If a y index is specified
    int *y_index;
    //When a z index is specify it can't be an array so it get not saved here
} array_parms;

typedef struct {
    int for_type;
    range_parms range;
    array_parms array;

    //The index wich just count from zero
    int index;

    //The temp varname get saved here. If there are multiple they get saved too
    int var_count;
    char **varnames;
} for_status;

int for_handling(token_t *anker, status_t *stat);
int end_for_handling(token_t *anker, status_t *stat);
int parseForCMD(token_t *start, for_status *stat);
int checkIfMultipleVarnames(char *found_names, for_status *stat);
