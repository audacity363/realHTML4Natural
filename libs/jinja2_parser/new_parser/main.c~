#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"

void fillTestvalues(struct variables *anker)
{
    char tmpbuff[100];
    int i,x;

    newIntVar(anker, "itest", 25);

    newIntArray(anker, "itestarray", 0);
    for(i=1; i < 6; i++)
    {
        appendIntArray(anker, "itestarray", i);
    }

    new2DIntArray(anker, "i2dtestarray", 5, 5);
    for(i=0; i < 5; i++)
    {
        for(x=0; x < 5; x++)
        {
            editIntVar2DArray(anker, "i2dtestarray", i+x, i, x);
        }
    }

    new2DStringArray(anker, "s2dtestarray", 5, 6);
    for(i=0; i < 5; i++)
    {
        for(x=0; x < 6; x++)
        {
            sprintf(tmpbuff, "value %d %d", i, x);
            editStringVar2DArray(anker, "s2dtestarray", tmpbuff, i, x);
        }
    }


    newStringVar(anker, "test", "Hello World");

    newStringArray(anker, "testarray", "value 0");
    for(i=1; i < 10; i++)
    {
        sprintf(tmpbuff, "value %d", i);
        appendStringArray(anker, "testarray", tmpbuff);
    }
}

int main()
{
    //Create Testvalue
    struct variables *anker = malloc(sizeof(struct variables));
    anker->next = NULL;
    fillTestvalues(anker);

    printVars(anker);
    
    char template[] = "./test_template.html";
    char outputfile[] = "./output.html";
    char error_str[2048];

    if(start_jinjaparser(anker, outputfile, template, error_str) < 0)
    {
        printf("%s\n", error_str);
        exit(0);
    }
}
