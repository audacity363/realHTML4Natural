#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "vars.h"
#include "parser.h"
#include "token_handling.h"
#include "macro.h"

vars_t *vars_anker;
macro_definition_t *macro_defs;

//Gibt den Typ des Blocker zurueck. Wenn nichts gefunden false 
//Variablenblock = VARIABLENBLOCK
//Commandblock = COMMANDBLOCK
int searchBlockBegin(wchar_t *str, wchar_t **pos)
{
    wchar_t *begin;

    if((begin = wcsstr(str, VARIABLEBEGIN_STR)) != NULL)
    {
        *pos = begin;
        return(VARIABLEBLOCK);
    }
    else if((begin = wcsstr(str, COMMANDBEGIN_STR)) != NULL)
    {
        *pos = begin;
        return(COMMANDBLOCK);
    }
    return(false);
}

int searchBlockEnd(wchar_t *str, wchar_t **pos, int type)
{
    wchar_t *end;

    if(type == VARIABLEBLOCK)
    {
        if((end = wcsstr(str, VARIABLEEND_STR)) != NULL)
        {
            *pos = end;
            return(true);
        }
        return(false);
    }
    else if(type == COMMANDBLOCK)
    {
        if((end = wcsstr(str, COMMANDEND_STR)) != NULL)
        {
            *pos = end;
            return(true);
        }
        return(false);
    }
    return(false);
}

int saveLine(wchar_t *line, status_t *stat)
{
    if(stat->sizeof_sav_buff == 0)
    {
        stat->save_buff = malloc(sizeof(wchar_t*));
        stat->save_buff[0] = malloc((wcslen(line)+1)*sizeof(wchar_t));
        wcscpy(stat->save_buff[0], line);
        stat->sizeof_sav_buff = 1;
    }
    else
    {
        stat->sizeof_sav_buff++;
        stat->save_buff = realloc(stat->save_buff, sizeof(wchar_t*)*stat->sizeof_sav_buff);
        stat->save_buff[stat->sizeof_sav_buff-1] = malloc((wcslen(line)+1)*sizeof(wchar_t)+1);
        wcscpy(stat->save_buff[stat->sizeof_sav_buff-1], line);
    }
    return(0);
}

void freeLineBuff(status_t *stat)
{
    int i;

    for(i=stat->sizeof_sav_buff-1; i > 0; i--)
    {
        free(stat->save_buff[i]);
    }
    free(stat->save_buff);
    stat->save_buff = NULL;
    stat->sizeof_sav_buff = 0;
}

int parseLine(wchar_t *line, status_t *status)
{
    wchar_t *inputstr = line,
         *begin,
         *end,
         *prev_end = NULL,
         *backup_line = line;
    size_t len = wcslen(line),
           restlength = len;
    int i,
        inblock = false,
        between_len = 0,
        ret = 0;

    printf("parse: [%S]\n", inputstr);
    while(restlength != 0) 
    { 
        if((inblock = searchBlockBegin(inputstr, &begin)) != false)
        {
            //Text zwischen den Bloecken ausgeben
            if(prev_end)
            {
                between_len = begin - prev_end;
                //printf("between_len: [%d]\n", between_len);
                printf("Kenn ich nicht [%.*S]\n", between_len, prev_end);
            }
            //Text vor dem ersten Block ausgeben
            if(!prev_end)
            {
                between_len = begin - inputstr;
                printf("Kenn ich nicht [%.*S]\n", between_len, inputstr);
            }
            if(searchBlockEnd(begin, &end, inblock) == false)
            {
                fprintf(stderr, "Syntax Error Missing Endbracked\n");
                break;
            }
            else if(inblock == VARIABLEBLOCK)
            {
                if(status->just_save)
                {
                    saveLine(line, status);
                    break;
                }
                parseVariable(begin, end);
            }
            else if(inblock == COMMANDBLOCK)
            {
                if((ret = parseCommand(begin, end, status)) == JUSTSAVE)
                {
                    saveLine(backup_line, status);
                    break;
                }
                else if(ret == EXIT)
                    return(EXIT);
            }
            restlength=wcslen(end+2);
            prev_end = inputstr = end+2;
        }
        //Text hinter dem letzen Block ausgeben
        else
        {
            printf("kenn ich nicht: [%S]\n", inputstr);
            restlength = restlength - wcslen(inputstr);
            if(status->just_save)
                saveLine(line, status);
        }
    }

}



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
    status_t status;  
    int i, ret;
    vars_t *anker = NULL;

    setlocale(LC_ALL, "");

    if((ret = initVarAnker(&anker)) != 0)
    {
        return(1);
    }

    if(initVars(anker) != 0)
        return(-1);

    vars_anker = anker;

    initMacroAnker(&macro_defs);

    printAllVars(vars_anker);
#define INPUTSTRS_LENGTH 5

    wchar_t *inputstrs[INPUTSTRS_LENGTH] = 
    {
        L"{% macro testmacro(hello=\"Test\", world, test) %}",
        L"<h1>Hello World</h1>",
        L"{% end-macro %}",
        L"<h1>Hello World</h1>",
        L"{% testmacro(grparray, \"bla 1\", 28.81) %}"
    };

    status.in_for = 0;
    status.in_if = 0;
    status.save_buff= NULL;
    status.just_save = 0;
    status.sizeof_sav_buff = 0;

    for(i=0; i < INPUTSTRS_LENGTH; i++)
    {
        if(parseLine(inputstrs[i], &status) == EXIT)
            break;
    }

    /*for(i=0; i < status.sizeof_sav_buff; i++)
    {
        printf("%s\n", status.save_buff[i]);
    }*/

#ifdef DEBUG
    listAllMacros(macro_defs);
#endif
    freeLineBuff(&status);
    freeMacros(macro_defs);
}
