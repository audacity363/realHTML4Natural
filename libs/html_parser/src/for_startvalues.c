#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "parser.h"
#include "for.h"
#include "for_startvalues.h"

/*
 * Lookup for the function wich creates the new variable and sets it start value
 * First Index is the variablen Type, the second the index type. Index type 3
 * is not allowed in a for loop because then the new var wouldnt be an array
 */
#define SIZEOF_FOR_LOOKUP 24
int start_value_lookup[SIZEOF_FOR_LOOKUP][2] = {
    {ONEDINTEGER, 0}, // results in normal integer
    {TWODINTEGER, 0}, // results in a one d integer array
    {THREEDINTEGER, 0}, // results in a two d inetger
    {TWODINTEGER, 1}, // results in a normal integer
    {THREEDINTEGER, 1}, // results in a two d integer array
    {THREEDINTEGER, 2}, // results in a one d integer array

    {ONEDFLOAT, 0}, // results in normal float
    {TWODFLOAT, 0}, // results in a one d float array
    {THREEDFLOAT, 0}, // results in a two d float
    {TWODFLOAT, 1}, // results in a one d float array
    {THREEDFLOAT, 1}, // results in a two d float array
    {THREEDFLOAT, 2}, // results in a one d float array

    {ONEDBOOL, 0}, // results in normal bool
    {TWODBOOL, 0}, // results in a one d bool array
    {THREEDBOOL, 0}, // results in a two d bool array
    {TWODBOOL, 1}, // results in a one d bool array
    {THREEDBOOL, 1}, // results in a two d bool array
    {THREEDBOOL, 2}, // results in a one d bool array


    {ONEDSTRING, 0}, // results in normal string
    {TWODSTRING, 0}, // results in a one d string array
    {THREEDSTRING, 0}, // results in a two d string array
    {TWODSTRING, 1}, // results in a one d string array
    {THREEDSTRING, 1}, // results in a two d string array
    {THREEDSTRING, 2}, // results in a one d string array
};

int (*start_value_lookup_func[SIZEOF_FOR_LOOKUP])(for_status, vars_t*, int) = {
    startval_1di0index,
    startval_2di0index,
    startval_3di0index,
    startval_2di1index,
    startval_3di1index,
    startval_3di2index,
    startval_1df0index,
    startval_2df0index,
    startval_3df0index,
    startval_2df1index,
    startval_3df1index,
    startval_3df2index,
    startval_1db0index,
    startval_2db0index,
    startval_3db0index,
    startval_2db1index,
    startval_3db1index,
    startval_3db2index,
    startval_1ds0index,
    startval_2ds0index,
    startval_3ds0index,
    startval_2ds1index,
    startval_3ds1index,
    startval_3ds2index
};

//Question: If multiple variablen were given but only one temp var: Just create
//          a new group with the temp name and grp entry with the original
//          names? (Probably a feature for a future version)
int setStartValue(char *tmp_var, for_status stat, vars_t *anker)
{
    int (*start_func)(for_status, vars_t*, int) = NULL, i, x;

    if(isDefinedGrpBool(anker, "loop", "i") == true)
    {
        editInteger(anker, "loop", "i", stat.index);
    }
    else
    {
        addGroup(anker, "loop", 0, 0, 0);

        addInteger(anker, "loop", "i", stat.index);
    }

    if(stat.for_type == RANGE)
    {
        if(isDefinedGrpBool(anker, NULL, tmp_var) == false)
        {
            if(stat.range.start != -1)
            {
                addInteger(anker, NULL, tmp_var, stat.range.start);
            }
            else
            {
                addInteger(anker, NULL, tmp_var, 0);
            }
        }
        else
        {
            if(stat.range.start != -1)
            {
                editInteger(anker, NULL, tmp_var, stat.range.start+stat.index);
            }
            else
            {
                editInteger(anker, NULL, tmp_var, stat.index);
            }
        }
        return(0);
    }

    //Create the temp var/vars 
    for(x=0; x < stat.var_count; x++)
    {
        for(i=0; i < SIZEOF_FOR_LOOKUP; i++)
        {
            //First Index of lookup table: Vartype
            //Second Index: index_type
            if(start_value_lookup[i][0] == stat.array.type[x] 
                && start_value_lookup[i][1] == stat.array.index_type[x])
            {
                start_func = start_value_lookup_func[i];
                break;
            }
        }
        if(start_func == NULL)
        {
            fprintf(stderr, "Combination out of variable type and index type are not supported\n");
            return(-2);
        }
        start_func(stat, anker, x);
        start_func = NULL;
    }

}

// Integer handles
int startval_1di0index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom1DIntegerArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2di0index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom2DIntegerArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_3di0index(for_status stat, vars_t *anker, int index)
{
    createNew2DArrayFrom3DIntegerArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2di1index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom2DIntegerArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3di1index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom3DIntegerArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3di2index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom3DIntegerArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.array.y_index[index], stat.index);
}

// Float handles
int startval_1df0index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom1DFloatArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2df0index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom2DFloatArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_3df0index(for_status stat, vars_t *anker, int index)
{
    createNew2DArrayFrom3DFloatArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2df1index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom2DFloatArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3df1index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom3DFloatArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3df2index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom3DFloatArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.array.y_index[index], stat.index);
}

// Boolean heandles
int startval_1db0index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom1DBooleanArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2db0index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom2DBooleanArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_3db0index(for_status stat, vars_t *anker, int index)
{
    createNew2DArrayFrom3DBooleanArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2db1index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom2DBooleanArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3db1index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom3DBooleanArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3db2index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom3DBooleanArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.array.y_index[index], stat.index);
}

// String handles
int startval_1ds0index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom1DStringArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2ds0index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom2DStringArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_3ds0index(for_status stat, vars_t *anker, int index)
{
    createNew2DArrayFrom3DStringArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], stat.index);
}

int startval_2ds1index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom2DStringArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3ds1index(for_status stat, vars_t *anker, int index)
{
    createNew1DArrayFrom3DStringArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.index);
}

int startval_3ds2index(for_status stat, vars_t *anker, int index)
{
    createNewVarFrom3DStringArray(anker, anker, stat.array.grp[index],
        stat.array.name[index], NULL, stat.varnames[index], 
        stat.array.x_index[index], stat.array.y_index[index], stat.index);
}
