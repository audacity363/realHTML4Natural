#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/standard.h"

#define SERVER_STRING "Server: nathttp/0.0.1\r\n"

void page_not_found(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, SERVER_STRING);
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "Content-Type: text/html\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "your request because the resource specified\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "is unavailable or nonexistent.\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "</BODY></HTML>\r\n");
    write_socket(client, buf, strlen(buf));

}

void page_forbidden(int client)
{
    write_socket(client, "403", 3);
}

void method_not_allowed(int client)
{
    write_socket(client, "405", 3);
}

void internal_error(int client)
{
    char buf[1024];

    sprintf(buf, "HTTP/1.0 500 Internal Server Error\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "\r\n");
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "<p>Error prohibited CGI execution.</p>\r\n");
    write_socket(client, buf, strlen(buf));
}

void page_unauthorized(int client)
{
    write_socket(client, "401", 3);
}

void sendSuccessHeader(int client, char *filetype, int content_length)
{
    char buf[1024];

    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    write_socket(client, buf, strlen(buf));
    strcpy(buf, SERVER_STRING);
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "Content-Type: %s\r\n", filetype);
    write_socket(client, buf, strlen(buf));
    sprintf(buf, "Content-Length: %d\r\n", content_length);
    write_socket(client, buf, strlen(buf));
    strcpy(buf, "\r\n");
    write_socket(client, buf, strlen(buf));
}

