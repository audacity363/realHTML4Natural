#include <stdio.h>
#include <stdlib.h>

#include "standard.h"

#include "httpapi.h"
#include "httpint.h"
#include "lookup.h"
#include "natural.h"


path_struct checkifNaturalPath(HttpParam* hp, char *url)
{
    printf("looking Up: [%s]\n", url);
    path_struct settings;

    settings = getpathsettings(url, hp->config_path);
    return(settings);
}

int callNaturalProgram(HttpParam *hp, HttpSocket* phsSocket,
                      path_struct settings)
{
    printf("Call Natural Program: [%s]\n", settings.programm);
}
