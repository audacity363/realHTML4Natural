#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"
#include "jinja_parser.h"
#include "command_handling.h"
/**
 * @file command_handling.c
 * @author Tom Engemann
 * @date 26.08.2016
 * @brief Startet den jeweiligen Handler fuer Befehle
 */

/**
 * @brief Such in \p line nach einem Befehl oder einem Macro
 *
 * @param line Buffer der nach einem Befehl durchsucht werden soll
 * @param macro Anker Punkt fuer macros
 *
 * @return > 0 Den gefundenen Befehl; < 0 Nichts gefunden.
 *             Konstanten sind in command_handling.h definiert
 */
int searchCommand(char *line, macros *anker)
{
    char *found_cmd;
    struct macro_definition *hptr;

    if((found_cmd = strstr(line, "end-for")) != NULL)
    {
        return(ENDFOR_CMD);
    }
    else if((found_cmd = strstr(line, "for")) != NULL)
    {
        return(FOR_CMD);
    }
    else if((found_cmd = strstr(line, "end-if")) != NULL)
    {
        return(ENDIF_CMD);
    }
    else if((found_cmd = strstr(line, "if")) != NULL)
    {
        return(IF_CMD);
    }
    else if((found_cmd = strstr(line, "printVars")) != NULL)
    {
        return(PRINTVARS_CMD);
    }
    else if((found_cmd = strstr(line, "typeof")) != NULL)
    {
        return(TYPEOF_CMD);
    }
    else if((found_cmd = strstr(line, "else")) != NULL)
    {
        return(ELSE_CMD);
    }
    else if((found_cmd = strstr(line, "continue")) != NULL)
    {
        return(CONTINUE_CMD);
    }
    else if((found_cmd = strstr(line, "break")) != NULL)
    {
        return(BREAK_CMD);
    }
    else if((found_cmd = strstr(line, "import")) != NULL)
    {
        return(IMPORT_CMD);
    }
    else if((found_cmd = strstr(line, "end-macro")) != NULL)
    {
        return(ENDMACRO_CMD);
    }
    else if((found_cmd = strstr(line, "macro")) != NULL)
    {
        return(NEWMACRO_CMD);
    }
    else
    {
        hptr = anker->anker;
        while(hptr != NULL)
        {
            if(strstr(line, hptr->name) != NULL)
            {
                return(MACRO_CMD);
            }
            hptr = hptr->next;
        }
        return(-1);
    }

}

/**
 * @brief startet den Handler fuer einen for Block
 *
 * @param anker Anker Punkt fuer Variablen
 * @param cmd_buff Buffer welcher den for Block beinhaltet
 * @param p_output Filehandler fuer das Outputfile
 * @param macro_anker Anker Punkt fuer macros
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Den exitstatus von for_handle
 */
int start_for(struct variables *anker, char *cmd_buff, FILE *p_output,
               macros *macro_anker, char *error_str)
{    
    return(for_handle(anker, cmd_buff, p_output, macro_anker, error_str));
}

/**
 * @brief startet den Handler fuer einen if Block
 *
 * @param anker Anker Punkt fuer Variablen
 * @param cmd_buff Buffer welcher den if Block beinhaltet
 * @param p_output Filehandler fuer das Outputfile
 * @param macro_anker Anker Punkt fuer macros
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Den exitstatus von if_handle
 */
int start_if(struct variables *anker, char *cmd_buff, FILE *p_output,
            macros *macro_anker, char *error_str)
{
    return(if_handle(anker, cmd_buff, p_output, macro_anker, error_str));
}

/**
 * @brief startet den Handler fuer einen import
 *
 * @param anker Anker Punkt fuer Variablen
 * @param line Buffer welcher den import Befehl beinhaltet
 * @param p_output Filehandler fuer das Outputfile
 * @param macro_anker Anker Punkt fuer macros
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Den exitstatus von import_handle
 */
int start_import(struct variables *anker, char *line, FILE *p_output,
                 macros *macro_anker, char *error_str)
{
    return(import_handle(anker, line, p_output, macro_anker, error_str));
}

/**
 * @brief gibt aus welchen Typ die uebergebende Variable hat oder ob es sich 
 *        um ein MAcro handelt
 *
 * @param anker Anker Punkt fuer Variablen
 * @param macro_anker Anker Punkt fuer macros
 * @param line Buffer welcher den typeof Befehl beinhaltet
 * @param p_output Filehandler fuer das Outputfile
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Den exitstatus von import_handle
 */
int typeof_cmd(struct varhandle *anker, macros *macro_anker, char *line, 
               FILE *p_output, char *error_str)
{

    char *begin_args, *end_args;
    char var_name[NAME_LENGTH];
    struct variables *found_var;
    struct macro_definition *hptr;


    line = StripTrailingSpaces(line);
    TrimSpaces(line);

    if((begin_args = strchr(line, '(')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing (");
        return(-1);
    }
    if((end_args = strchr(begin_args, ')')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing )");
        return(-2);
    }

    end_args[0] = '\0';
    strcpy(var_name, begin_args+1);
    printf("typeof: [%s]\n", var_name);

    if((found_var = searchVar(anker, var_name)) != NULL)
    {
        switch(found_var->type)
        {
            case INT:
                fprintf(p_output, "integer");
                break;
            case STRING:
                fprintf(p_output, "string");
                break;
            case INTARRAY:
                fprintf(p_output, "integer Array");
                break;
            case STRINGARRAY:
                fprintf(p_output, "string Array");
                break;
            case TWO_DINTARRAY:
                fprintf(p_output, "two dimension integer Array");
                break;
            case TWO_DSTRINGARRAY:
                fprintf(p_output, "two dimension string Array");
                break;
        }
        return(0);
    }
    else
    {
        hptr = macro_anker->anker;
        while(hptr != NULL)
        {
            if(strcmp(hptr->name, var_name) == 0)
            {
                fprintf(p_output, "macro");
                return(0);
            }
            hptr = hptr->next;
        }
    }
    fprintf(p_output, "undefined");

    return(0);
}

