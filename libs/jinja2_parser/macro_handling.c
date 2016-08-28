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
 * @file macro_handling.c
 * @author Tom Engemann
 * @date 26.08.2016
 * @brief Kuemmert sich um alle Aufrufe, die mit Macros zu tun haben
 */

/**
 * @brief Sucht den Namen, parameter und parameter default Values heraus und 
 *        schreibt diese in den zwischen Buffer im macro_anker struct
 *
 * @param macro_anker Anker Punkt fuer macros
 * @param line Buffer in dem die Definition des Macros steht
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return 0 wenn alles OK war; < 0 wenn es einen Fehler gab
 */
int getMacroName(macros *macro_anker, char *line, char *error_str)
{
    char *begin, *end;
    char *begin_args, *end_args;
    char *equals_ptr, default_val[2048];
    char *args_split;
    char *import_line, *cmd_buff = NULL;
    char *saveptr;

    int found_args = 0;

    if((begin = strchr(line, '"')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"");
        return(-1);
    }

    if((end = strrchr(begin+1, '"')) == NULL)
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

    if((begin_args = strchr(begin, '(')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing (");
        return(-4);
    }

    if((end_args = strchr(begin_args, ')')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing )");
        return(-5);
    }

    end_args[0] = '\0';
    begin_args[0] = '\0';

    bzero(macro_anker->aguments_name, sizeof(((macros *)0)->aguments_name));

    if(strlen(begin_args+1) != 0)
    {
        //Es werden argumente mit ubergeben
        
        begin_args = begin_args+1;

        printf("[%s]\n", begin_args);
        args_split = strtok_r(begin_args, ",", &saveptr);

        printf("Args:\n");
        while(args_split != NULL)
        {
            args_split = StripTrailingSpaces(args_split);
            TrimSpaces(args_split);

            //Check ob es einen Default Value gibt
            if((equals_ptr = strchr(args_split, '=')) != NULL)
            {
                //Speicher diesen Deafult Value
                equals_ptr[0] = '\0';
                strcpy(default_val, equals_ptr+1);
                macro_anker->default_value[found_args] = 1;
            }
            else
                macro_anker->default_value[found_args] = 0;
    
            //Name der Argumente und deren Default Values zwischen speichern
            strcpy(macro_anker->aguments_name[found_args], args_split);
            strcpy(macro_anker->default_values[found_args], default_val);

            found_args++;

            printf("\t%d: [%s]\n", found_args, args_split);
            printf("\t\tDefault: [%s]\n", default_val);

            //Clean the default value buffer
            bzero(default_val, sizeof(default_val));

            //Naechsten Wert heraussuchen
            args_split = strtok_r(NULL, ",", &saveptr);
        }

        macro_anker->aguments = found_args;
    }
    else
    {
        //Es werden keine Agrumente mit uebergeben
        macro_anker->aguments = 0;
    }


    strcpy(macro_anker->cur_name, begin+1);

    return(0);
}

/**
 * @brief Gibt alle definierten Macros aus
 *        \warning nur fuer Debugging 
 *        
 *
 * @param macro_anker Anker Punkt fuer macros
 *
 * @return Nothing
 */
void printallMacros(macros *macro_anker)
{
    struct macro_definition *hptr = macro_anker->anker;
    int i;

    while(hptr != NULL)
    {
        printf("Name: [%s]\n", hptr->name);
        for(i=0; i != hptr->aguments; i++)
        {
            printf("\t[%s] = [%s]\n", hptr->aguments_name[i], 
                                      hptr->default_values[i]);
        }
        hptr = hptr->next;
    }
}

/**
 * @brief Speichert ein Macro. Die Werte dafuer kommen aus dem macros_anker 
 *        struct
 *
 * @param macro_anker Anker Punkt fuer macros
 *
 * @return Zero
 */
int saveMacro(macros *macros_anker)
{
    struct macro_definition *hptr, *new_value, *anker;
    int override = 0;
    
    hptr = macros_anker->anker;

    while(hptr->next != NULL)
    {
        if(strcmp(hptr->name, macros_anker->cur_name) == 0)
        {
            override = 1;
            break;
        }

        hptr = hptr->next;
    }

    if(strcmp(hptr->name, macros_anker->cur_name) == 0)
    {
        override = 1;
    }

    if(override == 1)
    {
        free(hptr->value);
        hptr->value = malloc(strlen(macros_anker->macro_buff)+1);

        hptr->aguments = macros_anker->aguments;

        memcpy(hptr->aguments_name, macros_anker->aguments_name, 
                sizeof(((macros *)0)->aguments_name));

        memcpy(hptr->default_values, macros_anker->default_values,
                sizeof(((macros *)0)->default_values));
        memcpy(hptr->default_value, macros_anker->default_value,
                sizeof(((macros *)0)->default_value));

        strcpy(hptr->value, macros_anker->macro_buff);
    }
    else
    {
        new_value = malloc(sizeof(struct macro_definition));
        strcpy(new_value->name, macros_anker->cur_name);
        new_value->value = malloc(strlen(macros_anker->macro_buff)+1);
        strcpy(new_value->value, macros_anker->macro_buff);

        new_value->aguments = macros_anker->aguments;

        memcpy(new_value->aguments_name, macros_anker->aguments_name, 
                sizeof(((macros *)0)->aguments_name));

        memcpy(new_value->default_values, macros_anker->default_values,
                sizeof(((macros *)0)->default_values));

        new_value->next = NULL;
        hptr->next = new_value;
    }

    free(macros_anker->macro_buff);
    macros_anker->macro_buff = NULL;
    bzero(macros_anker->cur_name, 1024);
    return(0);
}

/**
 * @brief Fuert ein Macro aus
 *
 * @param macro_anker Anker Punkt fuer macros
 * @param line Zeile mit dem Aufruf des Macros 
 * @param p_output File pointer auf das Output file
 * @param var_anker Anker Punkt fuer Variablen
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return 0 wenn alles OK war; < 0 wenn es einen Fehler gab
 */
int printMacro(macros *macro_anker, char *line, FILE *p_output, 
               struct variables *var_anker, char *error_str)
{
    struct macro_definition *hptr, *new_value, *anker;
    char *buff;
    char arguments[1024][2048], tmp_buff[2];
    char *args_begin, *args_end;
    char *saveptr, *bodysave;
    char *bodybuff, *found_end;

    int anzahl_args = 0;
    int var_type, i;
    int index_type, x_index, y_index;
    int x_length, y_length;
    int x, y;
    int parser_rc;

    int i_buff;
    char *c_buff;

    struct variables *new_var_anker = malloc(sizeof(struct variables));
    new_var_anker->next = NULL;

    TrimSpaces(line);
    line = StripTrailingSpaces(line);

    if((args_begin = strchr(line, '(')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"(\"");
        return(-2);
    }
    
    if((args_end = strchr(line, ')')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \")\"");
        return(-3);
    }

    args_begin[0] = '\0';
    args_end[0] = '\0';

    if(strlen(args_begin+1) != 0)
    {
        buff = strtok_r(args_begin+1, ",", &saveptr);

        while(buff != NULL)
        {
            buff = StripTrailingSpaces(buff);
            TrimSpaces(buff);

            strcpy(arguments[anzahl_args], buff);
            anzahl_args++;

            buff = strtok_r(NULL, ",", &saveptr);
        }
    }
    else
    {
        anzahl_args = 0;
    }

    hptr = macro_anker->anker->next;

    while(hptr != NULL)
    {
        if(strcmp(hptr->name, line) == 0)
        {
            break;
        }
        hptr = hptr->next;
    }

    if(hptr == NULL)
    {
        sprintf(error_str, "Name Error: Macro [%s] not found", line);
        return(-1);
    }

    if(anzahl_args != hptr->aguments)
    {
        if(hptr->default_value[anzahl_args] != 1)
        {
            sprintf(error_str, "Type Error: %s takes exactly %d arguments (%d given)", 
                    line, hptr->aguments, anzahl_args);
            return(-4);
        }
        for(x=anzahl_args; x != hptr->aguments; x++)
        {
            strcpy(arguments[x], hptr->default_values[x]);
        }
        
    }

    for(i=0; i != hptr->aguments; i++)
    {
        index_type = getIndex(var_anker, arguments[i], &x_index, &y_index, 
                              error_str);
        if((var_type = getVarType(var_anker, arguments[i])) < 0)
        {
            if(arguments[i][0] == '"')
            {
                memmove(arguments[i], arguments[i]+1, strlen(arguments[i])+1);
                if((found_end = strrchr(arguments[i], '"')) == NULL)
                {
                    strcpy(error_str, "Syntax Error: Missing \"");
                    return(-1);
                }
                found_end[0] = '\0';
                newStringVar(new_var_anker, hptr->aguments_name[i],
                             arguments[i]);
            }
            else if(str_isdigit(arguments[i]) == 0)
            {
                i_buff = atoi(arguments[i]);
                newIntVar(new_var_anker, hptr->aguments_name[i], i_buff);
            }
            else
            {
                sprintf(error_str, "Type Error: %s Unkown Var or Type", 
                        arguments[i]);
                return(-5);
            }    
        }
        else if(var_type == INTARRAY && index_type == 1)
        {
            i_buff = getIntValuefromArray(var_anker, arguments[i], x_index);
            newIntVar(new_var_anker, hptr->aguments_name[i], i_buff);
        }
        else if(var_type == TWO_DINTARRAY && index_type == 1)
        {
            if(getArrayLength(var_anker, arguments[i], &x_length, &y_length) <0)
            {
                strcpy(error_str, "Type Error: Unkown Error");
                return(-6);
            }

            getIntValuefrom2DArray(var_anker, arguments[i], x_index, 0, &i_buff);
            newIntArray(new_var_anker, hptr->aguments_name[i], i_buff);

            for(x=1; x < x_length; x++)
            {
                getIntValuefrom2DArray(var_anker, arguments[i],
                                                x_index, x, &i_buff);
                appendIntArray(new_var_anker, hptr->aguments_name[i], i_buff);
            }
        }
        else if(var_type == TWO_DINTARRAY && index_type == 2)
        {
            getIntValuefrom2DArray(var_anker, arguments[i], x_index,
                                            y_index, &i_buff);
            newIntVar(new_var_anker, hptr->aguments_name[i], i_buff);
        }
        else if(var_type == STRINGARRAY && index_type == 1)
        {
            c_buff = getStringValuefromArray(var_anker, arguments[i], x_index);
            newStringVar(new_var_anker, hptr->aguments_name[i], c_buff);
        }
        else if(var_type == TWO_DSTRINGARRAY && index_type == 1)
        {
            if(getArrayLength(var_anker, arguments[i], &x_length, &y_length) <0)
            {
                strcpy(error_str, "Type Error: Unkown Error");
                return(-6);
            }

            c_buff = getStringValuefrom2DArray(var_anker, arguments[i], x_index, 0);
            newStringArray(new_var_anker, hptr->aguments_name[i], c_buff); 

            for(x=1; x < x_length; x++)
            {
                c_buff = getStringValuefrom2DArray(var_anker, arguments[i],
                                                x_index, x);
                appendStringArray(new_var_anker, hptr->aguments_name[i], c_buff);
            }

        }
        else if(var_type == TWO_DSTRINGARRAY && index_type == 2)
        {
            c_buff = getStringValuefrom2DArray(var_anker, arguments[i], x_index,
                                               y_index);
            newStringVar(new_var_anker, hptr->aguments_name[i], c_buff);
        }
        else if(var_type == STRING && index_type == 1)
        {
            c_buff = getStringValue(var_anker, arguments[i]);
            tmp_buff[0] = c_buff[x_index];
            newStringVar(new_var_anker, hptr->aguments_name[i], tmp_buff);
        }
        else if(var_type == STRING)
        {
            c_buff = getStringValue(var_anker, arguments[i]);
            newStringVar(new_var_anker, hptr->aguments_name[i], c_buff);
        }
        else if(var_type == INT)
        {
            i_buff = getIntValue(var_anker, arguments[i]);
            newIntArray(new_var_anker, hptr->aguments_name[i], i_buff);
        }
        else if(var_type == INTARRAY)
        {
            if((x_length = getArrayLength(var_anker, arguments[i], 0, 0)) <0)
            {
                strcpy(error_str, "Type Error: Unkown Error");
                return(-6);
            }
            i_buff = getIntValuefromArray(var_anker, arguments[i], 0);
            newIntArray(new_var_anker, hptr->aguments_name[i], i_buff);
            for(x=1; x < x_length; x++)
            {
                i_buff = getIntValuefromArray(var_anker, arguments[i], x);
                appendIntArray(new_var_anker, hptr->aguments_name[i], i_buff);
            }
        }
        else if(var_type == STRINGARRAY)
        {
            if((x_length = getArrayLength(var_anker, arguments[i], 0, 0)) < 0)
            {
                strcpy(error_str, "Type Error: Unkown Error");
                return(-7);
            }

            c_buff = getStringValuefromArray(var_anker, arguments[i], 0);
            newStringArray(new_var_anker, hptr->aguments_name[i], c_buff);

            for(x=1; x < x_length; x++)
            {
                c_buff = getStringValuefromArray(var_anker, arguments[i], x);
                appendStringArray(new_var_anker, hptr->aguments_name[i], c_buff);
            }
        }
        else if(var_type == TWO_DINTARRAY)
        {
            if(getArrayLength(var_anker, arguments[i], &x_length, &y_length) <0)
            {
                strcpy(error_str, "Type Error: Unkown Error");
                return(-8);
            }

            new2DIntArray(new_var_anker, hptr->aguments_name[i], x_length, 
                          y_length);
            for(x=0; x < x_length; x++)
            {
                for(y=0; y < y_length; y++)
                {
                    getIntValuefrom2DArray(var_anker, arguments[i], x, y,
                                         &i_buff);
                    editIntVar2DArray(new_var_anker, hptr->aguments_name[i], 
                                      i_buff, x, y);
                }
            }
        }
        else if(var_type == TWO_DSTRINGARRAY)
        {
            if(getArrayLength(var_anker, arguments[i], &x_length, &y_length) <0)
            {
                strcpy(error_str, "Type Error: Unkown Error");
                return(-8);
            }

            new2DStringArray(new_var_anker, hptr->aguments_name[i], x_length,
                             y_length);

            for(x=0; x < x_length; x++)
            {
                for(y=0; y < y_length; y++)
                {
                    c_buff = getStringValuefrom2DArray(var_anker,
                                              arguments[i], x, y);
                    editStringVar2DArray(new_var_anker, hptr->aguments_name[i],
                                         c_buff, x, y);
                }
            }
        }
    }

    //newStringVar(new_var_anker, "testvar", "Hello Wold");

    char *cmd_buff;
    int just_save, in_for, in_if;

    bodybuff = strtok_r(hptr->value, "\n", &bodysave);
    while(bodybuff != NULL)
    {
        if((parser_rc = parse_line(new_var_anker, macro_anker, bodybuff,
                                   p_output, &cmd_buff, &just_save, &in_for,
                                   &in_if, error_str)) < 0)
        {
            return(parser_rc);
        }
        //fprintf(p_output, "%s", bodybuff);
        fprintf(p_output, "\n");
        bodybuff = strtok_r(NULL, "\n", &bodysave);
    }

    return(0);
}
