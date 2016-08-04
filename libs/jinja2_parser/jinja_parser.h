struct macro_definition {
    char name[1024];
    char *value;
    struct macro_definition *next;
};

typedef struct {
    char cur_name[1024];
    char *macro_buff;
    struct macro_definition *anker;
} macros;

int start_jinjaparser(struct variables *anker, char *outputfile,
                      char *templatefile, char *error_str, int *error_zeile);
