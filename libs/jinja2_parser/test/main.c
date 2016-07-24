#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "parser.h"
#include "varhandle.h"
#include "utils.h"

int main()
{
    int i;
    char buff[20];
    char error_str[2048];
    int errorzeile;

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

    if(parser_start("./test.html", anker, "./test/output.html", error_str,
        &errorzeile) < 0)
    {
        printf("An Error in zeile %d [%s]\n", errorzeile, error_str);
        printf("[%s]\n", getLineFromFile("./test.html", errorzeile));
    }

}
