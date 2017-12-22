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

    matchPosition(NULL, NULL);
#if 0
    char error_buffer[2024];
    vars_t *hptr = NULL,
    *nat_anker = NULL;

    initVarAnker(&nat_anker);

    addString(nat_anker, NULL, "var1", L"val1", 4);
    addString(nat_anker, NULL, "var2", L"val2", 4);
    addString(nat_anker, NULL, "var3", L"val3", 4);
    addString(nat_anker, NULL, "var4", L"val4", 4);
    addString(nat_anker, NULL, "var5", L"val5", 4);
    addString(nat_anker, NULL, "var6", L"val6", 4);
    addString(nat_anker, NULL, "var7", L"val7", 4);
    addString(nat_anker, NULL, "var8", L"val8", 4);
    addString(nat_anker, NULL, "var9", L"val9", 4);
    addString(nat_anker, NULL, "var10", L"val10", 4);
    addString(nat_anker, NULL, "var11", L"val11", 4);
    addString(nat_anker, NULL, "var12", L"val12", 4);
    addString(nat_anker, NULL, "var13", L"val13", 4);
    addString(nat_anker, NULL, "var14", L"val14", 4);
    addString(nat_anker, NULL, "var15", L"val15", 4);
    addString(nat_anker, NULL, "var16", L"val16", 4);
    addString(nat_anker, NULL, "var17", L"val17", 4);
    addString(nat_anker, NULL, "var18", L"val18", 4);
    addString(nat_anker, NULL, "var19", L"val19", 4);
    addString(nat_anker, NULL, "var20", L"val20", 4);
    addString(nat_anker, NULL, "var21", L"val21", 4);
    addString(nat_anker, NULL, "var22", L"val22", 4);
    addString(nat_anker, NULL, "var23", L"val23", 4);
    addString(nat_anker, NULL, "var24", L"val24", 4);
    addString(nat_anker, NULL, "var25", L"val25", 4);
    addString(nat_anker, NULL, "var26", L"val26", 4);
    addString(nat_anker, NULL, "var27", L"val27", 4);
    addString(nat_anker, NULL, "var28", L"val28", 4);
    addString(nat_anker, NULL, "var29", L"val29", 4);
    addString(nat_anker, NULL, "var30", L"val30", 4);
    addString(nat_anker, NULL, "var31", L"val31", 4);
    addString(nat_anker, NULL, "var32", L"val32", 4);
    /*addString(nat_anker, NULL, "var33", L"val33", 4);
    addString(nat_anker, NULL, "var34", L"val34", 4);
    addString(nat_anker, NULL, "var35", L"val35", 4);*/

    if(startvar2name(nat_anker, "./test_src/TESTLDA1.NGL", false, stdout, "rolleninfo", error_buffer) != 0)
    {
        printf("Error buff: [%s]\n", error_buffer);
        printAllVars(nat_anker);
        return(1);
    }
  
   printAllVars(nat_anker);
   freeVarAnker(nat_anker);
   return(0);
#endif
}
