#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>

#include "hexdump.h"
#include "standard.h"
#include "varhandle.h"


int main(int argc, char *argv[])
{
    char teststr[13];
    int shID;
    char *mem;
    int key;
    int size;
    struct variables *anker;

    key = atoi(argv[1]);
    size = atoi(argv[2]);
    printf("Key: [%d]\n", key);
    printf("size: [%d]\n", size);

    if((shID = shmget(key, size, IPC_CREAT | 0666)) == -1)
    {
        printf("Error: [%s]\n", strerror(errno));
        exit(-1);
    }

    mem = shmat(shID, 0, 0);
    if(mem ==(char*)-1)
    {
        printf("Error: [%s]\n", strerror(errno));
        exit(-1);
    }
    anker = (struct variables*)mem;
    printVars(anker);

    //logHexDump(mem, size, stdout);

    shmdt(mem);

}
