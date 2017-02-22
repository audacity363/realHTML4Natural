#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "vars.h"

int initVars(vars_t *anker)
{
    int ret, i, x, y, z;
    float t = 82.62;
    wchar_t strbuff[100];

    if((ret = addGroup(anker, "hallo", 0,0,0)) != 0)
    {
        return(1);
    }

    addGroup(anker, "grparray", 10,0,0);
    addGroup(anker, "grptest", 0,0,0);
    add1DIntegerArray(anker, "grparray", "array", 10);
    for(i=0; i < 10; i++)
    {

        edit1DIntegerArray(anker, "grparray", "array", i, i);
    }

    addInteger(anker, NULL, "test", 677);
    addInteger(anker, NULL, "test1", 67);
    addInteger(anker, "hallo", "test2", 32);

    add1DIntegerArray(anker, "hallo", "array", 10);
    for(i=0; i < 10; i++)
    {

        edit1DIntegerArray(anker, "hallo", "array", i, i);
    }

    add2DIntegerArray(anker, "grparray", "2darray", 5, 7);

    z = 0;
    for(i=0; i < 5; i++)
    {
        for(x=0; x < 7; x++)
        {
            edit2DIntegerArray(anker, "grparray", "2darray", z, i, x);
            z++;
        }
    }

    i = 0;
    add3DIntegerArray(anker, "grparray", "3darray", 5, 7, 9);
    add3DIntegerArray(anker, "grptest", "3darray", 5, 7, 9);

    for(x=0; x < 5; x++)
        for(y=0; y < 7; y++)
            for(z=0; z < 9; z++)
            {
                edit3DIntegerArray(anker, "grparray", "3darray", i, x, y, z);
                edit3DIntegerArray(anker, "grptest", "3darray", i, x, y, z);
                i++;
            }
    if((ret = addString(anker, NULL, "teststr", L"höhöhöhöh", 100)) != 0)
    {
        return(1);
    }

    if((ret = addString(anker, NULL, "teststr1", L"höhö", 100)) != 0)
    {
        return(1);
    }

    add1DStringArray(anker, "grparray", "string1d", 100, 10);

    for(x=0; x < 10; x++)
    {
        swprintf(strbuff, 100, L"hühühü %d", x);
        edit1DStringArray(anker, "grparray", "string1d", strbuff, x);
    }

    add2DStringArray(anker, NULL, "string2d", 100, 5, 7);

    i = 0;
    for(x=0; x < 5; x++)
        for(y=0; y < 7; y++)
        {
            swprintf(strbuff, 100, L"hähähä %d", i);
            if((ret = edit2DStringArray(anker, NULL, "string2d", strbuff, x, y)) != 0)
            {
                return(1);
            }
            i++;
        }

    i=0;
    add3DStringArray(anker, NULL, "string3d",100, 5, 6, 7);
    for(x=0; x < 5; x++)
        for(y=0; y < 6; y++)
            for(z=0; z < 7; z++)
            {
                swprintf(strbuff, 100, L"hä %d", i);
                if((ret = edit3DStringArray(anker, NULL, "string3d", strbuff, x, y, z)) != 0)
                {
                    return(1);
                }
                i++;
            }



    if((ret = addBoolean(anker, NULL, "bool", 1)) != 0)
    {
        return(1);
    }

    if((ret = add1DBooleanArray(anker, "grparray", "bool1d", 10)) != 0)
    {
        return(1);
    }

    for(x=0; x < 10; x++)
    {
        if(x % 2)
            edit1DBooleanArray(anker, "grparray", "bool1d", 0, x);
        else
            edit1DBooleanArray(anker, "grparray", "bool1d", 1, x);
    }


    if((ret = add2DBooleanArray(anker, NULL, "bool2d", 5, 6)) != 0)
    {
        return(1);
    }
    add2DBooleanArray(anker, "grparray", "bool2d", 5, 6);
    for(x=0; x < 10; x++)
        for(y=0; y < 6; y++)
        {
            if(x % 2)
            {
                edit1DBooleanArray(anker, "grparray", "bool2d", 0, x, y);
                edit1DBooleanArray(anker, NULL, "bool2d", 0, x, y);
            }
            else
            {
                edit1DBooleanArray(anker, "grparray", "bool2d", 1, x, y);
                edit1DBooleanArray(anker, NULL, "bool2d", 1, x, y);
            }
        }

    addFloat(anker, NULL, "float", 677.01);
    add1DFloatArray(anker, "grparray", "float1d", 5);
    for(x=0; x < 5; x++)
    {
        edit1DFloatArray(anker, "grparray", "float1d", 72.9276, x);
    }

    return(0);
}

int main()
{
    vars_t *anker = NULL, *hptr = NULL;
    int ret = 0;

    setlocale(LC_ALL, "");

    if((ret = initVarAnker(&anker)) != 0)
    {
        return(1);
    }

    if(initVars(anker) != 0)
        return(-1);

    printAllVars(anker);

    if((hptr = isDefinedGrp(anker, "grptest", "3darray")) == NULL)
    {
        fprintf(stderr, "Var not defined\n");
        goto end;
    }


end:
    freeVarAnker(anker);
}
