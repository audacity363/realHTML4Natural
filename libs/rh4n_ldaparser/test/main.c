#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "standard.h"
#include "rh4n_ldaparser.h"

int main(int argc, char *argv[]) {
    RH4nProperties props;
    char errorstr[2048];
    int parserret = 0;
    RH4NLDAEntry_t *entries = NULL;

    props.logging = rh4nLoggingCreateRule("LDALIB", "LDAPARSE", RH4N_DEVELOP, "./logs/");
    parserret = rh4nldaStartParser("./testfiles/TEST.NGL", &entries, &props, errorstr);
    rh4n_log_debug(props.logging, "Parser ret: %d", parserret);
    rh4n_log_debug(props.logging, "Error: [%s]", errorstr);
    rh4n_log_debug(props.logging, "---------------------------------------------------------------");

    rh4nldaPrintList(entries, &props);
    rh4nldaFreeList(entries);
}
