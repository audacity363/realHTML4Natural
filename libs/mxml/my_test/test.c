#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mxml.h"


int main()
{
    mxml_node_t *tree, *route, *prog;
    FILE *xml_fp;

    if((xml_fp = fopen("routes.xml", "r")) == NULL)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(0);
    }

    if((tree = mxmlLoadFile(NULL, xml_fp, MXML_NO_CALLBACK)) == NULL)
    {
        fprintf(stderr, "Can not get XML Tree\n");
        fclose(xml_fp);
        exit(1);
    }

    if((route = mxmlFindElement(tree, tree, "route", "path", "/notizen", MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Can not find route\n");
        fclose(xml_fp);
        exit(1);
    }

    if((prog = mxmlFindElement(route, tree, "prog", NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Can not find prog Entry\n");
        fclose(xml_fp);
        exit(1);
    }

    printf("Prog: [%s]\n", mxmlGetText(prog, NULL));
    
    printf("Habs gefunden\n");
    

    fclose(xml_fp);

}
