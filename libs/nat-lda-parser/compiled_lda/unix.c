#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "lda_parser.h"

#define SAG_COMMENT_STR          "**C  "
#define SAG_DEFINE_STR           "**DF "
#define SAG_DEFINE_CONST_STR     "**DFC"
#define SAG_GROUP_MEMBER_STR     "**DK "
#define SAG_GROUP_HEAD_STR       "**DS "
#define SAG_REDEFINE_STR         "**DR"
#define SAG_REDEFINED_VALUES_STR "**DFR"
#define SAG_INIT_STR             "**I"
#define SAG_PRE_INIT_STR         "**HF1"
#define SAG_DYNAMIC_DF_STR       "**DE"

int getdefinetype_unix(char *line)
{
    if(strncmp(line, SAG_COMMENT_STR, sizeof(SAG_COMMENT_STR)-1) == 0)
        return(SAG_COMMENT);
    else if(strncmp(line, SAG_DEFINE_STR, sizeof(SAG_DEFINE_STR)-1) == 0)
        return(SAG_DEFINE);
    else if(strncmp(line, SAG_DEFINE_CONST_STR, sizeof(SAG_DEFINE_CONST_STR)-1) == 0)
        return(SAG_DEFINE_CONST);
    else if(strncmp(line, SAG_GROUP_MEMBER_STR, sizeof(SAG_GROUP_MEMBER_STR)-1) == 0)
        return(SAG_GROUP_MEMBER);
    else if(strncmp(line, SAG_GROUP_HEAD_STR, sizeof(SAG_GROUP_HEAD_STR)-1) == 0)
        return(SAG_GROUP_HEAD);
    else if(strncmp(line, SAG_REDEFINE_STR, sizeof(SAG_REDEFINE_STR)-1) == 0)
        return(SAG_REDEFINE);
    else if(strncmp(line, SAG_REDEFINED_VALUES_STR, sizeof(SAG_REDEFINED_VALUES_STR)-1) == 0)
        return(SAG_REDEFINED_VALUES);
    else if(strncmp(line, SAG_INIT_STR, sizeof(SAG_INIT_STR)-1) == 0)
        return(SAG_INIT);
    else if(strncmp(line, SAG_PRE_INIT_STR, sizeof(SAG_PRE_INIT_STR)-1) == 0)
        return(SAG_PRE_INIT);
    else if(strncmp(line, SAG_DYNAMIC_DF_STR, sizeof(SAG_DYNAMIC_DF_STR)-1) == 0)
        return(SAG_DYNAMIC_DF);
    else
        return(-1);
}

