#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "parser.h"
#include "varhandle.h"

int main()
{
    int i;
    char buff[20];

    struct variables *anker = malloc(sizeof(struct variables));
    strcpy(anker->name, "anker");
    anker->next = NULL;

    newStringArray(anker, "testarray", "test0");
    for(i=1; i < 6; i++)
    {
        sprintf(buff, "test%d", i);
        appendStringArray(anker, "testarray", buff);
    }

    printVars(anker);

    parser_start("./test.html", anker, "./test/output.html");

}
