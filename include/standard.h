#define PORT 8080
#define true 1
#define false 0
#define BUFFERSIZE 1025
#define MAXATTRLENGTH 500
#define MAXSTRINGLENGTH 500

#define DEBUG 
 
 //Kurzform fuer strcmp
#define cmp(X,Y) (strcmp(X,Y)==0)
#define ncmp(X,Y) (strncmp(X,Y,sizeof(Y)-1)==0)
 
typedef int bool;

struct web_server_settings {
    char templatepath[1024];
    char natlibrary[10];
    char nat_sourcepath[1024];
};

#include "standard.c"
