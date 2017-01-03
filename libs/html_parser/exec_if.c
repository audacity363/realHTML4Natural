#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "exec_if.h"
#include "error_vars.h"


//String String Equals
int SSEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//String String Unequals
int SSUE(if_parms_t*, if_parms_t*, if_parms_t*);

//Float Integer Equals
int FIEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Unequals
int FIUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Less then
int FILT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Less then equals
int FILTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Greater then
int FIGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Integer Greater then equals
int FIGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Float Float Equals
int FFEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Unequals
int FFUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Less then
int FFLT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Less then equals
int FFLTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Greater then
int FFGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Float Float Greater then equals
int FFGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Integer Integer Equals
int IIEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Unequals
int IIUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Less then
int IILT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Less then equals
int IILTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Greater then
int IIGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Integer Greater then equals
int IIGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Integer Float Equals
int IFEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Unequals
int IFUE(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Less then
int IFLT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Less then equals
int IFLTEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Greater then
int IFGT(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer Float Greater then equals
int IFGTEQ(if_parms_t*, if_parms_t*, if_parms_t*);

//Bool Bool Equals
int BBEQ(if_parms_t*, if_parms_t*, if_parms_t*);
//Bool Bool Unequals
int BBUE(if_parms_t*, if_parms_t*, if_parms_t*);

//String OneDString in
int SODSIN(if_parms_t*, if_parms_t*, if_parms_t*);
//Integer OneDInteger in
int IODIIN(if_parms_t*, if_parms_t*, if_parms_t*);
//Float OneDFloat in
int FODFIN(if_parms_t*, if_parms_t*, if_parms_t*);


#define SIZE_OF_FUNCTION_TABLE 31
int if_function_table[SIZE_OF_FUNCTION_TABLE][3] = 
{
    {STRING, STRING, IFEQUALS},
    {STRING, STRING, IFUNEQUALS},

    {FLOAT, INTEGER, IFEQUALS},
    {FLOAT, INTEGER, IFUNEQUALS},
    {FLOAT, INTEGER, IFLESSTHEN},
    {FLOAT, INTEGER, IFLESSTHENEQUAL},
    {FLOAT, INTEGER, IFGREATERTHEN},
    {FLOAT, INTEGER, IFGREATERTHENEQUALS},

    {FLOAT, FLOAT, IFEQUALS},
    {FLOAT, FLOAT, IFUNEQUALS},
    {FLOAT, FLOAT, IFLESSTHEN},
    {FLOAT, FLOAT, IFLESSTHENEQUAL},
    {FLOAT, FLOAT, IFGREATERTHEN},
    {FLOAT, FLOAT, IFGREATERTHENEQUALS},

    {INTEGER, INTEGER, IFEQUALS},
    {INTEGER, INTEGER, IFUNEQUALS},
    {INTEGER, INTEGER, IFLESSTHEN},
    {INTEGER, INTEGER, IFLESSTHENEQUAL},
    {INTEGER, INTEGER, IFGREATERTHEN},
    {INTEGER, INTEGER, IFGREATERTHENEQUALS},

    {INTEGER, FLOAT, IFEQUALS},
    {INTEGER, FLOAT, IFUNEQUALS},
    {INTEGER, FLOAT, IFLESSTHEN},
    {INTEGER, FLOAT, IFLESSTHENEQUAL},
    {INTEGER, FLOAT, IFGREATERTHEN},
    {INTEGER, FLOAT, IFGREATERTHENEQUALS},

    {BOOL, BOOL, IFEQUALS},
    {BOOL, BOOL, IFUNEQUALS},

    {STRING, ONEDSTRING, IFIN},
    
    {INTEGER, ONEDINTEGER, IFIN},

    {FLOAT, ONEDINTEGER, IFIN}
};

int (*if_function_table_v[SIZE_OF_FUNCTION_TABLE])(if_parms_t*, if_parms_t*, if_parms_t*) =
{
    SSEQ,
    SSUE,
    FIEQ,
    FIUE,
    FILT,
    FILTEQ,
    FIGT,
    FIGTEQ,
    FFEQ,
    FFUE,
    FFLT,
    FFLTEQ,
    FFGT,
    FFGTEQ,
    IIEQ,
    IIUE,
    IILT,
    IILTEQ,
    IIGT,
    IIGTEQ,
    IFEQ,
    IFUE,
    IFLT,
    IFLTEQ,
    IFGT,
    IFGTEQ,
    BBEQ,
    BBUE,
    SODSIN,
    IODIIN,
    FODFIN
};

#define SIZE_OF_COMPARE_TABLE 9
int compare_table[SIZE_OF_COMPARE_TABLE][2] =
{
    {STRING, STRING},
    {FLOAT, INTEGER},
    {FLOAT, FLOAT},
    {INTEGER, INTEGER},
    {INTEGER, FLOAT},
    {BOOL, BOOL},
    {STRING, ONEDSTRING},
    {FLOAT, ONEDFLOAT},
    {INTEGER, ONEDINTEGER}
    
};

#define OPERATOR_COUNT 7
int operator_table[SIZE_OF_COMPARE_TABLE][OPERATOR_COUNT] = 
{
    {IFEQUALS, IFUNEQUALS, -1, -1, -1, -1, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN,
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN,
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN, 
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, IFLESSTHEN, 
     IFLESSTHENEQUAL, IFGREATERTHEN, 
     IFGREATERTHENEQUALS, -1},

    {IFEQUALS, IFUNEQUALS, -1, -1, -1, -1, -1},

    {IFIN, -1, -1, -1, -1, -1, -1}
};

/*
 * return code: > 0 Var type
 *               -1 Unsupported
 */
int if_getVarType(if_parms_t *parm)
{
    int ret = 0;
    char *c_val = NULL;

    if(parm->type == IFSTR)
        return(STRING);

    if(!(c_val = malloc(wcslen(parm->val)+1)))
        return(-2);
    
    wcstombs(c_val, parm->val, wcslen(parm->val)+1);

    if((ret = getVarType(vars_anker, NULL, c_val)) < 0)
    {
        free(c_val);
        return(-1);
    }
    free(c_val);

    if(parm->hasindex == 0 && ret == STRING)
        return(STRING);
    else if(parm->hasindex == 0 && ret == ONEDSTRING)
        return(ONEDSTRING);
    else if(parm->hasindex == 0 && ret == TWODSTRING)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODSTRING)
        return(ONEDSTRING);
    else if(parm->hasindex == 1 && ret == ONEDSTRING)
        return(STRING);
    else if(parm->hasindex == 1 && ret == STRING)
        return(STRING);
    else if(parm->hasindex == 2 && ret == TWODSTRING)
        return(STRING);
    else if(parm->hasindex == 2 && ret == ONEDSTRING)
        return(STRING);
    else if(parm->hasindex == 2 && ret == STRING)
        return(-1);

    if(parm->hasindex == 0 && ret == INTEGER)
        return(INTEGER);
    else if(parm->hasindex == 0 && ret == ONEDINTEGER)
        return(ONEDINTEGER);
    else if(parm->hasindex == 0 && ret == TWODINTEGER)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODINTEGER)
        return(ONEDINTEGER);
    else if(parm->hasindex == 1 && ret == ONEDINTEGER)
        return(INTEGER);
    else if(parm->hasindex == 1 && ret == INTEGER)
        return(-1);
    else if(parm->hasindex == 2 && ret == TWODINTEGER)
        return(INTEGER);
    else if(parm->hasindex == 2 && ret == ONEDINTEGER)
        return(-1);
    else if(parm->hasindex == 2 && ret == INTEGER)
        return(-1);

    if(parm->hasindex == 0 && ret == FLOAT)
        return(FLOAT);
    else if(parm->hasindex == 0 && ret == ONEDFLOAT)
        return(ONEDFLOAT);
    else if(parm->hasindex == 0 && ret == TWODFLOAT)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODFLOAT)
        return(ONEDFLOAT);
    else if(parm->hasindex == 1 && ret == ONEDFLOAT)
        return(FLOAT);
    else if(parm->hasindex == 1 && ret == FLOAT)
        return(-1);
    else if(parm->hasindex == 2 && ret == TWODFLOAT)
        return(FLOAT);
    else if(parm->hasindex == 2 && ret == ONEDFLOAT)
        return(-1);
    else if(parm->hasindex == 2 && ret == FLOAT)
        return(-1);

    if(parm->hasindex == 0 && ret == BOOL)
        return(BOOL);
    else if(parm->hasindex == 0 && ret == ONEDBOOL)
        return(ONEDBOOL);
    else if(parm->hasindex == 0 && ret == TWODBOOL)
        return(-1);
    else if(parm->hasindex == 1 && ret == TWODBOOL)
        return(ONEDBOOL);
    else if(parm->hasindex == 1 && ret == ONEDBOOL)
        return(BOOL);
    else if(parm->hasindex == 1 && ret == BOOL)
        return(-1);
    else if(parm->hasindex == 2 && ret == TWODBOOL)
        return(BOOL);
    else if(parm->hasindex == 2 && ret == ONEDBOOL)
        return(-1);
    else if(parm->hasindex == 2 && ret == BOOL)
        return(-1);

    return(-2);
}

