#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "lda_parser.h"
#include "utils.h"

int main()
{
    struct variables *anker = malloc(sizeof(struct variables));
    char lda[] = "/home/tom/Documents/realHtml4Natural/var2names/minilda.NSL";
    char template[] = "/home/tom/Documents/realHtml4Natural/var2names/test.html";
    char error_str[2048];

    anker->next = NULL;
    strcpy(anker->name, "anker");

    newStringVar(anker, "var1", "Test");
    newStringVar(anker, "var2", "Test1");
    newIntVar(anker, "var3", 5);


    generate_page(anker, lda, template,
            "/home/tom/Documents/realHtml4Natural/var2names/testprogs/output.html");

    /*if(var2name(anker, lda, error_str) < 0)
    {
        printf("Error: [%s]\n", error_str);
        return(0);
    }
    printVars(anker);*/
}
