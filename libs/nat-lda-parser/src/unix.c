#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "lda.h"

#define SAG_COMMENT_STR          "C  "
#define SAG_DEFINE_STR           "DF "
#define SAG_DEFINE_CONST_STR     "DFC"
#define SAG_GROUP_MEMBER_STR     "DK "
#define SAG_GROUP_HEAD_STR       "DS "
#define SAG_REDEFINE_STR         "DR "
#define SAG_REDEFINED_VALUES_STR "DFR"
#define SAG_INIT_STR             "I  "
#define SAG_PRE_INIT_STR         "HF1"
#define SAG_DYNAMIC_DF_STR       "DE "

int unix_getDefinitionType(char *);
int unix_getVariablenType(char);
int parseArrayIndex(char *, int*, int *);
void trimTrailingSpaces(char *name);
void updateVarType(vars_t *target, int index_type, int *index_length);

vars_t *cur_pos = NULL;
vars_t *grp_head = NULL;

int unix_lda(char *line, vars_t *anker)
{
    char *line_begin = NULL,
          c_var_type = 0,
          before_comma[6],
          after_comma[4],
          tmp_name[LEN_NAME+1];

    bool found_comma = false;

    vars_t *cur = malloc(sizeof(vars_t)),
           *hptr = NULL;

    int definition_type = 0,
        i = 0,
        x = 0,
        level = -1,
        rest_length = 0,
        index_type = 0,
        array_length[3],
        sag_array_index = -1;

    if(cur_pos == 0x00)
        cur_pos = anker;

    line_begin = line;

    //Offset for the line numbers
    line+=4;
    //Offset for the lda type identifier
    line+=2;


    if((definition_type = unix_getDefinitionType(line)) == -1)
    {
        fprintf(stderr, "Unkown definition type [%4.4s]\n", line);
        return(-1);
    }

    //Check if we can ignore this type
    if(definition_type == SAG_COMMENT ||
       definition_type == SAG_DYNAMIC_DF)
        return(CONTINUE);

    line+=3;

    if(line[0] == 'I')
    {
        sag_array_index = line[1] - 0x30;
        printf("It is an Array. SAG index: [%d]\n", sag_array_index);
    }
    //I dont know for what this section is for. Just ignore is for the moment
    line+=13;

    //Get var type
    c_var_type = line[0];
    line++;

    if(definition_type != SAG_GROUP_HEAD)
    {
        if((cur->type = unix_getVariablenType(c_var_type)) == TYPE_UNSUPPORTED)
        {
            fprintf(stderr, "Variablen Type not supported\n");
            return(BREAK);
        }
        else if(cur->type == -2)
        {
            fprintf(stderr, "Unkown Variablentype: [%c]\n", c_var_type);
            return(BREAK);
        }

        //check length parms
        x=0;
        memset(before_comma, 0x00, sizeof(before_comma));
        memset(after_comma, 0x00, sizeof(after_comma));
        for(i=0; i < 5; i++)
        {
            //Ignore spaces
            if(line[i] == 0x20)
                continue;
            else if(line[i] == ',')
            {
                found_comma = true;
                x=0;
                continue;
            }
            if(found_comma)
            {
                after_comma[x] = line[i];
                x++;
            }
            else
            {
                before_comma[x] = line[i];
                x++;
            }
        }    
       
        if(found_comma && cur->type == INTEGER)
            cur->type = FLOAT;

        line+=5;
    }
    else
    {
        //Groups dont have length parms. Just jump over them
        line+=5;
        cur->type = GROUP;
        printf("Group\n");
    }

    //convert the char level to an integer
    cur->level = line[0] - 0x30;
    line++;

    memset(tmp_name, 0x00, sizeof(tmp_name));
    if((rest_length = strlen(line)) >= LEN_NAME)
    {
        memcpy(tmp_name, line, LEN_NAME);
        line+=LEN_NAME;
    }
    else
    {
        memcpy(tmp_name, line, rest_length+1);
        line+=rest_length;
    }

    trimTrailingSpaces(tmp_name);
    printf("name: [%s]\n", tmp_name);

    if(strlen(line) > 0)
    {
        //The variable is an array
        parseArrayIndex(line, &index_type, array_length);
        for(i=0; i < index_type; i++)
        {
            printf("Index: [%d]\n", array_length[i]);
        }

        updateVarType(cur, index_type, array_length);
        if(index_type == 1 && sag_array_index == 1)
        {
            cur->x_length = array_length[0];
            cur->y_length = -1;
            cur->z_length = -1;
        }
        else if(index_type == 1 && sag_array_index == 2)
        {
            cur->x_length = -1;
            cur->y_length = array_length[0];
            cur->z_length = -1;
            
        }
        else if(index_type == 1 && sag_array_index == 3)
        {
            cur->x_length = -1;
            cur->y_length = -1;
            cur->z_length = array_length[0];
            
        }
        else if(index_type == 2 && sag_array_index == 2)
        {
            cur->x_length = array_length[0];
            cur->y_length = array_length[1];
            cur->z_length = -1;
        }
        else if(index_type == 3 && sag_array_index == 3)
        {
            cur->x_length = array_length[0];
            cur->y_length = array_length[1];
            cur->z_length = array_length[2];
        }
        else
        {
            cur->x_length = -1;
            cur->y_length = -1;
            cur->z_length = -1;
        }
    }
    else
    {
        cur->x_length = -1;
        cur->y_length = -1;
        cur->z_length = -1;
    }

    if(cur->type == GROUP)
    {
        grp_head = cur;
    }
    else if(cur->type != GROUP && grp_head != NULL)
    {
        index_type = 0;
        if(grp_head->x_length != -1)
        {
            index_type = 1;
            cur->x_length = grp_head->x_length;
        }
        if(grp_head->y_length != -1)
        {
            index_type = 2;
            cur->y_length = grp_head->y_length;
        }
        if(grp_head->z_length != -1)
        {
            index_type = 3;
            cur->z_length = grp_head->z_length;
        }
        updateVarType(cur, index_type, array_length);

    }

    //Save the current dataset
    cur->name = malloc(strlen(tmp_name)+1);
    strcpy(cur->name, tmp_name);

    if(cur->level > cur_pos->level)
    {
        cur_pos->next_lvl = cur;
        cur->prev = cur_pos;
        cur_pos = cur;
    }
    else if(cur->level == cur_pos->level)
    {
        cur_pos->next = cur;
        cur->prev = cur_pos;
        cur_pos = cur;
    }
    else if(cur->level < cur_pos->level)
    {
        hptr = cur_pos;
        while(hptr->level != cur->level)
            hptr = hptr->prev;
        hptr->next = cur;
        cur->prev = hptr;
        cur_pos = cur;
    }

    cur_pos = cur;
    printf("\n");
    return(0);
}

