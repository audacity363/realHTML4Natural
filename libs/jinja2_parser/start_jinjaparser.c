#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "standard.h"
#include "varhandle.h"
#include "file_handling.h"
#include "jinja_parser.h"
#include "command_handling.h"

/**
 * @file start_jinjaparser.c
 * @author Tom Engemann
 * @date 26.08.2016
 * @brief Startpunkt fuer den Parser
 */


/**
 * @brief parst jeweils eine ganze Zeile
 *
 * @usage Wenn eine Zeile geparst werden soll, kann diese Funktion aufgerufen
 *        werden. Sie wird auch recusive benutzt
 *
 * Wird von folgenden Funktionen benutzt:
 *
 * - @ref start_jinjaparser 
 * - @ref printMacro
 * - @ref if_handle
 * - @ref import_handle
 *
 * @param anker Anker Punkt fuer Variablen
 * @param macro_anker Anker Punkt fuer Macro Definitionen
 * @param line Die zu parsende Zeile
 * @param p_output File pointer auf das Output file
 * @param cmd_buff BUffer um mehrzeilige Befehle zu speichern
 * @param just_save Inikator dafuer ob geparst oder gespeichert werden soll
 * @param in_for Inikator dafuer ob man sich in einem FOR block befindet
 * @param in_if Inikator dafuer ob man sich in einem IF block befindet
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return 0 wenn alles OK war; < 0 wenn es einen Fehler gab
 */
int parse_line(struct variables *anker, macros *macro_anker, char *line, FILE *p_output,
               char *cmd_buff, int *just_save, int *in_for, int *in_if,
               char *error_str)
{
    char *pos_open_var, *pos_close_var;
    char *pos_open_cmd, *pos_close_cmd;
    int cmd_type, if_ret, cmd = 0;

    if((pos_open_var = strstr(line, "{{")) != NULL && *just_save == 0)
    {
        pos_open_var[0] = '\0';
        fprintf(p_output, "%s", line);

        //Variablen an den Anfang verschieben
        memmove(line, line+(strlen(line)+2), strlen(pos_open_var+1));
        if((pos_close_var = strstr(line, "}}")) == NULL)
        {
            strcpy(error_str, "Syntax Error: Missing \"}}\"");
            return(-1);
        }
        pos_close_var[0] = '\0';

        if(handle_variable(anker, line, p_output, error_str) < 0)
        {
            return(-2);
        }

        fprintf(p_output, "%s", pos_close_var+2);
    }
    else if((pos_open_cmd = strstr(line, "{%")) != NULL) 
    {
        if((pos_close_cmd = strstr(line, "%}")) == NULL)
        {
            strcpy(error_str, "Syntax Error: Missing \"%}\"");
            return(-3);
        }
        
        pos_close_cmd[0] = '\0';
        fprintf(p_output, "%.*s", pos_open_cmd-line, line);
        memmove(line, pos_open_cmd+2, strlen(pos_open_cmd)+1);
        switch(searchCommand(line, macro_anker))
        {
            case -1:
                strcpy(error_str, "Name Error: Unkown Function");
                return(-4);
            //Forschleife:
            case FOR_CMD:
                if(*just_save == 2)
                {
                    cmd = 1;
                    goto save_macro;
                }
                *in_for = *in_for + 1;
                if(*just_save != 0)
                {
                    strcat(cmd_buff, "{%");
                    strcat(cmd_buff, line);
                    strcat(cmd_buff, "%}\n");
                    return(0);
                }
                bzero(cmd_buff, 2024*100);

                *just_save = 1;
                strcpy(cmd_buff, line);
                strcat(cmd_buff, "\n");
                return(0);
            //Forschleife ist geschlossen worden 
            case ENDFOR_CMD:
                if(*just_save == 2)
                {
                    cmd = 1;
                    goto save_macro;
                }
                if(*in_for-1 != 0)
                {
                    *in_for = *in_for -1; 
                    strcat(cmd_buff, "{%");
                    strcat(cmd_buff, line);
                    strcat(cmd_buff, "%}\n");
                    return(0);
                }
                if(*in_if != 0)
                {
                    *in_for = *in_for -1; 
                    strcat(cmd_buff, "{%");
                    strcat(cmd_buff, line);
                    strcat(cmd_buff, "%}\n");
                    return(0);
                }
                *in_for = *in_for - 1;
                if(start_for(anker, cmd_buff, p_output, macro_anker,
                             error_str) < 0)
                {
                    return(-1);
                }
                *just_save = 0;
                return(0);
            //If abfrage
            case IF_CMD:
                if(*just_save == 2)
                {
                    cmd = 1;
                    goto save_macro;
                }
                *in_if = *in_if + 1;
                if(*just_save != 0)
                {
                    strcat(cmd_buff, "{%");
                    strcat(cmd_buff, line);
                    strcat(cmd_buff, "%}\n");
                    return(0);
                }
                *just_save = 1;
                bzero(cmd_buff, 2024*100);
                strcpy(cmd_buff, line);
                strcat(cmd_buff, "\n");
                return(0);
            //Close if
            case ENDIF_CMD:
                if(*just_save == 2)
                {
                    cmd = 1;
                    goto save_macro;
                }
                if(*in_if-1 != 0)
                {
                    *in_if = *in_if -1;
                    strcat(cmd_buff, "{%");
                    strcat(cmd_buff, line);
                    strcat(cmd_buff, "%}\n");
                    return(0);

                }
                if(*in_for != 0)
                {
                    *in_if = *in_if -1;
                    strcat(cmd_buff, "{%");
                    strcat(cmd_buff, line);
                    strcat(cmd_buff, "%}\n");
                    return(0);

                }
                *in_if = *in_if -1;
                if((if_ret = start_if(anker, cmd_buff, p_output, macro_anker, error_str)) < 0)
                {
                    return(-1);
                }
                *just_save = 0;
                return(if_ret);
            //Found printVars()
            case PRINTVARS_CMD:
                if(*just_save == 2)
                {
                    cmd = 1;
                    goto save_macro;
                }
                if(*just_save == 2)
                    goto save_macro;
                printVarstoFile(anker, p_output);
                return(0);
            case TYPEOF_CMD:
                if(*just_save == 2)
                {
                    cmd = 1;
                    goto save_macro;
                }
                if(*just_save == 2)
                    goto save_macro;

                typeof_cmd(anker, macro_anker, line, p_output, error_str);
                fprintf(p_output, "%s", pos_close_cmd+2);
                return(0);
            case IMPORT_CMD:
                if(*just_save == 2)
                {
                    cmd = 1;
                    goto save_macro;
                }
                return(start_import(anker, line, p_output, macro_anker, error_str));
            case NEWMACRO_CMD:
                if(getMacroName(macro_anker, line) < 0)
                    return(-2);
                *just_save = 2;
                //macro_anker->macro_buff = malloc(sizeof(char));
                bzero(macro_anker->macro_buff, sizeof(macro_anker->macro_buff));
                return(0);
            case ENDMACRO_CMD:
                *just_save = 0;
                saveMacro(macro_anker);
                return(0);
            case MACRO_CMD:
                if(printMacro(macro_anker, line, p_output, 
                   anker, error_str) < 0)
                    return(-3);
                return(0);
            //Else
            case ELSE_CMD:
            //continue
            case CONTINUE_CMD:
            //break
            case BREAK_CMD:
                if(*just_save == 2)
                    goto save_macro;
                strcat(cmd_buff, "{%");
                strcat(cmd_buff, line);
                strcat(cmd_buff, "%}\n");
                return(0);
            default:
                strcpy(error_str, "Internal Error: searchCommand() return something unkown");
                return(-5);
        }
    }
    else if(*just_save > 0)
    {
        if(*just_save == 1)
        {
            strcat(cmd_buff, line);
        }
        else if(*just_save == 2)
        {
save_macro:
            if(cmd == 1)
            {
                //macro_anker->macro_buff = realloc(macro_anker->macro_buff, strlen(macro_anker->macro_buff)+strlen(line)+6);
                strcat(macro_anker->macro_buff, "{%");
                strcat(macro_anker->macro_buff, line);
                strcat(macro_anker->macro_buff, "%}");
            }
            else
            {
                //macro_anker->macro_buff = realloc(macro_anker->macro_buff, strlen(macro_anker->macro_buff)+strlen(line)+1);
                strcat(macro_anker->macro_buff, line);
            }

        }

    }
    else
    {
        fprintf(p_output, "%s", line);
    }

    return(0);
}

