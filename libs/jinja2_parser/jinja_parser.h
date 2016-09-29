/**
 * @file jinja_parser.h
 * @brief Standard Headerfile fuer den jinja parser
 */

#define DEFAULT_BLOCK_HEIGHT 200
#define DEFAULT_LINE_LENGTH 2024
#define DEFAULT_BLOCK_LENGTH (DEFAULT_BLOCK_HEIGHT*DEFAULT_LINE_LENGTH)


struct macro_definition {
    char name[1024];
    char value[DEFAULT_BLOCK_LENGTH];
    int aguments;
    char aguments_name[1024][1024];
    int default_value[1024];
    char default_values[1024][2048];
    struct macro_definition *next;
};

typedef struct {
    char cur_name[1024];
    char macro_buff[DEFAULT_BLOCK_LENGTH];
    int aguments;
    char aguments_name[1024][1024];
    int default_value[1024];
    char default_values[1024][2048];
    struct macro_definition *anker;
} macros;

int start_jinjaparser(struct variables *anker, char *outputfile,
                      char *templatefile, char *error_str, int *error_zeile);

int parse_line(struct variables *anker, macros *macro_anker, char *line, FILE *p_output,
               char *cmd_buff, int *just_save, int *in_for, int *in_if,
               char *error_str);
