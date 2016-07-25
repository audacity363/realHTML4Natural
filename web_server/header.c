#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "header.h"
#include "utils.h"
#include "hexdump.h"

void printheader(char **parsedheader, int size)
{
    int i;
    for(i=0; i < size+1; i++)
    {
        logHexDump(parsedheader[i], strlen(parsedheader[i]), stdout);
//        puts(parsedheader[i]);
        puts("\n");
    }
}

int parseGETRequest(httpheader *header)
{
    char *fragezeichen;
    char *get_buff, *pair, *key, *value, *tmp;
    int anzahl = 0;
    
    fragezeichen = strchr(header->page, '?');
    if(fragezeichen == NULL)
    {
        header->request_arguments = 0;
        return(0);
    }

    get_buff = malloc(strlen(fragezeichen));
    strcpy(get_buff, fragezeichen);
    fragezeichen[0] = '\0';
    memmove(get_buff, get_buff+1, strlen(get_buff));

    header->request_keys = malloc(sizeof(char*));
    header->request_values = malloc(sizeof(char*));

    pair = strtok(get_buff, "&");
    while(pair != NULL)
    {
        anzahl++;
        tmp = strchr(pair, '='); 
        if(tmp == NULL)
        {
            //Key hat kein Value
            value = malloc(2);
            strcpy(value, " ");
        }
        else
        {
            tmp[0] = '\0';
            value = malloc(strlen(tmp+1));
            strcpy(value, tmp+1);
        }

        key = malloc(strlen(pair));
        strcpy(key, pair);

        header->request_values = realloc(header->request_values, sizeof(char*)*anzahl);
        ((char**)header->request_values)[anzahl-1] = malloc(strlen(value));
        strcpy(((char**)header->request_values)[anzahl-1], value);

        header->request_keys = realloc(header->request_keys, sizeof(char*)*anzahl);
        ((char**)header->request_keys)[anzahl-1] = malloc(strlen(value));
        strcpy(((char**)header->request_keys)[anzahl-1], key);

        free(key);
        free(value);

        pair = strtok(NULL, "&");
    }

    header->request_arguments = anzahl;
    return(0);
}

httpheader parseheader(char *header, int headerlength)
{
    int i;
    int fieldindex = 0;
    int x = 0;
    int y = 0;

    char *fields[100];
    char *key, *value;
    char *buffer;

    //logHexDump(header, headerlength, stdout);
    printf("%s", header);

    httpheader header_struct;

    fields[fieldindex] = malloc(MAXATTRLENGTH);

    //Einzlnen Eintraege in ein Array Splitten
    for(i=0; i < headerlength+1; i++)
    {
        if((int)header[i] == CARRIAGERETURN && (int)header[i+1] == LINEFEED)
        {
            fields[fieldindex][x] = '\0';
//            printf("Found Line End\n");
            i++;
            x = 0;
            fields[++fieldindex] = malloc(MAXATTRLENGTH);
        }
        else
        {
          fields[fieldindex][x++] = header[i];
        }
    }

    //HTTP Method herausfinden. 
    for(i=0; i < fieldindex; i++)
    {
        if(memcmp(fields[i], GET, sizeof(GET)-1) == 0)
        {
            header_struct.page = malloc(strlen(fields[i])-9);
            y=0;
            for(x=4; x < strlen(fields[i])-9; x++)
            {
                header_struct.page[y++] = fields[i][x];
            }
            header_struct.page = (char*)StripTrailingSpaces(header_struct.page);
            memcpy(header_struct.request_type, GET, sizeof(GET)-1);
        }
        else if(memcmp(fields[i], POST, sizeof(POST)-1) == 0)
        {
            //Post Request. Post Parameter muessen geparst werden
            header_struct.page = malloc(strlen(fields[i])-9);
            y=0;
            for(x=5; x < strlen(fields[i])-9; x++)
            {
                header_struct.page[y++] = fields[i][x];
            }
            memcpy(header_struct.request_type, POST, sizeof(POST)-1);
        }
    }

    //Ersten Eintrag ueberspringen
    //Einzelne Eintraege in ein key/value Paar splitten und bereinigen
    for(i=1; i < fieldindex; i++)
    {
        key = strtok(fields[i], ":");
        if(key == NULL)
            break;
        value = strtok(NULL, ":");
//        printf("Key: [%s]\n Value: [%s]\n", key, value);
        if(memcmp(key, "Host", 4) == 0)
        {
            RemoveSpaces(value);
            sprintf(header_struct.host, "%s:%s", value, strtok(NULL, ":"));
        }
        else if(memcmp(key, "User-Agent", 10) == 0)
        {
            header_struct.user_agent = malloc(strlen(value));
            strcpy(header_struct.user_agent, value);
        }
        else if(memcmp(key, "Connection", 10) == 0)
        {
            header_struct.connection = malloc(strlen(value));
            strcpy(header_struct.connection, value);
        }
    }
    if(strcmp(header_struct.request_type, GET) == 0)
    {
        parseGETRequest(&header_struct);
    }

    return(header_struct);
}
