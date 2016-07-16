#include <stdio.h>

#include "hexdump.h"

int main()
{
    char teststr[] = "Hello World";

    logHexDump(teststr, sizeof(teststr), stdout);
}