/*
 * returns the type of the definition
 */
int unix_getDefinitionType(char *line)
{
    if(memcmp(line, SAG_COMMENT_STR, 3) == 0)
        return(SAG_COMMENT);
    else if(memcmp(line, SAG_DEFINE_STR, 3) == 0)
        return(SAG_DEFINE);
    else if(memcmp(line, SAG_DEFINE_CONST_STR, 3) == 0)
        return(SAG_DEFINE_CONST);
    else if(memcmp(line, SAG_GROUP_MEMBER_STR, 3) == 0)
        return(SAG_GROUP_MEMBER);
    else if(memcmp(line, SAG_GROUP_HEAD_STR, 3) == 0)
        return(SAG_GROUP_HEAD);
    else if(memcmp(line, SAG_REDEFINE_STR, 3) == 0)
        return(SAG_REDEFINE);
    else if(memcmp(line, SAG_REDEFINED_VALUES_STR, 3) == 0)
        return(SAG_REDEFINED_VALUES);
    else if(memcmp(line, SAG_INIT_STR, 3) == 0)
        return(SAG_INIT);
    else if(memcmp(line, SAG_PRE_INIT_STR, 3) == 0)
        return(SAG_PRE_INIT);
    else if(memcmp(line, SAG_DYNAMIC_DF_STR, 3) == 0)
        return(SAG_DYNAMIC_DF);
    return(-1);
}

/*
 * returns the integer representation of the variablen type
 * returns -1 = not supported
 * returns -2 = unkown type
 */
