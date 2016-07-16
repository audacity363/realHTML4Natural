#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "varhandle.h"
#include "parser.h"

extern int parser_start(char *, struct variables*, char*);

int generate_page(struct variables *anker, char *ldaname, char *templatename)
{
    char testlda[] = "/VAW/natural/logi/fuser63/TGAP0734/SRC/TESTLDA.NSL";
    char testoutput[] = "/tmp/t_output.log";
    char testtemplate[] = "/u/it/a140734/C/sharedlib_jinja2/var2names/test.html";

    var2name(anker, testlda);
    parser_start(testtemplate, anker, testoutput);
}
