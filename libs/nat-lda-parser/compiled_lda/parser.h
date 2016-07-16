#define NUMERIC      1
#define ALPHA        2
#define INTEGER      3
#define GROUP_LEADER 4
#define REDEFINE     5
#define DYNAMIC      -1

#define ARRAY_1D     1
#define ARRAY_2D     2

#define SAG_ALPHA_CHAR   'A'
#define SAG_INTEGER_CHAR 'I'
#define SAG_NUMERIK_CHAR 'N'
#define SAG_CONTROL_CHAR 'C'
#define SAG_DYNAMIC "DYNAMIC"

#define SAG_COMMENT          1
#define SAG_DEFINE           2
#define SAG_DEFINE_CONST     3
#define SAG_GROUP_MEMBER     4
#define SAG_GROUP_HEAD       5
#define SAG_REDEFINE         6
#define SAG_REDEFINED_VALUES 7
#define SAG_INIT             8
#define SAG_PRE_INIT         9
#define SAG_DYNAMIC_DF       10


struct varnames {
    char name[200];
    int level;
    int type;
    char rename[200];
    int length; //Laenge der Variable (Vor dem Komma)
    int length_comma;
    int array; //true|false
    int x_length; // X Laenge des Arrays
    int y_length; // Y Laenge des Arrays (Nur bei 2D)
    struct varnames *nextlevel;
    struct varnames *next;
    struct varnames *prev;
};

struct status_p {
    int akt_level;
    bool foundpage_grp;
    struct varnames *cur_dataset;
};

void natone_parser(struct varnames *anker, struct status_p *status, char *line);
void unix_parser(struct varnames *anker, struct status_p *status, char *line);

struct varnames* getVarNames(char *ldaname);
void printfork(struct varnames *ptr);
void printSingleVar(struct varnames *ptr);
