#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mxml.h>

#include "standard.h"
#include "lookup.h"
#include "cfg.h"

path_struct getConfig(char *requestpath, char *filepath, char *error_str)
{
    static path_struct settings;
    char debug_buff[20];
    FILE *xml_fp;
    mxml_node_t *tree, *route, *prog, *alias, *return_code, *library;

    settings.error = false;

    if((xml_fp = fopen(filepath, "r")) == NULL)
    {
        strcpy(error_str, strerror(errno));
        settings.error = true;
        return(settings);
    }

    if((tree = mxmlLoadFile(NULL, xml_fp, MXML_NO_CALLBACK)) == NULL)
    {
        strcpy(error_str, "Can not get XML Tree");
        settings.error = true;
        fclose(xml_fp);
        return(settings);
    }

    if((route = mxmlFindElement(tree, tree, ROUTE, PATH_PARM, requestpath, MXML_DESCEND_FIRST)) == NULL)
    {
        fprintf(stderr, "Can not find route\n");
        settings.found = false;
        fclose(xml_fp);
        return(settings);
    }
    settings.found = true;

    if((prog = mxmlFindElement(route, tree, PROGRAMM, NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        settings.found_programm = false;
    }
    else
    {
        settings.found_programm = true;
        strcpy(settings.programm, mxmlGetText(prog, NULL));
    }

    if((alias = mxmlFindElement(route, tree, ALIAS, NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        settings.found_alias= false;
    }
    else
    {
        settings.found_alias= true;
        strcpy(settings.alias, mxmlGetText(alias, NULL));
    }

    if((return_code = mxmlFindElement(route, tree, RETURN, NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        settings.found_returncode = false;
    }
    else
    {
        settings.found_returncode = true;
        strcpy(settings.returncode, mxmlGetText(return_code, NULL));
    }

    if((library = mxmlFindElement(route, tree, LIBRARY, NULL, NULL, MXML_DESCEND_FIRST)) == NULL)
    {
        settings.found_library = false;
    }
    else
    {
        settings.found_library = true;
        strcpy(settings.library, mxmlGetText(library, NULL));
    }
    
    fclose(xml_fp);

    return(settings);
}

path_struct getpathsettings(char *requestpath, char *config_file, char *error_str)
{

    path_struct settings = getConfig(requestpath, config_file, error_str);
    if(settings.found == false)
    {
        fprintf(stderr, "Error: [%s]\n", error_str);
    }
    return(settings);
}
