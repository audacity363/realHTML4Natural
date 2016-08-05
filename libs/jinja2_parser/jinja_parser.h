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

int parse_line(struct variables *anker, macros *macro_anker, char *line, FILE *p_output,
               char **cmd_buff, int *just_save, int *in_for, int *in_if,
               char *error_str);
