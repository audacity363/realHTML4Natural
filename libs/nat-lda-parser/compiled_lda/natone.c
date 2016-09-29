#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "lda_parser.h"

#define SAG_COMMENT_STR            "/*C  "
#define SAG_DEFINE_STR             "/*DF "
#define SAG_DEFINE_CONST_STR       "/*DFC"
#define SAG_GROUP_MEMBER_STR       "/*DK "
#define SAG_GROUP_HEAD_STR         "/*DS "
#define SAG_REDEFINE_STR           "/*DRR"
#define SAG_REDEFINED_VALUES_STR   "/*DKR"

int getdefinetype(char *line)
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
    else
        return(-1);
}

void natone_parser(struct varnames *anker, struct status_p *status, char *line)
{
    
    char var_length[11];
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

    printf("[%s]\n", line);

    switch(getdefinetype(line))
    {
        case SAG_COMMENT:
//            printf("Comment\n");
            return;
        case SAG_DEFINE:
            printf("normal define\n");
            break;
        case SAG_DEFINE_CONST:
            printf("const define\n");
            break;
        case SAG_GROUP_MEMBER:
            printf("Group Member\n");
            break;
        case SAG_GROUP_HEAD:
            printf("Group leader\n");
            vartype = GROUP_LEADER;
            break;
        case SAG_REDEFINE:
            printf("Redefine\n");
            vartype = REDEFINE;
            break;
        case SAG_REDEFINED_VALUES:
            printf("Rederfined values\n");
            break;
        default:
//            printf("Unkown Define Type\n");
            return;
    }
    line = line + 24;

    switch(line[0])
    {
        case SAG_ALPHA_CHAR:
            printf("Alphanumeric\n");
            vartype = ALPHA;
            break;
        case SAG_INTEGER_CHAR:
            printf("Integer\n");
            vartype = INTEGER;
            break;
        case SAG_LOGIC_CHAR:
            printf("Logical (Integer)\n");
            vartype = INTEGER;
            break;
        case SAG_NUMERIK_CHAR:
            printf("Numeric\n");
            vartype = NUMERIC;
            break;
        case SAG_CONTROL_CHAR:
            printf("Control\n");
            break;
        default:
            if(vartype == -1)
             {
                printf("Unkown Type\n");
                return;
             }
    }

    var_length[10] = '\0';

    memcpy(var_length, line+1, sizeof(var_length));
    striptrailspace(var_length);

    comma_pos = index(var_length, ',');;

    if(comma_pos != NULL && vartype == NUMERIC)
    {
        comma_pos[0] = '\0';

        i_var_length = atoi(var_length);
        i_var_length_comma = atoi(comma_pos+1);
        printf("[%d] [%d]\n", i_var_length, i_var_length_comma);
    }
    else if(strncmp(var_length, SAG_DYNAMIC, sizeof(SAG_DYNAMIC)-1) == 0)
    {
        printf("Length: DYNAMIC\n");
        i_var_length = DYNAMIC;
    }
    else
    {

        i_var_length = atoi(var_length);
        printf("Length: %d\n", i_var_length);
    }

    line = line +13;
    var_level[1] = '\0';
    var_level[0] = line[0];

    level = atoi(var_level);

    printf("Level: [%d]\n", level);

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
        printf("\n\n");
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
    printf("X Length: [%d]\n", x_array);
    is_array = ARRAY_1D;

    if(array_save[1] != NULL)
    {
        tmp_ptr = strtok(array_save[1], ":");
        tmp_ptr = strtok(NULL, ":");
        y_array = atoi(tmp_ptr);
        printf("Y Length: [%d]\n", y_array);
        is_array = ARRAY_2D;
    }


save:
    if(status->foundpage_grp == false)
        return;
    
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
