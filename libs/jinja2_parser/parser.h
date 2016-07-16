#define varopen(x,z) (x[z] == '{' && x[z+1] == '{')
#define varclose(x,z) (x[z] == '}' && x[z+1] == '}')
#define cmdopen(x,z) (x[z] == '{' && x[z+1] == '%')
#define cmdclose(x,z) (x[z] == '%' && x[z+1] == '}')

#define IF "if"
#define ENDIF "end-if"
#define IMPORT "import"
#define FOR "for"
#define ENDFOR "end-for"
#define PRINTVARS "printVars()"

#define INT 1
#define STRING 2
#define STRINGARRAY 3
#define TWO_DSTRINGARRAY 5
#define INTARRAY 4
#define TWO_DINTARRAY 6

typedef struct {
    bool found_var_open;
    bool found_cmd_open;
    bool in_if;
    bool in_for;
    bool if_ret;
    int index;
    char statement[200];
    int for_length;
    int count_for;
    char *forbuffer;
    char attrbuf[200];
} parser_info;

/*struct variables{
    char name[200];
    int length;
    int x_length;
    int y_length;
    int type;
    void *data;
    struct variables *next; //Pointer to next Variable in Stack
};*/

/*struct args {
        char *cargs;
        int iargs;
        int type;
};*/

struct functions {
    char name[100];
    int vartypes[4];
    void (*fncPnt)(struct args*, struct variables*);
};


int local_jinja_parser(parser_info *status, struct variables *anker, char *line,
           int length);
