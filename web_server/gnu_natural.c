#include <stdio.h>
#include <stdlib.h>

#include "standard.h"

#include "httpapi.h"
#include "httpint.h"
#include "lookup.h"
#include "natural.h"
#include "hexdump.h"


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
    int i;

    printf("There are [%d] Arguments\n", phsSocket->request.pchParamNumber);
    printf("Arguments:\n");
    for(i=0; i < phsSocket->request.pchParamNumber; i++)
    {
        printf("(Length: [%d]) [%s]=[%s]\n", phsSocket->request.stParams[i].valueLength,
            phsSocket->request.stParams[i].pchParamName, 
            phsSocket->request.stParams[i].pchParamValue);
    }
    return(0);
}
