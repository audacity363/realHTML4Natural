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
    vars_t *lda_anker = NULL, *hptr = NULL,
           *nat_anker = NULL;

    initVarAnker(&lda_anker);
    initVarAnker(&nat_anker);

    lda_anker = hptr->next_lvl;
    printfork(lda_anker);

    addString(nat_anker, NULL, "tmp4", L"Hello World", 11);
    addInteger(nat_anker, NULL, "tmp5", 7);
    addString(nat_anker, NULL, "tmp6", L"Hello World1", 12);
    addString(nat_anker, NULL, "tmp7", L"Hello World2", 12);
    addString(nat_anker, NULL, "tmp8", L"Hello World3", 12);
    addInteger(nat_anker, NULL, "tmp9", 10);
    addInteger(nat_anker, NULL, "tmp10", 8);
    addInteger(nat_anker, NULL, "tmp11", 9);

    if(startvar2name(nat_anker->next, "./test_src/TESTLDA.NSL", true, stdout) != 0)
    {
        printAllVars(nat_anker);
        return(1);
    }

    printAllVars(nat_anker);
    return(0);
}
