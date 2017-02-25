enum
{
    SAG_COMMENT,
    SAG_DEFINE, 
    SAG_DEFINE_CONST,
    SAG_GROUP_MEMBER,
    SAG_GROUP_HEAD,
    SAG_REDEFINE,
    SAG_REDEFINED_VALUES,
    SAG_INIT,
    SAG_PRE_INIT,
    SAG_DYNAMIC_DF
};

#define CONTINUE 1
#define BREAK 2
#define EXIT 3

//definitions from the natni.h file from the software AG
#define TYPE_ALPHA         'A'
#define TYPE_BIN           'B'
#define TYPE_CV            'C'
#define TYPE_DATE          'D'
#define TYPE_FLOAT         'F'
#define TYPE_INT           'I'
#define TYPE_LOG           'L'
#define TYPE_NUM           'N'
#define TYPE_OBJECT        'O'
#define TYPE_PACK          'P'
#define TYPE_TIME          'T'
#define TYPE_UNICODE       'U'
#define TYPE_EMPTY         ' '

#define LEN_NAME            32

#define TYPE_UNSUPPORTED -1

#define LDA_OK 0
#define LDA_FILE_ERROR 3 
#define LDA_TYPE_NOT_SUPPORTED 4
#define LDA_UNKOWN_VAR_TYPE 5
#define LDA_UNKOWN_SAG_DEF_TYPE 6

int getVariablenType(char type);
void printfork(vars_t *anker, FILE *logfile);
