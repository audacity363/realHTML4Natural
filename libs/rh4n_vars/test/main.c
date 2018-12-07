#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh4n.h"
#include "rh4n_ldaparser.h"
#include "rh4n_v2n.h"

int main(int argc, char *argv[]) {
    RH4nVarList varlist;
    RH4nProperties props;
    RH4nLDAEntry_t *entries = NULL;
    char errorstr[2048];
    int parserret = 0;

    props.logging = rh4nLoggingCreateStreamingRule("VARLIB", "TEST", RH4N_DEVELOP, "");

    rh4nvarInitList(&varlist);

    rh4nvarCreateNewString(&varlist, NULL, "str1", "val1");
    rh4nvarCreateNewString(&varlist, NULL, "str2", "val2");
    rh4nvarCreateNewString(&varlist, NULL, "str3", "val3");
    rh4nvarCreateNewString(&varlist, NULL, "str4", "val3");

    rh4nvarPrintList(&varlist, &props);

#if 0
    parserret = rh4nldaStartParser("./libs/rh4n_var2name/testfiles/TESTLDA.NGL", &entries, &props, errorstr);
    rh4nv2nStart(&varlist, entries, "GRP1", &props, errorstr);

    rh4nvarPrintList(&varlist, &props);
    rh4nvarPrintJSONFork(varlist.anker, 0, 1, &props, stdout);
#endif

    rh4nvarFreeList(&varlist);
    rh4n_del_log_rule(props.logging);
    return(0);
}
