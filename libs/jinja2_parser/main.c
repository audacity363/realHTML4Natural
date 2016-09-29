#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "standard.h"
#include "varhandle.h"
#include "hexdump.h"
#include "jinja_parser.h"

void fillTestvalues(struct variables *anker)
{
    char tmpbuff[100];
    int i,x;

    struct variables test1;

    wchar_t wctest[10];

    swprintf(wctest, 10, L"lölölö");
    newUStringVar(anker, "utest", wctest, 10);

    newIntVar(anker, "ISADMIN", 1);

    newIntArray(anker, "ADMIN", 20);
    newUStringArray(anker, "uarray", 10, 20);
    for(i=0; i < 20; i++)
    {
        editIntVarArray(anker, "ADMIN", i, i);
        swprintf(wctest, 10, L"lölö %d", i);
        editUStringArray(anker, "uarray", wctest, i);
    }

    new2DIntArray(anker, "i2dtestarray", 5, 5);
    newU2DStringArray(anker, "u2dtest", 10, 5, 5);
    for(i=0; i < 5; i++)
    {
        for(x=0; x < 5; x++)
        {
            swprintf(wctest, 10, L"öö %d %d", i, x);
            logHexDump(wctest, sizeof(wctest), stdout);
            editU2DStringArray(anker, "u2dtest", wctest, i, x);
            editIntVar2DArray(anker, "i2dtestarray", i+x, i, x);
            bzero(wctest, sizeof(wctest));
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

    newStringVar(anker, "test1", "Hello World");

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

    setlocale(LC_ALL, "");

    anker->next = NULL;
    fillTestvalues(anker);

    logHexDump(" ", 2, stdout);

    strcpy(webserver_settings.templatepath, "/u/it/a140734/C/realHtml4Natural/libs/jinja2_parser/");
    //strcpy(webserver_settings.templatepath, "/home/tom/Documents/realHtml4Natural/libs/jinja2_parser/");

    printVars(anker);
    
    //char template[] = "./test_template.html";
    //char template[] = "/home/tom/Documents/realHtml4Natural/libs/jinja2_parser/tshus.html";
    char template[] = "/u/it/a140734/C/realHtml4Natural/libs/jinja2_parser/tshus.html";
    //char template[] = "./utesttm.html";
    //char outputfile[] = "/home/tom/Documents/realHtml4Natural/libs/jinja2_parser/output.html";
    char outputfile[] = "/u/it/a140734/C/realHtml4Natural/libs/jinja2_parser/output.html";
    char error_str[2048];



    if(start_jinjaparser(anker, outputfile, template, error_str, &line_nr) < 0)
    {
        printf("Linenr: [%d]\n", line_nr);
        printf("%s\n", error_str);
        exit(0);
    }
}