int unix_getVariablenType(char type)
{
    if(type == TYPE_ALPHA)
        return(STRING);
    else if(type == TYPE_BIN)
        return(TYPE_UNSUPPORTED);
    else if(type == TYPE_CV)
        return(TYPE_UNSUPPORTED);
    else if(type == TYPE_DATE)
        return(TYPE_UNSUPPORTED);
    else if(type == TYPE_FLOAT)
        return(FLOAT);
    else if(type == TYPE_INT)
        return(INTEGER);
    else if(type == TYPE_LOG)
        return(BOOL);
    else if(type == TYPE_NUM)
        return(INTEGER);
    else if(type == TYPE_OBJECT)
        return(TYPE_UNSUPPORTED);
    else if(type == TYPE_PACK)
        return(TYPE_UNSUPPORTED);
    else if(type == TYPE_TIME)
        return(TYPE_UNSUPPORTED);
    else if(type == TYPE_UNICODE)
        return(STRING);

    return(-2);
}

void trimTrailingSpaces(char *name)
{
    int length = strlen(name),
        i = 0;

    for(i=0; i <= strlen(name); i++)
    {
        if(name[i] == 0x20 || name[i] == '\n')
        {
            name[i] = '\0';
            return;
        }
    }
    return;
}

int parseArrayIndex(char *line, int *index_type, int *index_length)
{
    char c_index[3][10];
    char tmp_buff[20];

    int i = 0;
    bool found_colon = false;
    //Jump over open bracket
    line++;
    *index_type = 0;

    while(strlen(line) != 0)
    {
        if(line[0] == ')')
        {
            //Found end of index
            tmp_buff[i] = '\0';
            index_length[*index_type] = atoi(tmp_buff);
            (*index_type)++;
            break;
        }
        else if(line[0] == ',')
        {
            //End off index
            tmp_buff[i] = '\0';
            index_length[*index_type] = atoi(tmp_buff);
            (*index_type)++;
            i=0;
        }
        else if(line[0] == ':')
        {
            i=0;
        }
        else
        {
            tmp_buff[i] = line[0];
            i++;
        }
        
        line++;
    }
    return(0);
}

void updateVarType(vars_t *target, int index_type, int *index_length)
{
    if(index_type == 1)
    {
        switch(target->type)
        {
            case STRING:
                target->type = ONEDSTRING;
                break;
            case ONEDSTRING:
                target->type = TWODSTRING;
                break;
            case TWODSTRING:
                target->type = THREEDSTRING;
                break;
            case INTEGER:
                target->type = ONEDINTEGER;
                break;
            case ONEDINTEGER:
                target->type = TWODINTEGER;
                break;
            case TWODINTEGER:
                target->type = THREEDINTEGER;
                break;
            case BOOL:
                target->type = ONEDBOOL;
                break;
            case ONEDBOOL:
                target->type = TWODBOOL;
                break;
            case TWODBOOL:
                target->type = THREEDBOOL;
                break;
            case FLOAT:
                target->type = ONEDFLOAT;
                break;
            case ONEDFLOAT:
                target->type = TWODFLOAT;
                break;
            case TWODFLOAT:
                target->type = THREEDFLOAT;
                break;
        }
    }
    else if(index_type == 2)
    {
        switch(target->type)
        {
            case STRING:
                target->type = TWODSTRING;
                break;
            case ONEDSTRING:
                target->type = THREEDSTRING;
                break;
            case INTEGER:
                target->type = TWODINTEGER;
                break;
            case ONEDINTEGER:
                target->type = THREEDINTEGER;
                break;
            case BOOL:
                target->type = TWODBOOL;
                break;
            case ONEDBOOL:
                target->type = THREEDBOOL;
                break;
            case FLOAT:
                target->type = TWODFLOAT;
                break;
            case ONEDFLOAT:
                target->type = THREEDFLOAT;
                break;
        }
    }
    else if(index_type == 3)
    {
        switch(target->type)
        {
            case STRING:
                target->type = THREEDSTRING;
                break;
            case INTEGER:
                target->type = THREEDINTEGER;
                break;
            case BOOL:
                target->type = THREEDBOOL;
                break;
            case FLOAT:
                target->type = THREEDFLOAT;
                break;
        }
    }
  
}

