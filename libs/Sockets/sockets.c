/*Eine Library um TCP Clients und Server zu erstellen*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "sockets.h"


//Erstellt einen TCP client und verbindet sich auf die uberergebene IP oder
//auf den Hostname auf Port der mit port uebergeben wird.
//Return: Pos: Socket Handle
//        Neg: -1
int create_tcp_client(char *ip, int port)
{
    register int s;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr,
                "create_tcp_server(): Socket konnte nicht erstellt werden\n");
        return(-1);
    }
    
    if((server = gethostbyname(ip)) == NULL)
    {
        printf("create_tcp_client(): Host [%s] nicht gefunden\n", ip);
        return(-1);
    }
    
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    
    if(connect(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr,
                "create_tcp_client(): Fehler bei verbindung mit host\n");
        return(-1);
    }
    return(s);
}

//Erstellt einen TCP Server auf dem Port der mit port uebrgeben wird.
//Return: Pos: Socket Handle
//        Neg: -1
int create_tcp_server(int port)
{
    struct hostent *hp;
    struct sockaddr_in sin;
    int sock;
   
    //create Socket Handle 
    if((hp = gethostbyname("0.0.0.0")) == NULL) 
    {
        exit(-1);
    }


    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("server: socket");
        exit(-1);
    }
    
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    
    bcopy(hp->h_addr, &sin.sin_addr, hp->h_length);
    
    if(bind(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        perror("server: bind");
        exit(-1);
    }

    if(listen(sock, 5) < 0)
    {
        perror("socket: listen");
        exit(-1);
    }
    return(sock);

}

//Wartet auf eine einkommente Verbindung auf dem Socket "hadle"
//Return: Pos: Client Handle
//        Neg: -1
int accept_con(int handle, char *ip)
{
    int c, ch;
    struct sockaddr_in client;

    c = sizeof(struct sockaddr_in);
    if((ch = accept(handle, (struct sockaddr *)&client, (socklen_t*)&c)) < 0)
    {
       printf("Accept_error\n");
       return(-1);
    }
    sprintf(ip, "%d.%d.%d.%d",
      client.sin_addr.s_addr&0xFF,
      (client.sin_addr.s_addr&0xFF00)>>8,
      (client.sin_addr.s_addr&0xFF0000)>>16,
      (client.sin_addr.s_addr&0xFF000000)>>24);
    return(ch);
}

//Schreibt das Char Array daten mit der Laenge nbytes auf den Socket handle
//Return: Pos: eine positive ganz Zahl
//        Neg: -1
int write_socket(int handle, char *daten, int nbytes)
{
    int bytes_written;

    if((bytes_written = write(handle, daten, nbytes)) < 0)
    {
        return(bytes_written);
    }
    return(bytes_written);
}

//liesst nbytes vom Socket handle und schreibt die in das Char Array daten
//Return: Pos: eine positive ganz Zahl
//        Neg: -1
int read_socket(int handle, char *daten, int nbytes)
{
    int bytes_recv;

    if((bytes_recv = read(handle, daten, nbytes)) < 0)
    {
        return(bytes_recv);
    }
    return(bytes_recv);
}

//Blockiert neue einkommende und ausgehende Daten und schliesst den Soocket
//endgueltig
//Return: Pos: 0
//        Neg: -1
int close_socket(int handle)
{
    if(shutdown(handle, 2) < 0)
    {
        fprintf(stderr, "close_socket(): Shutdown Error\n");
        return(-1);
    }
    
    if(close(handle) < 0)
    {
        fprintf(stderr, "close_socket(): close Error\n");
        return(-1);
    }
    return(0);
}
