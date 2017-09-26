#include <stdio.h>
#include <stdlib.h>

#include "vars.h"
#include "lda.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s inputfile\n", argv[0]);
        return(-1);
    }

    char error[2048];
    int ret = 0;

    vars_t *anker = NULL;

    initVarAnker(&anker);

    if(startLDAParser(argv[1], anker, stdout, error) < 0)
        return(-1);
    printfork(anker, stdout);
    return(0);
}

