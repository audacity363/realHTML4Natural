#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <wchar.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"
#include "jinja_parser.h"

/**
 * @file for_handling.c
 * @brief Bearbeitet einen For-Block
 */

/**
 * @brief Ließt die Array Laenge aus und gibt die gebrauchte zurueck
 *
 * @param anker Anker Punkt fuer Variablen
 * @param name Name der Variable
 * @param var_type Typ der variable
 * @param index_type Index Typ; Ob es 0, 1 oder 2 Index ist
 * @param x_index Ermittelte X-Index
 * @param y_index Ermittelte Y-Index
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Erfolg: Variablen Length; Error: -100
 */
int for_getArrayLength(struct variables *anker, char *name, int var_type,
                   int index_type, int x_index, int y_index, char *error_str)
{
    char *c_value;
    wchar_t *wc_value;
    int *i_value, length;
    int l_y_index, l_x_index;
    

    if(var_type == STRING)
    {
        c_value = getStringValue(anker, name);
        return(strlen(c_value));
    }
    else if(var_type == STRINGARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, name, NULL, NULL)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-1);
            }
            return(length);
        }
        else if(index_type == 1)
        {
            c_value = getStringValuefromArray(anker, name, x_index);
            return(strlen(c_value));
        }
    }
    else if(var_type == TWO_DSTRINGARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, name, &l_x_index, &l_y_index)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-2);
            }
            return(l_x_index);
        }
        else if(index_type == 1)
        {
            if((length = getArrayLength(anker, name, &l_x_index, &l_y_index)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-3);
            }
            return(l_y_index);
        }
        else if(index_type == 2)
        {
            c_value = getStringValuefrom2DArray(anker, name, x_index, y_index);
            return(strlen(c_value));
        }
    }
    else if(var_type == U_STRING)
    {
        if((wc_value = getUStringVal(anker, name)) ==NULL)
        {
            strcpy(error_str, varhandle_error_str);
            return(-3);
        }
        return(wcslen(wc_value));
    }
    else if(var_type == U_STRINGARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, name, &l_x_index, &l_y_index)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-3);
            }
            return(l_x_index);
        }
        else if(index_type == 1)
        {
            if((wc_value = getUStringArrayVal(anker, name, x_index)) == NULL)
            {
                strcpy(error_str, varhandle_error_str);
                return(-3);
            }
            return(wcslen(wc_value));
        }
    }
    else if(var_type == U_TWO_DSTRINGARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, name, &l_x_index, &l_y_index)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-3);
            }
            return(l_x_index);
        }
        else if(index_type == 1)
        {
            if((length = getArrayLength(anker, name, &l_x_index, &l_y_index)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-3);
            }
            return(l_y_index);

        }
        else if(index_type == 2)
        {
            if((wc_value = getUStringArrayVal(anker, name, x_index)) == NULL)
            {
                strcpy(error_str, varhandle_error_str);
                return(-3);
            }
            return(wcslen(wc_value));
        }
    }
    else if(var_type == INTARRAY)
    {
        if((length = getArrayLength(anker, name, NULL, NULL)) < 0)
        {
            sprintf(error_str, "Unkown_variable [%s]", name);
            return(-4);
        }
        return(length);
    }
    else if(var_type == TWO_DINTARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, name, &l_x_index, &l_y_index)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-3);
            }
            return(l_x_index);
        }
        else if(index_type == 1)
        {
            if((length = getArrayLength(anker, name, &l_x_index, &l_y_index)) < 0)
            {
                sprintf(error_str, "Unkown_variable [%s]", name);
                return(-3);
            }
            return(l_y_index);
        }
    }
    return(-100);
}

