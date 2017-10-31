#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "lda.h"
#include "var2name.h"

vars_t *handleGroup(vars_t *cur_var, vars_t *group);

int main()
{
    char error_buffer[2024];
    vars_t *hptr = NULL,
           *nat_anker = NULL;

    initVarAnker(&nat_anker);

    //addString(nat_anker, NULL, "var1", L"val1", 5);
    addString(nat_anker, NULL, "var2", L"val2", 5);
    addInteger(nat_anker, NULL, "var3", 3);
    addBoolean(nat_anker, NULL, "var4", true);


    if(startvar2name(nat_anker, "./test_src/TESTLDA.NGL", true, stdout, "platzbelegungen", error_buffer) != 0)
    {
        printf("Error: [%s]\n", error_buffer);
        printAllVars(nat_anker);
        return(1);
    }

    printAllVars(nat_anker);
    freeVarAnker(nat_anker);
    return(0);
}
