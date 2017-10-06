//LDA Offset 72
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "vars.h"
#include "lda.h"

#define MAX_VARNAME_LENGTH 32
#define MAX_LENGTH_CHARS_LENGTH 6

#define LDA_DEFINE_DATA "DEFINE DATA LOCAL "
#define LDA_END_DEFINED_DATA "END-DEFINE "
#define LDA_REDEFINE "REDEFINE"
#define LDA_VIEW "VIEW OF"
#define LDA_CONST "CONST"
#define LDA_INIT "INIT"
#define LDA_HEADER_OFFSET 72

#define COMPARE_HEAD_STR(x) strcmp(x, LDA_DEFINE_DATA) == 0
#define COMPARE_FOOT_STR(x) strcmp(x, LDA_END_DEFINED_DATA) == 0
#define COMPARE_REDEFINE_STR(x) strncmp(x, LDA_REDEFINE, sizeof(LDA_REDEFINE)) == 0
#define CHECK_IF_VIEW(x) strstr(x, LDA_VIEW) != NULL
#define CHECK_IF_CONST(x) strstr(x, LDA_CONST) != NULL
#define CHECK_IF_INIT(x) strstr(x, LDA_INIT) != NULL

FILE *logfile;
char *error_str;


#ifdef LOGDEBUG
int debug_int = 1;
#define D(x) if (!debug_int); else x; fflush(logfile);
#else
#define D(x)
#endif


char *getVariablenLength(char *line, int *length, int type);
char *getArrayType(char *line, int *array_type, int *index);
char *my_strtok(char *str, char delmiter);
void updateVarType(vars_t *target, int index_type);
int getVariablenType(char type);

vars_t *cur_pos = NULL;
vars_t *grp_head = NULL;
int grp_head_index_type = -1;

int startLDAParser(char *ldapath, vars_t *anker, FILE *logfile_l, char *error_buffer)
{
    anker->level = 1;

    FILE *fp = NULL;

    char header[72], cur_chr, *complete_line = NULL;
    int length = 0, ret = 0;

    logfile = logfile_l;
    error_str = error_buffer;

    if((fp = fopen(ldapath, "r")) == NULL)
    {
        sprintf(error_str, "Error opening [%s]\nError: [%s]\n", ldapath, strerror(errno));
        fprintf(logfile, "%s\n", error_str);
        fflush(logfile);
        return(-1);
    }


    //Jump over the LDA header
    fread(header, LDA_HEADER_OFFSET, 1, fp);

    complete_line = malloc(1);

    while((cur_chr = fgetc(fp)) != EOF)
    {
        if(cur_chr == 0x00)
        {
            complete_line[length] = 0x00;
            if((ret = parseLDALine(complete_line, length, anker)) == EXIT)
            {
                sprintf(error_str, "%sSomething went wrong while parsing [%s]\n", error_str, ldapath);
                fprintf(logfile, "%s", error_str);
                fflush(logfile);
                fclose(fp);
                free(complete_line);
                cur_pos = NULL;
                return(-2);
            }
            else if(ret == BREAK)
            {
                break;
            }
            free(complete_line);
            complete_line = NULL;
            complete_line = malloc(1);
            length = 0;
        }
        else if(cur_chr == 0x1E)
            break;
        else
        {
            complete_line[length++] = cur_chr;
            if((complete_line = realloc(complete_line, length+1)) == NULL)
            {
                sprintf(error_str, "%sSomething went wrong while parsing [%s]\nError: [%s]\n", 
                    error_str, ldapath, strerror(errno));
                fprintf(logfile, "%s", error_str);
                fflush(logfile);
                cur_pos = NULL;
                return(-2);
            }
        }
    }

    D(printfork(anker, logfile));

    free(complete_line);
    fclose(fp);

    cur_pos = NULL;

    return(0);
}

