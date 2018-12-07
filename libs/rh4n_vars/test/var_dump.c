#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh4n.h"

int main() {
    RH4nVarList varlist;
    RH4nProperties props;
    char errorstr[2048];
    int parserret = 0;
    int arrlength[3] = {2, 0, 0};

    props.logging = rh4nLoggingCreateStreamingRule("VARLIB", "TEST", RH4N_DEVELOP, "");

    rh4nvarInitList(&varlist);

    rh4nvarCreateNewString(&varlist, NULL, "str1", "");

    char *parents[] = { "grp1", NULL, NULL };
    rh4nvarCreateNewGroup(&varlist, "grp1");

    rh4nvarCreateNewString(&varlist, "grp1", "str1", "");
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);

    rh4nvarCreateNewGroup_m(&varlist, parents, "grp2");

    parents[1] = "grp2";
    rh4nvarCreateNewStringArray(&varlist, NULL, "str3", 1, arrlength);
    rh4nvarMoveVarToGroup_m(&varlist, "str3", parents);
    rh4nvarCreateNewString(&varlist, NULL, "str4", "");
    rh4nvarMoveVarToGroup_m(&varlist, "str4", parents);

    rh4nvarCreateNewStringArray(&varlist, "grp1", "str5", 1, arrlength);

    rh4nvarPrintList(&varlist, &props);

    printf("dump: [%d]\n", rh4nvarDumpToFile(&varlist, "./vardump.rh4n"));

    rh4nvarFreeList(&varlist);
    rh4n_del_log_rule(props.logging);
}
