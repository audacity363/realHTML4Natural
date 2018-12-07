#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh4n.h"


int main() {
    RH4nVarList varlist;
    RH4nProperties props;

    props.logging = rh4nLoggingCreateStreamingRule("VARLIB", "TEST", RH4N_DEVELOP, "");

    rh4nvarInitList(&varlist);
    printf("ret: [%s]\n", rh4nvarLoadFromFile("./vardump.rh4n", &varlist));

    rh4nvarPrintList(&varlist, &props);
    rh4nvarFreeList(&varlist);
    rh4n_del_log_rule(props.logging);

}
