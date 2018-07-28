#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh4n.h"

int main(int argc, char *argv[]) {
    RH4nVarList varlist;
    RH4nVarRef ref;
    RH4nProperties props;
    int arraylength[3] = { 3, 2, 4}, index[3];
    char testbuff[100];

    props.logging = rh4nLoggingCreateStreamingRule("VARLIB", "TEST", RH4N_DEVELOP, "./logs/");

    rh4nvarInitList(&varlist);

    rh4nvarCreateNewString(&varlist, NULL, "newstr", "value");

    memset(arraylength, 0x00, sizeof(arraylength));
    arraylength[0] = 2;
    rh4nvarCreateNewStringArray(&varlist, NULL, "strarray", 1, arraylength);
    rh4nvarPrintJSONFork(varlist.anker, 0, 1, &props, stdout);
    printf("\n");

    arraylength[0] = 4;
    printf("Expand: [%d]\n", rh4nvarExpandArray(&varlist, NULL, "strarray", 1, arraylength));

    rh4nvarPrintList(&varlist, &props);
    rh4nvarPrintJSONFork(varlist.anker, 0, 1, &props, stdout);

    rh4nvarFreeList(&varlist);
    rh4n_del_log_rule(props.logging);
    return(0);
}
