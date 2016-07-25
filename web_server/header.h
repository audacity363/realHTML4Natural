#define GET "GET"
#define POST "POST"

//Nicht darstellbare ASCII Zeichen
#define CARRIAGERETURN 0x0D
#define LINEFEED       0x0A

typedef struct {
    char http_version[8];
    char request_type[5];
    char host[100];
    char *user_agent;
    char *page;
    char *connection;
    int request_arguments;
    char *request_keys;
    char *request_values;
} httpheader;


httpheader parseheader(char*, int);