/**
 * @brief Schließt alle offene Filehandler
 *
 * @param p_output Filehandler fuer das Outputfile (write)
 * @param p_template Filehandler fuer das Templatefile (read)
 *
 * @return Nothing
 */
void close_jinjaparser(FILE *p_output, FILE *p_template)
{
    char error_str[2048];

    if(p_output != NULL)
        closeOutputFile(p_output, error_str);

    if(p_template != NULL)
        closeTemplateFile(p_template, error_str);
}

/**
 * @brief Eintiegspunkt des kompletten jinja parsers
 *
 * @param anker Anker Punkt fuer Variablen
 * @param outputfile kompletter Pfad zum outputfile
 * @param templatefile kompletter Pfad zum outputfile
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * @param line_nr Buffer in den die Fehler Zeilennummer geschrieben wird
 *
 * @return 0 wenn alles OK war; < 0 wenn es einen Fehler gab
 */
int start_jinjaparser(struct variables *anker, char *outputfile,
                      char *templatefile, char *error_str, int *line_nr)
{
    FILE *p_template, *p_output;
    char *line, cmd_buff[2024*100];
    int parser_status = 0, in_for = 0, in_if = 0, l_line_nr = 0;

    setlocale(LC_ALL, "");

    macros macros_anker;

    macros_anker.anker = malloc(sizeof(struct macro_definition));
    macros_anker.anker->next = NULL;
    //macros_anker.macro_buff = NULL;
    strcpy(macros_anker.anker->name, "macroanker");

    if((p_template = openTemplateFile(templatefile, error_str)) == NULL)
    {
        return(-1);
    }

    if((p_output = openOutputFile(outputfile, error_str)) == NULL)
    {
        close_jinjaparser(p_output, p_template);
        return(-2);
    }

    printVars(anker);

    while(1)
    {
        if((line = getLinefromTemplate(p_template, error_str)) == NULL)
        {
            if(feof(p_template))
            {
                strcpy(error_str, "");
                break;
            }
            close_jinjaparser(p_output, p_template);
            return(-3);
        }

        l_line_nr++;
        if(parse_line(anker, &macros_anker, line, p_output, cmd_buff, &parser_status, 
                      &in_for, &in_if, error_str) < 0)
        {
            close_jinjaparser(p_output, p_template);
            *line_nr = l_line_nr;
            return(-4);
        }
    }

    if(parser_status != 0)
    {
        return(-7);
    }
    close_jinjaparser(p_output, p_template);
    return(0);
}
