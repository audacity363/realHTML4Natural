int create_tcp_client(char *ip, int port);
int create_tcp_server(int port);
int write_socket(int handle, char *daten, int nbytes);
int read_socket(int handle, char *daten, int nbytes);
int close_socket(int handle);
int accept_con(int handle, char *ip);
