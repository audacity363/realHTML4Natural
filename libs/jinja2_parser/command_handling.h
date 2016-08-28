/**
 * @file command_handling.h
 */
#define ENDFOR_CMD 2
#define FOR_CMD 1
#define ENDIF_CMD 4
#define IF_CMD 3
#define PRINTVARS_CMD 5
#define ELSE_CMD 6
#define CONTINUE_CMD 7
#define BREAK_CMD 8
#define IMPORT_CMD 9
#define NEWMACRO_CMD 10
#define ENDMACRO_CMD 11
#define MACRO_CMD 12
#define TYPEOF_CMD 13

int searchCommand(char *line, macros *anker);
int start_for(struct variables *anker, char *cmd_buff, FILE *p_output,
               macros *macros_anker, char *error_str);
int start_if(struct variables *anker, char *cmd_buff, FILE *p_output,
               macros *macros_anker, char *error_str);
int start_import(struct variables *anker, char *line, FILE *p_output,
                 macros *macros_anker, char *error_str);
