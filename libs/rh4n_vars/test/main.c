#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"

int main(int argc, char *argv[]) {
    RH4nVarList varlist;
    RH4nVarRef ref;
    RH4nProperties props;
    int arraylength[3] = { 3, 2, 4}, index[3];
    char testbuff[100];

    props.logging = rh4nLoggingCreateStreamingRule("VARLIB", "TEST", RH4N_DEVELOP, "./logs/");

    rh4nvarInitList(&varlist);

    //rh4nvarCreateNewString(&varlist, NULL, "str1", "Hello World");
    //rh4nvarGetString(&varlist, NULL, "str1", sizeof(testbuff), testbuff);
    //printf("Testbuff: [%s]\n", testbuff);
    //memset(testbuff, NULL, sizeof(testbuff));

    rh4nvarCreateNewStringArray(&varlist, NULL, "strarray1", 3, arraylength);
    index[0] = index[1] = 1;
    index[2] = 2;
    printf("setArray: %d\n", rh4nvarSetStringArrayEntry(&varlist, NULL, "strarray1", index, "Hello World Array"));
    rh4nvarGetStringArrayEntry(&varlist, NULL, "strarray1", index, sizeof(testbuff), testbuff);
    printf("Testbuff Array: [%s]\n", testbuff);

    //arraylength[0] = 5;
    //arraylength[1] = 3;
    memset(arraylength, 0x00, sizeof(arraylength));
    rh4nvarCreateNewStringArray(&varlist, NULL, "strarray2", 1, arraylength);

    //rh4nvarPrintList(&varlist);
    //printf("\n");

    rh4nvarCreateNewGroup(&varlist, "grp1");
    rh4nvarCreateNewBool(&varlist, NULL, "bool1", true);
    rh4nvarCreateNewBool(&varlist, NULL, "bool2", true);
    //rh4nvarPrintList(&varlist);
    //printf("\n");

    rh4nvarMoveVarToGroup(&varlist, "bool1", "grp1");
    rh4nvarMoveVarToGroup(&varlist, "strarray1", "grp1");
    rh4nvarPrintList(&varlist, &props);
    printf("\n");
    rh4nvarFreeList(&varlist);
    rh4n_del_log_rule(props.logging);
    return(0);
}