int checkIfcompatible(int types[3])
{
    int i = 0,
        x = 0;

    printf("type1[%d]\ntype2[%d]\ntype3[%d]\n", types[0], types[1], types[2]);

    for(; i < SIZE_OF_COMPARE_TABLE; i++)
    {
        if(compare_table[i][0] == types[0] &&
           compare_table[i][1] == types[2])
        {
            for(; x < OPERATOR_COUNT; x++)
            {
                if(types[1] == operator_table[i][x])
                    return(true);
            }
            return(false);
        }
    }
    return(false);
}

int (*getIfFunction(int types[3]))(if_parms_t*, if_parms_t*, if_parms_t*)
{
    int i = 0;

    for(; i < SIZE_OF_FUNCTION_TABLE; i++)
    {
        if(if_function_table[i][0] == types[0] &&
           if_function_table[i][1] == types[2] &&
           if_function_table[i][2] == types[1])
            return(if_function_table_v[i]);
    }
    return(NULL);
}

/*
 * return code: > 0 Compare type
 *               -1 Unsupported
 */
int getCompareType(wchar_t *val)
{
    if(wcscmp(val, L"==") == 0)
        return(IFEQUALS);
    if(wcscmp(val, L"<=") == 0)
        return(IFLESSTHENEQUAL);
    if(wcscmp(val, L">=") == 0)
        return(IFGREATERTHENEQUALS);
    if(wcscmp(val, L"<") == 0)
        return(IFLESSTHEN);
    if(wcscmp(val, L">") == 0)
        return(IFGREATERTHEN);
    if(wcscmp(val, L"!=") == 0)
        return(IFUNEQUALS);
    if(wcscmp(val, L"in") == 0)
        return(IFIN);

    return(-1);
}

