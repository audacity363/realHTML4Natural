#include <stdio.h>
#include <stdlib.h>

#include "../include/standard.h"
#include "parser.h"

int main()
{
    struct varnames *vars;
    
    vars = getVarNames("../../TESTLDA.NSL");
    printfork(vars);

}
