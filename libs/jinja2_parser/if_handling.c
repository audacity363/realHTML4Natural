#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <alloca.h>
#include <wchar.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"
#include "jinja_parser.h"

/**
 * @file if_handling.c
 * @brief Bearbeitet einen IF-Block
 */


int convert1Bto4BString(char *inbuffer, wchar_t *outbuffer, int length)
{
    int i, offset;

    void *v_in, *v_out, *v_tmp;

    v_in = (void*)inbuffer;
    v_out = (void*)outbuffer;

    for(i=0; i < length; i++)
    {
        offset = (sizeof(wchar_t)*i)+3;

        v_tmp = v_out+offset;

        memcpy(v_tmp, v_in+i, 1);
    }

    return(0);
}


/**
 * @brief vergleicht die beiden Variablen
 *
 * @param anker Anker Punkt fuer Variablen
 * @param var1 name der ersten Variable
 * @param var2 name der zweiten Variable
 * @param symbol Vergleichsymbol
 * @param index Wird nicht verwendet und ich habe keine Ahnung mehr warum es
 *        dort steht
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Error: < 0; True: 1; False: 0
 */
int compare(struct variables *anker, char *var1, char *var2, char *symbol, 
            int index, char *error_str)
{
    int var_type1, var_type2;
    char *c_value1, *c_value2;
    wchar_t *wc_value1, *wc_value2;
    int i_value1, i_value2;
    int x_index1, y_index1, index_type1;
    int x_index2, y_index2, index_type2;

    index_type1 = getIndex(anker, var1, &x_index1, &y_index1, error_str);
    if((var_type1 = getVarType(anker, var1)) < 0)
    {
        if(var1[0] == '"')
        {
            //Es ist ein fester String
            var_type1 = U_STRING;
            wc_value1 = malloc((strlen(var1)+1)*sizeof(wchar_t));
            bzero(wc_value1, (strlen(var1)+1)*sizeof(wchar_t));
            //memmove(c_value1, var1+1, strlen(var1));
            convert1Bto4BString(var1, wc_value1, strlen(var1)+1);
            //c_value1[strlen(c_value1)-1] = '\0';
        }
        else if(str_isdigit(var1) == 0)
        {
            //Es ist ein Integer
            var_type1 = INT;
            i_value1 = atoi(var1);
        }
        else
        {
            strcpy(error_str, "Type Error: Var1: Unkown Var or Type");
            return(-1);
        }
    }
    else if(var_type1 == INTARRAY && index_type1 == 1)
    {
        if((i_value1 = getIntValuefromArray(anker, var1, x_index1)) == -1)
        {
            sprintf(error_str, "Unkown Variable [%s]", var1);
            return(-2);
        }
        else if(i_value2 == -2)
        {
            strcpy(error_str, "Index Error: index out of range");
            return(-3);
        }
        var_type1 = INT;
    }
    else if(var_type1 == TWO_DINTARRAY && index_type1 == 2)
    {
        if(getIntValuefrom2DArray(anker, var1, x_index1, y_index1,
                                  &i_value1) < 0)
        {
            strcpy(error_str, varhandle_error_str);
            return(-4);
        }
        var_type1 = INT;
    }
    else if(var_type1 == STRINGARRAY && index_type1 == 1)
    {
        c_value1 = getStringValuefromArray(anker, var1, x_index1);
        var_type1 = STRING;
    }
    else if(var_type1 == TWO_DSTRINGARRAY && index_type1 == 2)
    {
        c_value1 = getStringValuefrom2DArray(anker, var1, x_index1, y_index1);
        var_type1 = STRING;
    }
    else if(var_type1 == STRING)
    {
        c_value1 = getStringValue(anker, var1);
    }
    else if(var_type1 == INT)
    {
        i_value1 = getIntValue(anker, var1);
    }
    else if(var_type1 == U_STRING)
    {
        wc_value1 = getUStringVal(anker, var1);
    }
    else if(var_type1 == U_STRINGARRAY && index_type1 == 1)
    {
        wc_value1 = getUStringArrayVal(anker, var1, x_index1);
    }
    else if(var_type1 == U_TWO_DSTRINGARRAY && index_type1 == 2)
    {
        wc_value1 = getUString2DArrayVal(anker, var1, x_index1, y_index1);
    }
    else
    {
        strcpy(error_str, "Type Error: Arrays are not supported, yet");
        return(-2);
    }

    index_type2 = getIndex(anker, var2, &x_index2, &y_index2, error_str);
    if((var_type2 = getVarType(anker, var2)) < 0)
    {
        if(var2[0] == '"')
        {
            //Es ist ein fester String
            var_type2 = U_STRING;
            wc_value2 = malloc((strlen(var1)+1)*sizeof(wchar_t));
            bzero(wc_value2, (strlen(var2)+1)*sizeof(wchar_t));
            //memmove(c_value1, var1+1, strlen(var1));
            convert1Bto4BString(var2, wc_value2, strlen(var2)+1);
            //c_value1[strlen(c_value1)-1] = '\0';

            /*var_type2 = STRING;
            c_value2 = malloc(strlen(var2));
            memmove(c_value2, var2+1, strlen(var2));
            c_value2[strlen(c_value2)-1] = '\0';*/
        }
        else if(str_isdigit(var2) == 0)
        {
            //Es ist ein Integer
            var_type2 = INT;
            i_value2 = atoi(var2);
        }
        else
        {
            strcpy(error_str, "Type Error: var2: Unkown Var or Type");
            return(-1);
        }
    }
    else if(var_type2 == INTARRAY && index_type2 == 1)
    {
        if((i_value2 = getIntValuefromArray(anker, var2, x_index2)) == -1)
        {
            sprintf(error_str, "Unkown Variable [%s]", var2);
            return(-2);
        }
        else if(i_value2 == -2)
        {
            strcpy(error_str, "Index Error: index out of range");
            return(-3);
        }
        var_type2 = INT;
    }
    else if(var_type2 == TWO_DINTARRAY && index_type2 == 2)
    {
        if(getIntValuefrom2DArray(anker, var2, x_index2, y_index2,
                                  &i_value2) < 0)
        {
            strcpy(error_str, varhandle_error_str);
            return(-4);
        }
        var_type2 = INT;
    }
    else if(var_type2 == STRINGARRAY && index_type2 == 1)
    {
        c_value2 = getStringValuefromArray(anker, var2, x_index2);
        var_type2 = STRING;
    }
    else if(var_type2 == TWO_DSTRINGARRAY && index_type2 == 2)
    {
        c_value2 = getStringValuefrom2DArray(anker, var2, x_index2, y_index2);
        var_type2 = STRING;
    }

    else if(var_type2 == STRING)
    {
        c_value2 = getStringValue(anker, var2);
    }
    else if(var_type2 == INT)
    {
        i_value2 = getIntValue(anker, var2);
    }
    else if(var_type2 == U_STRING)
    {
        wc_value2 = getUStringVal(anker, var2);
    }
    else if(var_type2 == U_STRINGARRAY && index_type2 == 1)
    {
        wc_value2 = getUStringArrayVal(anker, var2, x_index2);
    }
    else if(var_type2 == U_TWO_DSTRINGARRAY && index_type2 == 2)
    {
        wc_value2 = getUString2DArrayVal(anker, var2, x_index2, y_index2);
    }
    else
    {
        strcpy(error_str, "Type Error: Arrays are not supported, yet");
        return(-2);
    }

    if(var_type1 != var_type2)
    {
        strcpy(error_str, "Type Error: Var Types are not equals");
        return(-3);
    }

    if(var_type1 == STRING)
    {
        if(strcmp(symbol, "==") == 0)
        {
            if(strcmp(c_value1, c_value2) == 0)
            {
                return(1);
            }
            return(0);
        }
        else if(strcmp(symbol, "!=") == 0)
        {
            if(strcmp(c_value1, c_value2) != 0)
            {
                return(1);
            }
            return(0);
        }
        else
        {
            strcpy(error_str, "Syntax Error: Unkown Opertor");
            return(-4);
        }
    }
    if(var_type1 == U_STRING)
    {
        if(strcmp(symbol, "==") == 0)
        {
            if(wcscmp(wc_value1, wc_value2) == 0)
            {
                return(1);
            }
            return(0);
        }
        else if(strcmp(symbol, "!=") == 0)
        {
            if(wcscmp(wc_value1, wc_value2) != 0)
            {
                return(1);
            }
            return(0);
        }
        else
        {
            strcpy(error_str, "Syntax Error: Unkown Opertor");
            return(-4);
        }
    }
    else if(var_type1 == INT)
    {
        if(strcmp(symbol, "<") == 0)
        {
            if(i_value1 < i_value2)
            {
                return(1);
            }
            return(0);
        }
        else if(strcmp(symbol, ">") == 0)
        {
            if(i_value1 > i_value2)
            {
                return(1);
            }
            return(0);

        }
        else if(strcmp(symbol, "==") == 0)
        {
            if(i_value1 == i_value2)
            {
                return(1);
            }
            return(0);

        }
        else if(strcmp(symbol, "<=") == 0)
        {
            if(i_value1 <= i_value2)
            {
                return(1);
            }
            return(0);

        }
        else if(strcmp(symbol, ">=") == 0)
        {
            if(i_value1 >= i_value2)
            {
                return(1);
            }
            return(0);

        }
        else
        {
            strcpy(error_str, "Syntax Error: Unkown Opertor");
            return(-4);
        }
    }
    free(c_value1);
    free(c_value2);
}