/**
 * @brief erstellt eine temporaere Variable welche im For-Block benutzt werden 
 *        kann
 *
 * @param anker Anker Punkt fuer Variablen
 * @param tmp_name Name der neuen Variable
 * @param variable Name der Variable, durch welche durchiteriert werden soll
 * @param var_type Typ der neuen Variable
 * @param index_type Falls die alte Variable ein Array ist wird hier angegbene
 *        welcher Index Typ benutzt werden soll
 * @param x_index Vorher emittelte X-Index
 * @param y_index Vorher emittelte Y-Index
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 */
int createTmpVar(struct variables *anker, char *tmp_name, char *variable,
                int var_type, int index_type, int x_index, int y_index,
                char *error_str)
{
    char *c_value, tmp_str[2];
    int i_value;
    int i, x, y, length, var_length;


    tmp_str[1] = '\0';

    if(var_type == STRING)
    {
        newStringVar(anker, tmp_name, " ");
    }
    else if(var_type == STRINGARRAY)
    {
        newStringVar(anker, tmp_name, " ");
    }
    else if(var_type == TWO_DSTRINGARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, variable, &x, &y)) < 0)
            {
                sprintf(error_str, "Unkown Variable [%s]", variable);
                return(-1);
            }
            newStringArray(anker, tmp_name, " ");
            for(i=1; i < y; i++)
            {
                appendStringArray(anker, tmp_name, " ");
            }
        }
        else if(index_type == 1)
        {
            newStringVar(anker, tmp_name, " ");
        }
        else if(index_type == 2)
        {
            newStringVar(anker, tmp_name, " ");
        }
    }
    else if(var_type == U_STRING)
    {
        if((length = getVarLength(anker, variable)) < 0)
        {
            strcpy(error_str, varhandle_error_str);
            return(-2);
        }
        newUStringVar(anker, tmp_name, L" ", length);
    }
    else if(var_type == U_STRINGARRAY)
    {
        if((length = getVarLength(anker, variable)) < 0)
        {
            strcpy(error_str, varhandle_error_str);
            return(-2);
        }
        newUStringVar(anker, tmp_name, L" ", length);
    }
    else if(var_type == U_TWO_DSTRINGARRAY)
    {
        if((var_length = getVarLength(anker, variable)) < 0)
        {
            strcpy(error_str, varhandle_error_str);
            return(-2);
        }

        if(index_type == 0)
        {
            if((length = getArrayLength(anker, variable, &x, &y)) < 0)
            {
                sprintf(error_str, "Unkown Variable [%s]", variable);
                return(-1);
            }
            newUStringArray(anker, tmp_name, var_length, x);
        }
        else if(index_type == 1)
        {
            newUStringVar(anker, tmp_name, L" ", var_length);
        }
        else if(index_type == 2)
        {
            newUStringVar(anker, tmp_name, L" ", var_length);
        }
    }
    else if(var_type == INTARRAY)
    {
        newIntVar(anker, tmp_name, 0);
    }
    else if(var_type == TWO_DINTARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, variable, &x, &y)) < 0)
            {
                sprintf(error_str, "Unkown Variable [%s]", variable);
                return(-2);
            }
            newIntArray(anker, tmp_name, 0);
            for(i=1; i < y; i++)
            {
                appendIntArray(anker, tmp_name, 0);
            }
        }
        else if(index_type == 1)
        {
            newIntVar(anker, tmp_name, 0);
        }
    }
    return(0);
}

/**
 * @brief Fuellt die zuvor, in @ref createTmpVar erstellte, temporaere Variable
 *        welche im For-Block benutzt werden 
 *
 * @param anker Anker Punkt fuer Variablen
 * @param tmp_name Name der neuen Variable
 * @param variable Name der Variable, durch welche durchiteriert werden soll
 * @param var_type Typ der neuen Variable
 * @param index_type Falls die alte Variable ein Array ist wird hier angegbene
 *        welcher Index Typ benutzt werden soll
 * @param x_index Vorher emittelte X-Index
 * @param y_index Vorher emittelte Y-Index
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 */

