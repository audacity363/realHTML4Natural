#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "sockets.h"
#include "hexdump.h"
#include "standard.h"
#include "header.h"
#include "lookup.h"
#include "httpcodes.h"
#include "filehandling.h"
#include "response.h"
#include "server.h"

#undef PORT
#define PORT 1024


int serverhandle;
void signalhandler(int);
extern void call_nat_handler(char *programm, char* deliver_file);

FILE *logfile;

bool checkforFile(char *requestpath)
{
    int i;

    for(i=0; i < strlen(requestpath); i++)
    {
        if(requestpath[i] == '.')
        {
            return(true);
        }
    }
    return(false);
}


void handleclient(int client, char *ip, int (*nat_handler)(char*, char*, httpheader))
{
    char *headerbuffer = malloc(BUFFERSIZE);
    char *deliver_file = malloc(100);
    int bytes_read;
    httpheader header;
    path_struct settings;
    int natrc;

    bytes_read = read_socket(client, headerbuffer, BUFFERSIZE);

    if(bytes_read == 0)
    {
        printf("IP: [%s] ERROR: Nichts empfangen\n", ip);
        return;
    }
    
    headerbuffer[bytes_read+1] = '\0';

    header = parseheader(headerbuffer, bytes_read);
    printf("IP: [%s] REQUEST: %s\n", ip, header.page);

    if(checkforFile(header.page) == true)
    {
        printf("Deliver File [%s]\n", header.page);
        deliverFile(header.page, client, 1);
        return;
    }
    settings = getpathsettings(header.page, logfile);
    if(settings.found == true)
    {
        printf("found: [%d]\n", settings.found);
        printf("programm: [%d][%s]\n", settings.found_programm, settings.programm);
        printf("alias: [%d][%s]\n", settings.found_alias, settings.alias);
        printf("return: [%d][%s]\n", settings.found_returncode, settings.returncode);
        printf("library: [%d][%s]\n", settings.found_library, settings.library);

        if(settings.found_returncode == true)
        {
            if(cmp(settings.returncode, NOT_FOUND))
            {
                page_not_found(client);
            }
            else if(cmp(settings.returncode, FORBIDDEN))
            {
                page_forbidden(client);
            }
            else if(cmp(settings.returncode, NOT_ALLOWED))
            {
                method_not_allowed(client);
            }
            else if(cmp(settings.returncode, SERVER_ERROR))
            {
                internal_error(client);
            }
            else
            {
                page_unauthorized(client);
            }
            return;
        }
        else if(settings.found_alias == true)
        {
            deliverFile(settings.alias, client, 1);
            return;
        }
        //Programm ausfueren und auf return warten
        //sendSuccessHeader(client, "text/html", 7);
        printf("Start Subprog\n");
        natrc = nat_handler(settings.programm, deliver_file, header);
        printf("Nat Returncode: [%d]\n", natrc);

        deliverFile(deliver_file, client, 0);
        if(remove(deliver_file) != 0)
        {
            printf("Error while deleting [%s]\n", deliver_file);
        }

        //write_socket(client, "OK Habs", 7);
        return;
    }
    page_not_found(client);
    printf("IP: [%s] ERROR: 404 (%s)\n", ip, header.page);
}

int start_web_server_from_nat(int (*nat_handler)(char*, char*, httpheader))
{
    int socket, client, bytes_read;
    char ip[15], buff[BUFFERSIZE];

    if((logfile = fopen("/tmp/nat_webserver.log", "w")) == NULL)
    {
        return(-1);
    }

    signal(SIGCHLD, SIG_IGN);

    dup2(fileno(logfile), 1);
    close(fileno(logfile));

    strcpy(webserver_settings.templatepath, "/u/it/a140734/C/realHtml4Natural/web_server/templates");
    strcpy(webserver_settings.natlibrary, "TGAP0734");
    strcpy(webserver_settings.nat_sourcepath, "/VAW/natural/logi/fuser63/");

    printf("Create Signal listner\n");
    fflush(logfile);
    signal(SIGINT, signalhandler);
    signal(SIGKILL, signalhandler);

    printf("Create Socket\n");
    if((serverhandle = create_tcp_server(PORT)) < 0)
    {
        printf("Can not create socket\n");
        exit(-1);
    }

    printf("Socket created [%d]\n", serverhandle);

    while(true)
    {
        if((client = accept_con(serverhandle , ip)) < 0)
        {
            printf("Error: Can not create client socket\n");
            close_socket(client);
            continue;
        }
        int newpid = fork();
        if(newpid == -1)
        {
            printf("Error: Fork fehlgeschlagen\n");
        }
        if(newpid == 0)
        {
            //Handler fuer den Client
            handleclient(client, ip, nat_handler);
            close_socket(client);
            exit(0);
        }
    }

    close_socket(serverhandle);
}

void signalhandler(int signal)
{
    fclose(logfile);
    printf("Received Signal\nStopping Server\n");
    if(signal == SIGINT)
    {
        close_socket(serverhandle);
    }
    else if(signal == SIGKILL)
    {
        close_socket(serverhandle);
    }
    exit(0);
}
