#include <stdio.h>
#include <stdlib.h>

#include "vars.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s inputfile\n", argv[0]);
        return(-1);
    }

    char error[2048];

    vars_t *anker = NULL;

    initVarAnker(&anker);

    return(start(argv[1], anker));
}
