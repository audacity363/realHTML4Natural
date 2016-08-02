#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "header.h"
#include "filehandling.h"
#include "response.h"

#define HTDOCS "/u/it/a140734/C/sharedlib_jinja2/webserver/htdocs"

//Binary File Extension
static char binary_extensions[6][3] = {"jpg",
                                       "png",
                                       "ico",
                                       "img",
                                       "svg",
                                       "gif"};

bool checkFileExtension(char *ext)
{
    int extlength = sizeof(binary_extensions);
    int i;

    for(i=0; i < extlength; i++)
    {
        if(memcmp(ext, binary_extensions[i], 3) == 0)
            return true;
    }
    return false;
}


int getFilesize(char *file)
{
    FILE *fp;
    int size;
    if((fp = fopen(file, "r")) == NULL)
        return(-1);

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fclose(fp);

    return(size);
}

void deliverFile(char *requestfile, int client, int htdocs)
{
    char filename[500];
    char *name, *ext;
    int content_length;

    if(htdocs == 0)
    {
        printf("deliver [%s]\n", requestfile);
        sprintf(filename, "%s", requestfile);
        content_length = getFilesize(filename);
        sendSuccessHeader(client, "text/html", content_length);
        deliverTextFile(client, filename);
        return;
    }

    if((name = strtok(requestfile, ".")) == NULL)
    {
        internal_error(client);
        return;
    }
    if((ext = strtok(NULL, ".")) == NULL)
    {
        internal_error(client);
        return;
    }

    if(checkFileExtension(ext) == true)
    {
        sprintf(filename, "%s%s.%s", HTDOCS, name, ext);
        content_length = getFilesize(filename);
        sendSuccessHeader(client, "image/gif", content_length);
        deliverBinaryFile(client, filename);

        return;
    }
    printf("deliver File\n");
    if(requestfile[0] != '/')
    {
        sprintf(filename, "%s/%s.%s", HTDOCS, requestfile, ext);
    }
    else
    {
        sprintf(filename, "%s%s.%s", HTDOCS, requestfile, ext);
    }
    content_length = getFilesize(filename);
    sendSuccessHeader(client, "text/html", content_length);
    deliverTextFile(client, filename);

}

int deliverTextFile(int client, char *file)
{

    char buf[1024];                                                                
    int sum_size = 0;
    int bytes_read;
    FILE *resource;

    if((resource = fopen(file, "r")) == NULL)
    {
        page_not_found(client);
        return(0);
    }

    do
    {                                                                              
        fgets(buf, sizeof(buf), resource);
        write_socket(client, buf, strlen(buf));
        sum_size = sum_size + strlen(buf);
    }while(!feof(resource));
    fclose(resource);

    return(sum_size);
}

int deliverBinaryFile(int client, char *file)
{
    FILE *resource;
    char buf[1024];
    int bytes_read;
    int sum_size = 0;

    if((resource = fopen(file, "rb")) == NULL)
    {
        page_not_found(client);
        return;
    }

    do
    {
        bytes_read = fread(buf, 1, sizeof(buf), resource);
        write_socket(client, buf, bytes_read);
        sum_size = sum_size + bytes_read;
        bzero(buf, sizeof(buf));
    }while(!feof(resource));

    fclose(resource);
    return(sum_size);
}
