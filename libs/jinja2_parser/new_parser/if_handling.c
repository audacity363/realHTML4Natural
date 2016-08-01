#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"

int compare(struct variables *anker, char *var1, char *var2, char *symbol, 
            int index, char *error_str)
{
    int var_type1, var_type2;
    char *c_value1, *c_value2;
    int i_value1, i_value2;

    printf("var1: [%s]\n", var1);
    printf("var2: [%s]\n", var2);
    printf("Operator: [%s]\n", symbol);

    if((var_type1 = getVarType(anker, var1)) < 0)
    {
        if(var1[0] == '"')
        {
            //Es ist ein fester String
            var_type1 = STRING;
            c_value1 = malloc(strlen(var1));
            memmove(c_value1, var1+1, strlen(var1));
            c_value1[strlen(c_value1)-1] = '\0';
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
    else if(var_type1 == INTARRAY || var_type1 == STRINGARRAY || 
            var_type1 == TWO_DSTRINGARRAY || var_type1 == TWO_DINTARRAY)
    {
        strcpy(error_str, "Type Error: Arrays are not supported, yet");
        return(-2);
    }
    else if(var_type1 == STRING)
    {
        c_value1 = getStringValue(anker, var1);
    }
    else if(var_type1 == INT)
    {
        i_value1 = getIntValue(anker, var1);
    }

    if((var_type2 = getVarType(anker, var2)) < 0)
    {
        if(var2[0] == '"')
        {
            //Es ist ein fester String
            var_type2 = STRING;
            c_value2 = malloc(strlen(var2));
            memmove(c_value2, var2+1, strlen(var2));
            c_value2[strlen(c_value2)-1] = '\0';
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
    else if(var_type2 == INTARRAY || var_type2 == STRINGARRAY || 
            var_type2 == TWO_DSTRINGARRAY || var_type2 == TWO_DINTARRAY)
    {
        strcpy(error_str, "Type Error: Arrays are not supported, yet");
        return(-2);
    }
    else if(var_type2 == STRING)
    {
        c_value2 = getStringValue(anker, var2);
    }
    else if(var_type2 == INT)
    {
        i_value2 = getIntValue(anker, var2);
    }

    if(var_type1 != var_type2)
    {
        strcpy(error_str, "Type Error: Var Types are not equals");
        return(-3);
    }

    if(var_type1 == STRING)
    {
        printf("compare [%s] and [%s]\n", c_value1, c_value2);
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
    else if(var_type1 == INT)
    {
        printf("compare [%d] and [%d]\n", i_value1, i_value2);
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
}

int if_handle(struct variables *anker, char *cmd_buff, FILE *p_output,
              char *error_str)
{
    char *if_block, *tmp_buff, *if_cmd, *if_cmd_detail, *line, *l_cmd_buff = NULL;
    int block_length = 0, detail_length = 0, i, x, y, var_type, found_str = 0;
    int if_rc, parser_status = 0, l_in_for = 0, l_in_if = 0;
    int index_type, x_index, y_index, else_offset = 0, found_ifs = 0;
    FILE *tmp_stdout = stdout;

    tmp_buff = strtok(cmd_buff, "\n");

    if_block = malloc(sizeof(char*));
    ((char**)if_block)[0] = malloc(strlen(tmp_buff)+1);
    strcpy(((char**)if_block)[0], tmp_buff);
    block_length++;


    while(1)
    {
        if((tmp_buff = strtok(NULL, "\n")) == NULL)
        {
            break;
        }
        block_length++;

        if_block = realloc(if_block, sizeof(char*)*block_length);
        ((char**)if_block)[block_length-1] = malloc(strlen(tmp_buff)+1);
        strcpy(((char**)if_block)[block_length-1], tmp_buff);
    }

    //For Befehl vorne und hinten von Leerzeichen befreien
    if_cmd = ((char**)if_block)[0];
    TrimSpaces(if_cmd);
    if_cmd = StripTrailingSpaces(if_cmd);

    if_cmd_detail = malloc(sizeof(char*));

    free(tmp_buff);
    tmp_buff = malloc(1);
    tmp_buff[0] = '\0';

    for(i=0; i < strlen(if_cmd); i++)
    {
        if(if_cmd[i] == '"')
        {
            found_str = 1 - found_str;
        }
        if(if_cmd[i] == ' ' && found_str == 0)
        {
            if((((char**)if_cmd_detail)[detail_length] = malloc(strlen(tmp_buff)+1)) == NULL)
            {
                sprintf(error_str, "Error while allocate memory. detail_length=[%d]", detail_length);
                return(-1);
            }
            memcpy(((char**)if_cmd_detail)[detail_length], tmp_buff, strlen(tmp_buff)+1);

            free(tmp_buff);

            tmp_buff = malloc(1);

            tmp_buff[0] = '\0';

            detail_length++;

            if_cmd_detail = realloc(if_cmd_detail, sizeof(char*)*(detail_length+1));
        }
        else
        {
            tmp_buff = realloc(tmp_buff, strlen(tmp_buff)+2);
            char_strcat(tmp_buff, if_cmd[i]);
        }
    }

    //Letzen Token auslesen
    ((char**)if_cmd_detail)[detail_length] = malloc(strlen(tmp_buff)+1);
    strcpy(((char**)if_cmd_detail)[detail_length], tmp_buff);
    free(tmp_buff);
    detail_length++;

    if((if_rc = compare(anker, ((char**)if_cmd_detail)[1], ((char**)if_cmd_detail)[3],
        ((char**)if_cmd_detail)[2], y, error_str)) < 0)
    {
        return(-4);
    }
    else if(if_rc == 0)
    {
        found_ifs = 0;
        for(i=1; i < block_length; i++)
        {
            if(strstr(((char**)if_block)[i], "end-if") != NULL)
            {
                found_ifs--;
                continue;
            }
            else if(strstr(((char**)if_block)[i], "if") != NULL)
            {
                found_ifs++;
                continue;
            }
            if(strstr(((char**)if_block)[i], "else") != NULL && found_ifs == 0)
            {
                else_offset = i;
                break;
            }
        }
    }
    else
    {
        else_offset = 1;
    }
    
    found_ifs = 0;

    for(i=else_offset; i < block_length; i++)
    {
        line = malloc(strlen(((char**)if_block)[i])+2);
        strcpy(line, ((char**)if_block)[i]);
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
        if(parse_line(anker, line, p_output, &l_cmd_buff,
                   &parser_status, &l_in_for, &l_in_if, error_str) < 0)
        {
            free(line);
            return(-10);
        }
        free(line);
        //fprintf(p_output, "\n");

    }

    /*for(y=1; y < i; y=y+3) 
    {
        if(compare(anker, ((char**)if_cmd_detail)[y], ((char**)if_cmd_detail)[y+2],
            ((char**)if_cmd_detail)[y+1], y, error_str) < 0)
        {
            return(-4);
        }
    }*/
    return(0);
}
