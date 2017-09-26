#include <stdio.h>
#include "sockets.h"

int main()
{
    int handle = create_tcp_client("127.0.0.1", 1);
    close_socket(handle);
}