int parseLDALine(char *complete_line, int length, vars_t *anker)
{
    int level = -1, varname_length = 0,
        vartype = -1, i = 0, var_length = -3,
        index[3] = {-1, -1, -1}, index_type = 0;

    char varname[MAX_VARNAME_LENGTH],
         *line = complete_line;

    //Views are not allowed
    if(CHECK_IF_VIEW(line))
    {
        sprintf(error_str, "Views are not allowed in the LDA\n");
        return(EXIT);
    }
    else if(CHECK_IF_CONST(line))
    {
        return(CONTINUE);
    }
    else if(CHECK_IF_INIT(line))
    {
        return(CONTINUE);
    }



    D(fprintf(logfile, "[%s]\n", line));

    vars_t *cur = malloc(sizeof(vars_t)),
           *hptr = NULL;

    cur->next = NULL;
    cur->prev = NULL;
    cur->next_lvl = NULL;

    if(cur_pos == 0x00)
        cur_pos = anker;

    //Ignore the line number
    line+=5;
    if(COMPARE_HEAD_STR(line))
    {
        return(0);
    }
    else if(COMPARE_FOOT_STR(line))
    {
        return(BREAK);
    }
    for(;*line == 0x20; line++);
       
    //line++;

    D(fprintf(logfile, "Check first char in line\n"));

    //Check if it is a init value
    if(*line == '<')
        return(CONTINUE);
    //Check if it is an array init value
    if(*line == '(')
        return(CONTINUE);
    //ignore comments
    if(*line == '*')
        return(CONTINUE);

    level = line[0] - 0x30;
    //printf("level: [%d]\n", level);
    cur->level = level;

    line+=2;

    while(line[0] != 0x20 && strlen(line) > 0)
    {
        varname[varname_length++] = line[0];
        line++;
    }
    varname[varname_length] = 0x00;

    for(;line[0] == 0x20 && strlen(line) > 0; line++);

    //It is a new group without an array
    if(strlen(line) == 0)
    {
        D(fprintf(logfile, "New Group\n\n"));

        cur->name = malloc(strlen(varname)+1);
        strcpy(cur->name, varname);

        cur->type = GROUP;
        goto save;
    }

    if(COMPARE_REDEFINE_STR(varname))
    {
        D(fprintf(logfile, "It is a REDEFINE\n\n"));

        //Get the name from the redefined variable
        varname_length = 0;
        while(line[0] != 0x20 && strlen(line) > 0)
        {
            varname[varname_length++] = line[0];
            line++;
        }
        varname[varname_length] = 0x00;

        cur->name = malloc(strlen(varname)+1);
        strcpy(cur->name, varname);

        cur->type = REDEFINE;
        goto save;
    }

    D(fprintf(logfile, "Varname: [%s]\n", varname));
    cur->name = malloc(strlen(varname)+1);
    strcpy(cur->name, varname);

    if(*line != 0x28)
    {
        sprintf(error_str, "Something went totaly wrong. Unexcpected char [%c] in LDA\n", *line);
        sprintf(error_str, "%sLine: [%s]\n", error_str, line);
        return(EXIT);
    }

    line++;

    //It is a new group with array specification
    if((vartype = getVariablenType(line[0])) == -2)
    {
        D(fprintf(logfile, "New Group with Index specification\n"));
        if(getArrayType(line, &index_type, index) == NULL)
            return(EXIT);
        cur->type = GROUP;
        cur->x_length = index[0];
        cur->y_length = index[1];
        cur->z_length = index[2];

        D(printfork(cur, logfile));
        goto save;
    }
    else if(vartype == TYPE_UNSUPPORTED)
    {
        sprintf(error_str, "Variabletype [%c] currently not supported\n", *line);
        return(EXIT);
    }

    D(fprintf(logfile, "vartype: [%d]\n", vartype));
    cur->type = vartype;

    line++;

    line = getVariablenLength(line, &var_length, vartype);
    cur->length = var_length;


    if(*line == '/')
    {
        if(getArrayType(++line, &index_type, index) == NULL)
            return(EXIT);
        cur->x_length = index[0];
        cur->y_length = index[1];
        cur->z_length = index[2];
        updateVarType(cur, index_type);
    }
    else
    {
        cur->x_length = -1;
        cur->y_length = -1;
        cur->z_length = -1;
    }

save:
    if(cur->level > cur_pos->level)
    {
        D(fprintf(logfile, "create new level cur: [%d] new: [%d]\n", cur_pos->level, cur->level));
        cur_pos->next_lvl = cur;
        cur->prev = cur_pos;
        cur_pos = cur;
    }
    else if(cur->level == cur_pos->level)
    {
        D(fprintf(logfile, "same level\n"));
        cur_pos->next = cur;
        cur->prev = cur_pos;
        cur_pos = cur;
    }
    else if(cur->level < cur_pos->level)
    {
        D(fprintf(logfile, "Search prev level [%d] cur level [%d]\n", cur->level, cur_pos->level));
        hptr = cur_pos;
        while(hptr->level != cur->level)
            hptr = hptr->prev;
        hptr->next = cur;
        cur->prev = hptr;
        cur_pos = cur;
    }

    D(printfork(cur, logfile));
    return(0);
}

