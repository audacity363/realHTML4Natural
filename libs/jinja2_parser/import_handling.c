#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"
#include "file_handling.h"
#include "jinja_parser.h"
/**
 * @file import_handling.c
 * @brief Importiert eine Datei und parst diese
 */

/**
 * @brief Parst den import Befehl, sucht diese Datei und parst diese
 *
 * @param anker Anker Punkt fuer Variablen
 * @param Buffer welcher den Import Befehl beinhaltet
 * @param p_output File pointer auf das Output file
 * @param macro_anker Anker Punkt fuer Macro Definitionen
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Error: < 0; Parser-Error: return Code von @ref parse_line; Erfolg: 0
 */
int import_handle(struct variables *anker, char *line, FILE *p_output, 
                  macros *macro_anker, char *error_str)
{
    char *begin, *end, *template, *complete_path;
    char *import_line, *cmd_buff = NULL;

    int parser_status = 0, in_for = 0, in_if = 0, l_line_nr = 0;

    FILE *import_file;

    if((begin = strchr(line, '"')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"");
        return(-1);
    }

    if((end = strchr(begin+1, '"')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"");
        return(-2);
    }
    end[0] = '\0';

    if(strlen(begin+1) == 0)
    {
        strcpy(error_str, "Syntax Error: Empty String");
        return(-3);
    }

    template = malloc(strlen(begin+1)+1);
    strcpy(template, begin+1);
    printf("Template: [%s]\n", template);

    complete_path = malloc(strlen(webserver_settings.templatepath)+strlen(template)+1);
    sprintf(complete_path, "%s/%s", webserver_settings.templatepath, template);
    printf("Complete: [%s]\n", complete_path);

    if((import_file = fopen(complete_path, "r")) == NULL)
    {
        sprintf(error_str, "Import Error: %s", strerror(errno));
        return(-4);
    }

    while(1)
    {
        if((import_line = getLinefromTemplate(import_file, error_str)) == NULL)
        {
            if(feof(import_file))
            {
                strcpy(error_str, "");
                break;
            }
            fclose(import_file);
            return(-5);
        }

        l_line_nr++;
        if(parse_line(anker, macro_anker, import_line, p_output, &cmd_buff, &parser_status, 
                      &in_for, &in_if, error_str) < 0)
        {
            fclose(import_file);
            return(-6);
        }
    }


    fclose(import_file);
    free(complete_path);
    free(template);


    return(0);
}
