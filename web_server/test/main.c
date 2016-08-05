#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "server.h"

int dummyhandle(char *prog, char *ip, httpheader test)
{
    return(0);
}

int main()
{
    start_web_server_from_nat(&dummyhandle);
}
