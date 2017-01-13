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

    if((ret = startLDAParser(argv[1], anker, true, stdout)) != LDA_OK)
        return(ret);

    printfork(anker, stdout);
    return(0);
}

