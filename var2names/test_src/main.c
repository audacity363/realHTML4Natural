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

    add1DStringArray(nat_anker, NULL, "tmp1", 10, 10);
    add1DStringArray(nat_anker, NULL, "tmp2", 10, 10);
    add2DStringArray(nat_anker, NULL, "tmp3", 10, 10, 20);

    //add3DStringArray(nat_anker, NULL, "tmp4", 1, 2, 4, 30);
    //addString(nat_anker, NULL, "tmp5", L"Hello World", 11);
    

    if(startvar2name(nat_anker->next, "./test_src/TEST.NGL", true, stdout, error_buffer) != 0)
    {
        printAllVars(nat_anker);
        return(1);
    }

    printAllVars(nat_anker);
    freeVarAnker(nat_anker);
    return(0);
}
