#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "utils.h"

int for_getArrayLength(struct variables *anker, char *name, int var_type,
                   int index_type, int x_index, int y_index, char *error_str)
{
    char *c_value;
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

int createTmpVar(struct variables *anker, char *tmp_name, char *variable,
                int var_type, int index_type, int x_index, int y_index,
                char *error_str)
{
    char *c_value, tmp_str[2];
    int i_value;
    int i, x, y, length;


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

int fillTmpVar(struct variables *anker, char *tmp_name, char *variable,
                int var_type, int index_type, int x_index, int y_index,
                int for_index, char *error_str)
{
    char *c_value, *tmp_str = NULL;
    int i_value, x, y, length, i, rc;

    if(var_type == STRING)
    {
        tmp_str = malloc(2);
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
            tmp_str = malloc(2);
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
            tmp_str = malloc(2);
            tmp_str[1] = '\0';
            tmp_str[0] = c_value[for_index];
            editStringVar(anker, tmp_name, tmp_str);
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

    if(tmp_str != NULL)
    {
        free(tmp_str);
    }
    return(0);
}

int for_handle(struct variables *anker, char *cmd_buff, FILE *p_output,
               char *error_str)
{
    char *for_block, *tmp_buff, *for_cmd, *for_cmd_detail;
    int block_length = 0, i, x, for_index, arg_length, var_type;
    int index_type, x_index, y_index;
    char *l_cmd_buff = NULL;
    int parser_status = 0;
    char *line;
    int l_in_for = 0, l_in_if = 0;

    tmp_buff = strtok(cmd_buff, "\n");

    for_block = malloc(sizeof(char*));
    ((char**)for_block)[0] = malloc(strlen(tmp_buff));
    strcpy(((char**)for_block)[0], tmp_buff);
    block_length++;


    while(1)
    {
        if((tmp_buff = strtok(NULL, "\n")) == NULL)
        {
            break;
        }
        block_length++;

        for_block = realloc(for_block, sizeof(char*)*block_length);
        ((char**)for_block)[block_length-1] = malloc(strlen(tmp_buff));
        strcpy(((char**)for_block)[block_length-1], tmp_buff);
    }


    //For Befehl vorne und hinten von Leerzeichen befreien
    for_cmd = ((char**)for_block)[0];
    TrimSpaces(for_cmd);
    for_cmd = StripTrailingSpaces(for_cmd);

    //for Token
    if((tmp_buff = strtok(for_cmd, " ")) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing Token");
        return(-1);
    }

    for_cmd_detail = malloc(sizeof(char*));
    ((char**)for_cmd_detail)[0] = malloc(strlen(tmp_buff));
    strcpy(((char**)for_cmd_detail)[0], tmp_buff);
    
    for(i=1; i < 4;i++)
    {
        if((tmp_buff = strtok(NULL, " ")) == NULL)
        {
            strcpy(error_str, "Syntax Error: Missing Token");
            return(-1);
        }
        if((for_cmd_detail = realloc(for_cmd_detail, sizeof(char*)*(i+1))) == NULL)
        {
            strcpy(error_str, strerror(errno));
            return(-2);
        }
        ((char**)for_cmd_detail)[i] = malloc(strlen(tmp_buff)+1);
        memcpy(((char**)for_cmd_detail)[i], tmp_buff, strlen(tmp_buff)+1);
    }

    if(strcmp(((char**)for_cmd_detail)[2], "in") == 0)
    {
        if((index_type = getIndex(anker, ((char**)for_cmd_detail)[3],
                                  &x_index, &y_index, error_str)) < 0)
        {
            return(-3);
        }
        if((var_type = getVarType(anker, ((char**)for_cmd_detail)[3])) < 0)
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

    if((arg_length = for_getArrayLength(anker, ((char**)for_cmd_detail)[3],
            var_type, index_type, x_index, y_index, error_str)) < 0)
    {
        return(-9);
    }

  
    //loop.i nur anlegen wenn sie noch nicht existiert
    if(getVarType(anker, "loop.i") == -1)
    {
        newIntVar(anker, "loop.i", 0);
    }

    createTmpVar(anker, ((char**)for_cmd_detail)[1], ((char**)for_cmd_detail)[3],
                var_type, index_type, x_index, y_index,
                error_str);

    for(i=0; i < arg_length; i++)
    {
        fillTmpVar(anker, ((char**)for_cmd_detail)[1], ((char**)for_cmd_detail)[3],
                var_type, index_type, x_index, y_index,
                i, error_str);

        for(x=1; x < block_length; x++)
        {
            editIntVar(anker, "loop.i", i);
            line = malloc(strlen(((char**)for_block)[x]));
            strcpy(line, ((char**)for_block)[x]);
            if(parse_line(anker, line, p_output, &l_cmd_buff,
                       &parser_status, &l_in_for, &l_in_if, error_str) < 0)
            {
                free(line);
                return(-10);
            }
            free(line);
            fprintf(p_output, "\n");
        }
    }

    return(0);
}