void unix_parser(struct varnames *anker, struct status_p *status, char *line)
{
    
    char var_length[4];
    char *comma_pos;
    int i_var_length, level, is_array, vartype;
    int i_var_length_comma = -1;
    int x_array, y_array = -1;
    char var_level[2];
    char var_name[33];
    char *array_save[3];
    char *tmp_ptr;
    int i;

    struct varnames *hptr, *ptr, *hhptr;
    vartype = -1;
    is_array = 0;

    if(line[strlen(line)-2] == '\r')
        line[strlen(line)-2] = '\0';
    else if(line[strlen(line)-1] == '\n')
        line[strlen(line)-1] = '\0';

    line = line+4;

    switch(getdefinetype_unix(line))
    {
        case SAG_COMMENT:
            return;
        case SAG_DYNAMIC_DF:
        case SAG_INIT:
        case SAG_PRE_INIT:
            return;
        case SAG_DEFINE:
            break;
        case SAG_DEFINE_CONST:
            break;
        case SAG_GROUP_MEMBER:
            break;
        case SAG_GROUP_HEAD:
            vartype = GROUP_LEADER;
            break;
        case SAG_REDEFINE:
            vartype = REDEFINE;
            break;
        case SAG_REDEFINED_VALUES:
            break;
        default:
            return;
    }
    line = line + 18;

    switch(line[0])
    {
        case SAG_ALPHA_CHAR:
            vartype = ALPHA;
            break;
        case SAG_NUMERIK_CHAR:
            vartype = NUMERIC;
            break;
        case SAG_INTEGER_CHAR:
            vartype = NUMERIC;
            break;
        case SAG_CONTROL_CHAR:
            break;
        default:
            if(vartype == -1)
             {
                return;
             }
    }

    var_length[3] = '\0';

    memcpy(var_length, line+1, sizeof(var_length));
    striptrailspace(var_length);

    comma_pos = index(var_length, ',');;

    if(comma_pos != NULL && vartype == NUMERIC)
    {
        comma_pos[0] = '\0';

        i_var_length = atoi(var_length);
        i_var_length_comma = atoi(comma_pos+1);
    }
    else
    {

        i_var_length = atoi(var_length);
        if(i_var_length == 0 && vartype == ALPHA)
            i_var_length = DYNAMIC;
    }

    line = line +6;
    var_level[1] = '\0';
    var_level[0] = line[0];

    level = atoi(var_level);


    line = line +1;

    for(i=0; i < 32 && i != strlen(line); i++)
    {
        if(line[i] == ' ')
            break;
        var_name[i] = line[i];
    }

    var_name[i] = '\0';

    if(strcmp(var_name, "PAGE") == 0 || strcmp(var_name, "page") == 0)
        status->foundpage_grp = true;

    line = line +i;
    if(strlen(line) == 0)
    {
        //Kein Array
        if(status->foundpage_grp == false)
            return;
        goto save;
    }


    striptrailspace(line);
    line = line +1;
    line[strlen(line)-1] = '\0';

    array_save[0] = strtok(line, ",");
    array_save[1] = strtok(NULL, ",");
    if(strtok(NULL, ",") != NULL)
    {
        fprintf(stderr, "3D Arrays are not Supported yet\n");
        return;
    }

    tmp_ptr = strtok(array_save[0], ":");
    tmp_ptr = strtok(NULL, ":");
    x_array = atoi(tmp_ptr);
    is_array = ARRAY_1D;

    if(array_save[1] != NULL)
    {
        tmp_ptr = strtok(array_save[1], ":");
        tmp_ptr = strtok(NULL, ":");
        y_array = atoi(tmp_ptr);
        is_array = ARRAY_2D;
    }

    if(status->foundpage_grp == false)
        return;

save:
    /*if(status->foundpage_grp == false)
        return;
    */
    hptr = malloc(sizeof(struct varnames));
    if(hptr == NULL)
    {
        fprintf(stderr, "Can not allocate memory for new entry [%s]\n", 
                strerror(errno));
        return;
    }

    hptr->level = level;
    strcpy(hptr->name, var_name);
    hptr->type = vartype;
    hptr->length = i_var_length;
    hptr->length_comma = i_var_length_comma;
    hptr->x_length = x_array;
    hptr->y_length = y_array;
    hptr->array = is_array;

    if(vartype == REDEFINE)
    {
        strcpy(hptr->rename, var_name);
    }

    hptr->nextlevel = NULL;
    hptr->next = NULL;
    hptr->prev = NULL;

    if(status->cur_dataset->level == hptr->level)
    {
        hptr->prev = status->cur_dataset;
        status->cur_dataset->next = hptr;
        status->cur_dataset = hptr;
    }
    else if(status->cur_dataset->level > hptr->level)
    {
        hhptr = status->cur_dataset;
        while(hhptr != NULL)
        {
            if(hhptr->level == hptr->level)
                break;
            hhptr = hhptr->prev;
        }
        if(hhptr == NULL)
        {
            fprintf(stderr, "Level nicht gefunden\n");
            return;
        }

        while(hhptr->next != NULL)
        {
            hhptr = hhptr->next;
        }

        hptr->prev = hhptr;
        hhptr->next = hptr;
        status->cur_dataset = hptr;
    }
    else if(status->cur_dataset->level < hptr->level)
    {
        hptr->prev = status->cur_dataset;
        status->cur_dataset->nextlevel = hptr;
        status->cur_dataset = hptr;
    }

}