/*
 * Currently the if just works with one query. The rest of the
 * head is ignored
 */
int exec_if(if_parms_t *parms, wchar_t **body, int body_length)
{
    int i = 0,
        types[3] = {-1, -1, -1},
        (*if_function)(if_parms_t*, if_parms_t*, if_parms_t*) = NULL;

    if_parms_t *leftval = NULL,
               *compare = NULL,
               *rightval = NULL;

    status_t status;  

    status.in_for = 0;
    status.in_if = 0;
    status.save_buff= NULL;
    status.sizeof_sav_buff = 0;

    //Check if the first three parms are set
    if(!parms->next)
       return(-1);
    if(!parms->next->next)
       return(-2);
    if(!parms->next->next->next)
       return(-3);

    leftval = parms->next; compare = parms->next->next; rightval = parms->next->next->next;

    if((types[0] = if_getVarType(leftval)) < 0)
    {
        //Unsuported Type in first parm
        printf("Unsupported Type in first parm\n");
        return(-4);
    }
    leftval->varlib_type = types[0];

    if((types[1] = getCompareType(compare->val)) < 0)
    {
        //Unsuported Type in compare parm
        printf("Unsupported Type in compare parm\n");
        return(-5);
    }
    compare->varlib_type = types[1];

    if((types[2] = if_getVarType(rightval)) < 0)
    {
        //Unsuported Type in second parm
        printf("Unsupported Type in second parm\n");
        return(-6);
    }
    rightval->varlib_type = types[2];

    if(checkIfcompatible(types) != true)
    {
        printf("Type are not compatible\n");
        exit(-1);
    }

    if((if_function = getIfFunction(types)) == NULL)
    {
        printf("Can not get If Function\n");
        return(-8);
    }

    if(if_function(leftval, compare, rightval) != 0)
    {
        freeLineBuff(&status);
        return(0);
    }

    printf("---------If body-------\n");
    for(i=1; i < body_length; i++)
    {
        parseLine(body[i], &status);
    }
    printf("---------end-if--------\n");
    freeLineBuff(&status);
    return(0);
}

int convertNames(if_parms_t *left, if_parms_t *right, char **leftname, char **rightname)
{   
    if(!(*leftname = malloc(wcslen(left->val)+1)))
    {
        printf("Malloc error\n");
        return(-1);
    }
    wcstombs(*leftname, left->val, wcslen(left->val)+1);

    if(!(*rightname = malloc(wcslen(right->val)+1)))
    {
        free(leftname);
        printf("Malloc error\n");
        return(-1);
    }
    wcstombs(*rightname, right->val, wcslen(right->val)+1);
    return(0);
}

void freeNames(char **leftname, char **rightname)
{
    free(*leftname);
    free(*rightname);
}

