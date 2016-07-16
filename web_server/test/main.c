#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int dummyhandle(char *prog)
{
    return(0);
}

int main()
{
    start_web_server_from_nat(&dummyhandle);
}