/**
 * @brief zerlegt den IF-Block, parst die erste Zeile, erstellt dann daraus 
 *        eine IF-Abfrage und geht durch jede Zeile im Block durch und ruft 
 *        @ref parse_line auf
 *
 * @param anker Anker Punkt fuer Variablen
 * @param cmd_buff Buffer in dem der IF-Block gespeichert ist
 * @param p_output Filehandle auf das Output File
 * @param macro_anker Anker Punkt fuer Macro Definitionen
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Header-Error: < 0; Parser-Error: Return Code von @ref parse_line;
 *         Erfolg: 0
 */
int if_handle(struct variables *anker, char *cmd_buff, FILE *p_output,
              macros *macro_anker, char *error_str)
{
    char if_block[200][2024], tmp_buff[1024], *if_cmd, if_cmd_detail[10][1024], line[2024], *p_tmp_buff = NULL,
         l_cmd_buff[100*2024];
    //char *if_block, *if_cmd_detail, *tmp_buff, *line;

    int block_length = 0, detail_length = 0, i, x, y, var_type, found_str = 0;
    int if_rc, parser_status = 0, l_in_for = 0, l_in_if = 0;
    int index_type, x_index, y_index, else_offset = 0, found_ifs = 0, found_else = 0;

    FILE *tmp_stdout = stdout;

    p_tmp_buff = strtok(cmd_buff, "\n");

    strcpy(if_block[0], p_tmp_buff);
    block_length++;


    while(1)
    {
        if((p_tmp_buff = strtok(NULL, "\n")) == NULL)
        {
            break;
        }
        block_length++;

        strcpy(if_block[block_length-1], p_tmp_buff);
    }


    //For Befehl vorne und hinten von Leerzeichen befreien
    //if_cmd = ((char**)if_block)[0];
    if_cmd = if_block[0];
    TrimSpaces(if_cmd);
    if_cmd = StripTrailingSpaces(if_cmd);

    bzero(tmp_buff, sizeof(tmp_buff));

    for(i=0; i < strlen(if_cmd); i++)
    {
        if(if_cmd[i] == '"')
        {
            found_str = 1 - found_str;
        }
        if(if_cmd[i] == ' ' && found_str == 0)
        {
            strcpy(if_cmd_detail[detail_length], tmp_buff);

            bzero(tmp_buff, sizeof(tmp_buff));

            detail_length++;
        }
        else
        {
            char_strcat(tmp_buff, if_cmd[i]);
        }
    }

    //Letzen Token auslesen
    strcpy(if_cmd_detail[detail_length], tmp_buff);
    bzero(tmp_buff, sizeof(tmp_buff));
    detail_length++;

    if((if_rc = compare(anker, if_cmd_detail[1], if_cmd_detail[3],
        if_cmd_detail[2], y, error_str)) < 0)
    {
        return(-4);
    }
    else if(if_rc == 0)
    {
        found_ifs = 0;
        for(i=1; i < block_length; i++)
        {
            if(strstr(if_block[i], "end-if") != NULL)
            {
                found_ifs--;
                continue;
            }
            else if(strstr(if_block[i], "if") != NULL)
            {
                found_ifs++;
                continue;
            }
            if(strstr(if_block[i], "else") != NULL && found_ifs == 0)
            {
                else_offset = i+1;
                found_else = 1;
                break;
            }
        }
        if(found_else == 0)
            return(0);
    }
    else
    {
        else_offset = 1;
    }
    
    found_ifs = 0;

    for(i=else_offset; i < block_length; i++)
    {
        bzero(line, sizeof(line));
        strcpy(line, if_block[i]);
        strcat(line, "\n");
        if(strstr(line, "if") != NULL && strstr(line, "end-if") == NULL)
        {
            found_ifs++;
        }
        else if(strstr(line, "end-if"))
        {
            found_ifs--;
        }
        if(if_rc == 1 && found_ifs == 0)
        {
            if(strstr(line, "else") != NULL)
            {
                return(0);
            }
        }

        if(strstr(line, "break") != NULL)
        {
            return(1);
        }
        else if(strstr(line, "continue") != NULL)
        {
            return(2);
        }

        if(parse_line(anker, macro_anker, line, p_output, l_cmd_buff,
                   &parser_status, &l_in_for, &l_in_if, error_str) < 0)
        {
            return(-10);
        }
    }
    return(0);
}
