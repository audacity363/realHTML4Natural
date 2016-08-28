/**
 * @file file_handling.h
 */

#define LINELENGTH 1024

FILE *openOutputFile(char *filename, char *error_str);
int closeOutputFile(FILE *p_output, char *error_str);
FILE *openTemplateFile(char *filename, char *error_str);
char *getLinefromTemplate(FILE *p_template, char *error_str);
int  closeTemplateFile(FILE *p_template, char *error_str);