int fillTmpVar(struct variables *anker, char *tmp_name, char *variable,
                int var_type, int index_type, int x_index, int y_index,
                int for_index, char *error_str)
{
    char *c_value, tmp_str[10];
    wchar_t *wc_value;
    int i_value, x, y, length, i, rc;

    if(var_type == STRING)
    {
        tmp_str[1] = '\0';

        c_value = getStringValue(anker, variable);
        tmp_str[0] = c_value[for_index];

        editStringVar(anker, tmp_name, tmp_str);
    }
    else if(var_type == STRINGARRAY)
    {
        if(index_type == 0)
        {
            c_value = getStringValuefromArray(anker, variable, for_index);
            editStringVar(anker, tmp_name, c_value);
        }
        else if(index_type == 1)
        {
            tmp_str[1] = '\0';

            c_value = getStringValuefromArray(anker, variable, x_index);
            tmp_str[0] = c_value[for_index];

            editStringVar(anker, tmp_name, tmp_str);
        }
    }
    else if(var_type == TWO_DSTRINGARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, variable, &x, &y)) < 0)
            {
                sprintf(error_str, "Unkown Variable [%s]", variable);
                return(-1);
            }

            for(i=0; i < y; i++)
            {
                c_value = getStringValuefrom2DArray(anker, variable, for_index,
                            i);
                editStringVarArray(anker, tmp_name, c_value, i);
            }
        }
        else if(index_type == 1)
        {
            c_value = getStringValuefrom2DArray(anker, variable, x_index,
                                                for_index);
            editStringVar(anker, tmp_name, c_value);
        }
        else if(index_type == 2)
        {
            c_value = getStringValuefrom2DArray(anker, variable, x_index,
                                                y_index);
            tmp_str[1] = '\0';
            tmp_str[0] = c_value[for_index];
            editStringVar(anker, tmp_name, tmp_str);
        }
    }
    else if(var_type == U_STRING)
    {
        //die vier Byte herausfilter die vom for_index gewuenscht wurden
    }
    else if(var_type == U_STRINGARRAY)
    {
        if(index_type == 0)
        {
            if((wc_value = getUStringArrayVal(anker, variable, for_index)) == NULL)
            {
                strcpy(error_str, varhandle_error_str);
                return(-2);
            }
            editUStringVar(anker, tmp_name, wc_value);
        }
        else if(index_type == 1)
        {
            //die vier Byte herausfilter die vom for_index gewuenscht wurden
        }
    }
    else if(var_type == U_TWO_DSTRINGARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, variable, &x, &y)) < 0)
            {
                sprintf(error_str, "Unkown Variable [%s]", variable);
                return(-1);
            }
            for(i=0; i < y; i++)
            {
                if((wc_value = getUString2DArrayVal(anker, variable, for_index, i)) == NULL)
                {
                    strcpy(error_str, varhandle_error_str);
                    return(-2);
                }
                editUStringVar(anker, tmp_name, wc_value);
            }
        }
        else if(index_type == 1)
        {
            if((wc_value = getUString2DArrayVal(anker, variable, x_index, for_index)) == NULL)
            {
                strcpy(error_str, varhandle_error_str);
                return(-2);
            }
            editUStringVar(anker, tmp_name, wc_value);

        }
        else if(index_type == 2)
        {
            //die vier Byte herausfilter die vom for_index gewuenscht wurden
        }
    }
    else if(var_type == INTARRAY)
    {
        if((i_value = getIntValuefromArray(anker, variable, for_index)) == -1)
        {
            sprintf(error_str, "Unkown Variable [%s]", variable);
            return(-2);
        }
        else if(i_value == -2)
        {
            strcpy(error_str, "Index Error: Index out of range");
            return(-3);
        }
        editIntVar(anker, tmp_name, i_value);
    }
    else if(var_type == TWO_DINTARRAY)
    {
        if(index_type == 0)
        {
            if((length = getArrayLength(anker, variable, &x, &y)) < 0)
            {
                sprintf(error_str, "Unkown Variable [%s]", variable);
                return(-4);
            }
            for(i=0; i < x; i++)
            {
                if((rc = getIntValuefrom2DArray(anker, variable, for_index, i,
                                                &i_value)) == -1)
                {
                    sprintf(error_str, "Unkown Variable [%s]", variable);
                    return(-5);
                }
                else if(rc == -2)
                {
                    strcpy(error_str, "Index Error: Index out of range");
                    return(-6);
                }
                editIntVarArray(anker, tmp_name, i_value, i);
            }
        }
        else if(index_type == 1)
        {
            if((rc = getIntValuefrom2DArray(anker, variable, x_index, for_index,
                                            &i_value)) == -1)
            {
                sprintf(error_str, "Unkown Variable [%s]", variable);
                return(-7);
            }
            else if(rc == -2)
            {
                strcpy(error_str, "Index Error: Index out of range");
                return(-8);
            }
            editIntVar(anker, tmp_name, i_value);
        }
    }

    return(0);
}

/**
 * @brief zerlegt den For-Block, parst die erste Zeile, erstellt dann daraus 
 *        eine For-Schleife und geht durch jede Zeile im Block durch und ruft 
 *        @ref parse_line auf
 *
 * @param anker Anker Punkt fuer Variablen
 * @param cmd_buff Buffer in dem der For-Block gespeichert ist
 * @param p_output Filehandle auf das Output File
 * @param macro_anker Anker Punkt fuer Macro Definitionen
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Header-Error: < 0; Parser-Error: Return Code von @ref parse_line;
 *         Erfolg: 0
 */
