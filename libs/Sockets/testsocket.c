#include <stdio.h>
#include <stdlib.h>

int main()
{
    int socket, client, bytes_read;
    char buff[100], ip[15];

    if((socket = create_tcp_server(8888)) < 0)
    {
        fprintf(stderr, "Socket konnte nicht erstellt werden\n");
        exit(-1);
    }
    
    if((client = accept_con(socket, ip)) < 0)
    {
        fprintf(stderr, "Fehler beim client Handle\n");
        close_socket(socket);
        exit(-1);
    }

    bytes_read = read_socket(client, buff, sizeof(buff));
    printf("[%d] Bytes readed\n", bytes_read);

    printf("[%d] Bytes written\n", write_socket(client, buff, bytes_read));
    close_socket(client);
    close_socket(socket);
    return(0);
}