//String String Equals
int SSEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int left_length = 0, right_length = 0,
        cmp_ret = 0;
    char *left_name = NULL, *right_name = NULL;
    wchar_t *left_val = NULL, *right_val = NULL;

    if(leftval->type != IFSTR)
    {

        if(!(left_name = malloc(wcslen(leftval->val)+1)))
        {
            printf("Malloc error\n");
            return(-1);
        }
        
        wcstombs(left_name, leftval->val, wcslen(leftval->val)+1);


        if(getStringLength(vars_anker, NULL, left_name, &left_length) < 0)
        {
            free(left_name);
            return(-2);
        }

        if(!(left_val = malloc(left_length*sizeof(wchar_t))))
        {
            free(left_name);
            return(-3);
        }

        if(getString(vars_anker, NULL, left_name, left_val, left_length))
        {
            free(left_name);
            free(left_val);
            return(-4);
        }
        free(left_name);
    }
    else
    {
        left_val = leftval->val;
    }

    if(rightval->type != IFSTR)
    {

        if(!(right_name = malloc(wcslen(rightval->val)+1)))
        {
            printf("Malloc error\n");
            return(-1);
        }
        
        wcstombs(right_name, rightval->val, wcslen(rightval->val)+1);


        if(getStringLength(vars_anker, NULL, right_name, &right_length) < 0)
        {
            free(right_name);
            free(left_val);
            return(-2);
        }

        if(!(right_val = malloc(right_length*sizeof(wchar_t))))
        {
            free(left_name);
            free(left_val);
            return(-3);
        }

        if(getString(vars_anker, NULL, left_name, left_val, left_length))
        {
            free(right_name);
            free(left_val);
            return(-4);
        }
        free(right_name);
    }
    else
    {
        right_val = rightval->val;
    }

    if((cmp_ret = wcscmp(left_val, right_val)) <  0)
        cmp_ret *= -1;

    free(left_val);
    free(right_val);
    return(cmp_ret);
}

//String String Unequals
int SSUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int ret = 0;

    if((ret = SSEQ(leftval, compare, rightval)) < 0)
        return(ret);
    else if(ret != 0)
        return(0);
    else
        return(1);
}

//Float Integer Equals
int FIEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0;   
    int right_val = 0,
        ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val == right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Float Integer Unequals
int FIUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int ret = 0;

    if((ret = FIEQ(leftval, compare, rightval)) < 0)
        return(ret);
    else if(ret != 0)
        return(0);
    else
        return(1);

}
//Float Integer Less then
int FILT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0;   
    int right_val = 0,
        ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val < right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Float Integer Less then equals
int FILTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0;   
    int right_val = 0,
        ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val <= right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Float Integer Greater then
int FIGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0;   
    int right_val = 0,
        ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val > right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Float Integer Greater then equals
int FIGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0;   
    int right_val = 0,
        ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val >= right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}

//Float Float Equals
int FFEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getFloat(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val == right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);

}
//Float Float Unequals
int FFUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int ret = 0;

    if((ret = FFEQ(leftval, compare, rightval)) < 0)
        return(ret);
    else if(ret != 0)
        return(0);
    else
        return(1);

    return(1);
}
//Float Float Less then
int FFLT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getFloat(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val < right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Float Float Less then equals
int FFLTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getFloat(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val <= right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Float Float Greater then
int FFGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getFloat(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val > right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Float Float Greater then equals
int FFGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    double left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getFloat(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getFloat(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val >= right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}

//Integer Integer Equals
int IIEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getInteger(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val == right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Integer Integer Unequals
int IIUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Integer Integer Less then
int IILT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getInteger(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val < right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Integer Integer Less then equals
int IILTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getInteger(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val <= right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Integer Integer Greater then
int IIGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getInteger(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val > right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}
//Integer Integer Greater then equals
int IIGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    int left_val = 0,
        right_val = 0;
    int ret = 0;
    char *left_name = NULL,
         *right_name = NULL;
    
    if(convertNames(leftval, rightval, &left_name, &right_name) < 0)
    {
        return(-1);
    }
    
    if((ret = getInteger(vars_anker, NULL, left_name, &left_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }
    if((ret = getInteger(vars_anker, NULL, right_name, &right_val)) == GRP_NOT_DEFINED ||
        ret == VAR_NOT_DEFINED)
    {
        printf("error: [%s]\n", var_errorstrs[ret]);
        return(-2);
    }

    if(left_val >= right_val)
    {
        freeNames(&left_name, &right_name);
        return(0);
    }
    freeNames(&left_name, &right_name);
    return(1);
}

//Integer Float Equals
int IFEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Integer Float Unequals
int IFUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Integer Float Less then
int IFLT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Integer Float Less then equals
int IFLTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Integer Float Greater then
int IFGT(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Integer Float Greater then equals
int IFGTEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}

//Bool Bool Equals
int BBEQ(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Bool Bool Unequals
int BBUE(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}

//String OneDString in
int SODSIN(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Integer OneDInteger in
int IODIIN(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}
//Float OneDFloat in
int FODFIN(if_parms_t *leftval, if_parms_t *compare, if_parms_t *rightval)
{
    return(1);
}

