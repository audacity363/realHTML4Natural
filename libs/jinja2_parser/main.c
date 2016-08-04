#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "hexdump.h"
#include "jinja_parser.h"

void fillTestvalues(struct variables *anker)
{
    char tmpbuff[100];
    int i,x;

    newIntVar(anker, "ISADMIN", 1);

    newIntArray(anker, "ADMIN", 0);
    for(i=1; i < 20; i++)
    {
        appendIntArray(anker, "ADMIN", i);
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

    newStringArray(anker, "username", "value 0");
    for(i=1; i < 10; i++)
    {
        sprintf(tmpbuff, "value %d", i);
        appendStringArray(anker, "username", tmpbuff);
    }
    for(i=11; i < 20; i++)
    {
        appendStringArray(anker, "username", " ");
    }

    newStringArray(anker, "links", "value2 0");
    for(i=1; i < 20; i++)
    {
        sprintf(tmpbuff, "links %d", i);
        appendStringArray(anker, "testarray2", tmpbuff);
    }
}

int main()
{
    //Create Testvalue
    struct variables *anker = malloc(sizeof(struct variables));
    int line_nr; 

    anker->next = NULL;
    fillTestvalues(anker);

    logHexDump(" ", 2, stdout);

    strcpy(webserver_settings.templatepath, "/u/it/a140734/C/realHtml4Natural/libs/jinja2_parser/");

    printVars(anker);
    
    char template[] = "./test_template.html";
    char outputfile[] = "./output.html";
    char error_str[2048];

    if(start_jinjaparser(anker, outputfile, template, error_str, &line_nr) < 0)
    {
        printf("%s\n", error_str);
        exit(0);
    }
}
