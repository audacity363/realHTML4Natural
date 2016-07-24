#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/standard.h"
#include "lookup.h"
#include "cfg.h"

path_struct getConfig(char *requestpath, char *filepath, char **error_str)
{
    path_struct settings;
    char debug_buff[20];

    if(OpenCfg(filepath, requestpath, error_str) < 0)
    {
        settings.found = 0;
        return(settings);
    }

    settings.found = 1;

    if(GetCfgToken(PROGRAMM, error_str, 0) == NULL)
    {
        settings.found_programm = 0;
    }
    else
    {
        strcpy(settings.programm, GetCfgToken(PROGRAMM, error_str, 0));
        settings.found_programm = 1;
    }

    if(GetCfgToken(ALIAS, error_str, 0) == NULL)
    {
        settings.found_alias = 0;
    }
    else
    {
        strcpy(settings.alias, GetCfgToken(ALIAS, error_str, 0));
        settings.found_alias = 1;
    }

    if(GetCfgToken(RETURN, error_str, 0) == NULL)
    {
        settings.found_returncode = 0;
    }
    else
    {
        strcpy(settings.returncode, GetCfgToken(RETURN, error_str, 0));
        settings.found_returncode = 1;
    }
    if(GetCfgToken(DEBUG, error_str, 0) == NULL)
    {
        settings.debug = 0;
        settings.found_debug = 0;
    }
    else
    {
        strcpy(debug_buff, GetCfgToken(DEBUG, error_str, 0));
        if(strcmp("yes", debug_buff) == 0)
        {
            settings.debug = 1;
        }
        else
        {
            settings.debug = 0;
        }
        settings.found_debug = 1;
    }

    CloseCfg();
    return(settings);
}

path_struct getpathsettings(char *requestpath, FILE *logfile)
{
    char *error_str = malloc(100);
    path_struct settings = getConfig(requestpath, "/u/it/a140734/C/sharedlib_jinja2/web_server/test.cfg", &error_str);
    if(settings.found == 0)
    {
        fprintf(logfile, "Error: [%s]\n", error_str);
    }
    free(error_str);
    return(settings);
}
