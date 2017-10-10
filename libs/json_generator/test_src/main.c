#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vars.h"
#include "gen_json.h"

int main()
{
    vars_t *anker = NULL;

    initVarAnker(&anker);

    addGroup(anker, "grp1", 0, 0, 0);

    add2DIntegerArray(anker, "grp1", "field4", 5, 6);

    add1DIntegerArray(anker, "grp1", "field1", 5);
    add1DStringArray(anker, "grp1", "field2", 12, 5);
    add1DBooleanArray(anker, "grp1", "field3", 5);


    printAllVars(anker);

    //printEntry(anker->next->next_lvl->next, -1, -1, -1, stdout);
    
    addGroup(anker, "grp2", 0, 0, 0);
    addString(anker, "grp2", "field5", L"Hello", 12);
    addString(anker, "grp2", "field6", L"Hello", 12);

    startJSONgenerator(anker, "./test_src/test.out", stdout);

    freeVarAnker(anker);
    return(0);
}