/*
 * Parses the array specification from the Variable
 * the type of the array gets written into the array_type param
 * 0 = Not an array
 * 1 = 1D Array
 * 2 = 2D Array
 * 3 = 3D Array
 * The lower bound get completly ignored and the length gets calculated from
 * the upper bound minus the lower bound. This length gets written in the array
 * index
 */
char *getArrayType(char *line, int *array_type, int *index)
{
    *array_type = 0;
    int upperbound = -1, lowerbound = -1;

    char *end_var_params = NULL, *comma = NULL,
         *double_collon = NULL;

    if((end_var_params = strchr(line, ')')) == NULL)
    {
        fprintf(stderr, "Something went totaly wrong. Can not find array parmams end\n");
        return(NULL);
    }
    *end_var_params = 0x00;

    comma = my_strtok(line, ',');
    while(comma)
    {
        (*array_type)++;
        //Check if a lower bound is specified
        if((double_collon = strchr(comma, ':')) != NULL)
        {
            *double_collon = 0x00;
            double_collon++;
            index[(*array_type)-1] = atoi(double_collon);
        }
        else
        {
            index[(*array_type)-1] = atoi(comma);
        }
        comma = my_strtok(NULL, ',');
    }
    return(line);
}

char *savptr = NULL;
char *my_strtok(char *str, char delmiter)
{
    char *delmiter_pos = NULL, *tmp = NULL;

    if(str == NULL && savptr == NULL)
        return(NULL);

    if(str)
    {
        if((delmiter_pos = strchr(str, delmiter)) == NULL)
        {
            savptr = str+strlen(str);
            return(str);
        }
        *delmiter_pos = 0x00;
        savptr = ++delmiter_pos;
        return(str);
    }
    if(strlen(savptr) == 0)
    {
        savptr = NULL;
        return(NULL);
    }

    if((delmiter_pos = strchr(savptr, delmiter)) == NULL)    
    {
        tmp = savptr;
        savptr = savptr+strlen(savptr);
        return(tmp);
    }
    *delmiter_pos = 0x00;
    tmp = savptr;
    savptr = ++delmiter_pos;
    return(tmp);
}

/*
 * Gets the Length of the Variable
 * Return a pointer to the char behind the Length param.
 * The Length get written in length
 * length >  0: The actual length
 * length = -1: Dynamic length
 * length = -2: The variable Type dont have a length param
 */
char *getVariablenLength(char *line, int *length, int type)
{
    char var_length[MAX_LENGTH_CHARS_LENGTH];
    int i = 0;
    //Check if the Variable is from the length DYNAMIC
    if(*line == 0x2F || *line == 0x29)
    {
        //Only Unicode and Alphanumeric can have a length of Dynamic
        //just trust on Natural that the Syntax is right
        if(type == TYPE_ALPHA || type == TYPE_UNICODE)
        {
            *length = -1;
            return(line);
        }
        //The rest of the variablen type don't have a length specification
        *length = -2;
        return(line);
    }
    for(i=0; (*line != 0x2F && *line != 0x29) && strlen(line) > 0; i++) 
    {
        var_length[i] = *line;
        line++;
    }
    var_length[i] = 0x00;

    *length = atoi(var_length);

    return(line);
}

/*
 * returns the integer representation of the variablen type
 * returns -1 = not supported
 * returns -2 = unkown type
 */
int getVariablenType(char type)
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
    else if(type == TYPE_EMPTY)
        return(UNKOWN);

    return(-2);
}

void updateVarType(vars_t *target, int index_type)
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
