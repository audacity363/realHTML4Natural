#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

#include "vars.h"
#include "parser.h"
#include "parser_errno.h"
#include "token_handling.h"
#include "macro.h"

#include "error_str.h"

vars_t *vars_anker;
macro_definition_t *macro_defs;
FILE *f_output;
FILE *logfile;
int parser_errno = 0;
int varhandle_error = 0;

int start(vars_t *variablen, char *template, char *output, FILE *l_logfile)
{
    FILE *f_template = NULL, *old_stdout = stdout;

    int ret = 0;

    wchar_t line[MAXLINELENGTH];

    status_t status = {-1, 0, 0, 0, 0, 0, NULL};

    setlocale(LC_ALL, "");

    logfile = l_logfile;

    if((f_template = fopen(template, "r")) == NULL)
    {
        fprintf(logfile, "Error while opening template: [%s]\n", strerror(errno));
        return(-1);
    }

    if((f_output = fopen(output, "w")) == NULL)
    {
        fprintf(logfile, "Error while opening Outputfile: [%s]\n", strerror(errno));
        return(-2);
    }

    vars_anker = variablen;

    initMacroAnker(&macro_defs);

    while(getLineFromFile(f_template, line) == true)
    {
        if((ret = parseLine(line, &status)) == EXIT)
            break;
    }

    printf("Ret: [%d]\n", ret);

    if(parser_errno != 0)
    {
        fprintf(logfile, "Error number:  [%02d]\n", parser_errno);
        fprintf(logfile, "Error message: [%s]\n", parser_error_strs[parser_errno]);
    }

#ifdef DEBUG
    listAllMacros(macro_defs);
#endif

    freeLineBuff(&status);
    freeMacros(macro_defs);

    fclose(f_template);
    fclose(f_output);

    return(0);
}

int getLineFromFile(FILE *template, wchar_t *line)
{
    if(fgetws(line, MAXLINELENGTH, template) == NULL)
    {
        if(!feof(template))
        {
            fprintf(stderr, "Error while reading template file: [%s]\n", 
                strerror(errno));
            return(-1);
        }
        return(false);
    }
    return(true);

}
