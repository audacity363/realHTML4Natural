#include <stdio.h>
#include <stdlib.h>

#include "varhandle.h"

extern int parser_start(char *templatefile, struct variables *anker, char *deliverfile);

int main()
{
    parser_start("", NULL, "");
    return(0);
}