int for_handle(struct variables *anker, char *cmd_buff, FILE *p_output,
               macros *macro_anker, char *error_str)
{
    char for_block[200][2024], *tmp_buff, *for_cmd, for_cmd_detail[10][1024];
    //char *for_block, *for_cmd_detail;
    char l_cmd_buff[100*2024];
    //char *line;
    char line[2024];

    int block_length = 0, i, x, for_index, arg_length, var_type;
    int index_type, x_index, y_index;
    int parser_status = 0;
    int l_in_for = 0, l_in_if = 0;
    int found_fors = 0, found_ifs = 0, parser_ret;

    tmp_buff = strtok(cmd_buff, "\n");

    strcpy(for_block[0], tmp_buff);
    block_length++;


    while(1)
    {
        if((tmp_buff = strtok(NULL, "\n")) == NULL)
        {
            break;
        }
        block_length++;

        strcpy(for_block[block_length-1], tmp_buff);
    }


    //For Befehl vorne und hinten von Leerzeichen befreien
    for_cmd = for_block[0];
    TrimSpaces(for_cmd);
    for_cmd = StripTrailingSpaces(for_cmd);

    //for Token
    if((tmp_buff = strtok(for_cmd, " ")) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing Token");
        return(-1);
    }

    strcpy(for_cmd_detail[0], tmp_buff);
    
    for(i=1; i < 4;i++)
    {
        if((tmp_buff = strtok(NULL, " ")) == NULL)
        {
            strcpy(error_str, "Syntax Error: Missing Token");
            return(-1);
        }
        strcpy(for_cmd_detail[i], tmp_buff);
    }

    if(strcmp(for_cmd_detail[2], "in") == 0)
    {
        if((index_type = getIndex(anker, for_cmd_detail[3],
                                  &x_index, &y_index, error_str)) < 0)
        {
            return(-3);
        }
        if((var_type = getVarType(anker, for_cmd_detail[3])) < 0)
        {
            strcpy(error_str, varhandle_error_str);
            return(-4);
        }
        if(var_type == INT)
        {
            strcpy(error_str, "Type Error: Integer is not iterable");
            return(-4);
        }
        else if(var_type == INTARRAY && index_type > 0)
        {
            strcpy(error_str, "Type Error: Integer is not iterable");
            return(-5);
        }
        else if(var_type == TWO_DINTARRAY && index_type == 2)
        {
            strcpy(error_str, "Type Error: Integer is not iterable");
            return(-6);
        }
        else if(var_type == STRING && index_type > 0)
        {
            strcpy(error_str, "Type Error: Char is not iterable");
            return(-7);
        }
        else if(var_type == STRINGARRAY && index_type == 2)
        {
            strcpy(error_str, "Type Error: String has no second index");
            return(-8);
        }

    }

    if((arg_length = for_getArrayLength(anker, for_cmd_detail[3],
            var_type, index_type, x_index, y_index, error_str)) < 0)
    {
        return(-9);
    }

  
    //loop.i nur anlegen wenn sie noch nicht existiert
    if(getVarType(anker, "loop.i") == -1)
    {
        newIntVar(anker, "loop.i", 0);
    }

    createTmpVar(anker, for_cmd_detail[1], for_cmd_detail[3],
                var_type, index_type, x_index, y_index,
                error_str);

    for(i=0; i < arg_length; i++)
    {
        fillTmpVar(anker, for_cmd_detail[1], for_cmd_detail[3],
                var_type, index_type, x_index, y_index,
                i, error_str);

        found_ifs = 0;

        for(x=1; x < block_length; x++)
        {
            editIntVar(anker, "loop.i", i);
            strcpy(line, for_block[x]);
            strcat(line, "\n");

            if(strstr(line, "if") != NULL && strstr(line, "end-if") == NULL)
            {
                found_ifs++;
            }
            else if(strstr(line, "end-if"))
            {
                found_ifs--;
            }

            if(found_ifs == 0)
            {
                if(strstr(line, "break") != NULL)
                {
                    return(0);
                }
                else if(strstr(line, "continue") != NULL)
                {
                    break;
                }
            }

            if((parser_ret = parse_line(anker, macro_anker, line, p_output, l_cmd_buff,
                       &parser_status, &l_in_for, &l_in_if, error_str)) < 0)
            {
                return(-10);
            }
            else if(parser_ret == 1)
            {
                return(0);
            }
            else if(parser_ret == 1)
            {
                break;
            }
            bzero(line, sizeof(line));
        }
    }
    return(0);
}
