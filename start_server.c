#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#include "natuser.h"

#define VARLENGTH 20 
#define ARRAYLENGTH 20
#define LOGFILEPATH "/tmp/t_engemann.log"

int call_subprog(char*);

long start_web_server( WORD nparm, void *parmhandle, void *traditional )
{
    start_web_server_from_nat(&call_subprog);
    return((long)0);
}

void nat_signalHandler(int signal)
{
    fprintf(stderr, "Recv Signal: [%d]\n", signal);
}

int call_subprog(char *programm)
{
    int i,
        arraylength = ARRAYLENGTH;
    pid_t nat_pid;
    sigset_t mask;
    sigset_t orig_mask;
    struct timespec timeout;
    int timeout_bool = 0;

    char req_type[] = "POST";
    char keys[VARLENGTH];
    char values[VARLENGTH];

    printf("Starting Subprog [%s]\n", programm);
    nat_pid = fork();
    if(nat_pid == 0)
    {
        //Child
        FILE *error_file;
        int nat_rc;
        struct parameter_description pass[3], test;
        void *myparmhandle;
        int arraylength = ARRAYLENGTH;

        error_file = fopen("/tmp/nat_error.log", "w");
        dup2(fileno(error_file), 2);
        close(fileno(error_file));

        for(i=0; i < NSIG; i++)
        {
            signal(i, nat_signalHandler);
        }

        /*Create Datasets for Subprog Call*/
        /*Aus irgendeinem Grund Funktionieren die Parameter hier nicht. 
         * Warum bin ich mir noch nicht ganz sicher, da sich der Prozess ohne ein Signal beendet
         * Ersteinmal werden also keine Parameter mit uebergeben
         */
        /*ncxr_create_parm(4, &myparmhandle);

        ncxr_init_parm_sa(1, myparmhandle, 'A', VARLENGTH, 0, 1, &arraylength, 0);
        ncxr_init_parm_sa(2, myparmhandle, 'A', VARLENGTH, 0, 1, &arraylength, 0);

        switch(ncxr_init_parm_s(3, myparmhandle, 'A', 4, 0, 0))
        {
            case -1:
                exit(2);
            case -2:
                exit(3);
            case -6:
                exit(4);
            case -8:
                exit(5);
            case -9:
                exit(6);
        }


        for(i=0; i < 20; i++)
        {
            sprintf(keys, "key %d", i);
            sprintf(values, "value %d", i);
            ncxr_put_parm_array(1, myparmhandle, VARLENGTH, values, &i);
            ncxr_put_parm_array(2, myparmhandle, VARLENGTH, keys, &i);
        }

        if(ncxr_put_parm(3, myparmhandle, 4, req_type) < 0)
        {
            exit(7);
        }


        memset(pass, 0, sizeof(pass));

        ncxr_get_parm_info(1, myparmhandle, &pass[0]);
        ncxr_get_parm_info(2, myparmhandle, &pass[1]);
        ncxr_get_parm_info(3, myparmhandle, &pass[2]);*/
        /*End Datasets*/

        fprintf(stderr, "Calling ncxr_if4_callnat\n"); 

        //Aufruf mit Parametern !!!FUNKTIONIERT NICHT!!!
        //nat_rc = ncxr_if4_callnat(programm, 3, pass);
        nat_rc = ncxr_if4_callnat(programm, 0, 0);
        fprintf(stderr, "Nat return Code: [%d]\n", nat_rc);
        exit(0);
    }
    //Parent

    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;
    int status, waitrc, i_timeout = 0;


    printf("Wait for Child to exit\n");    
    do
    {
        waitrc = waitpid(nat_pid, &status, WNOHANG);
        if(waitrc < 0)
        {
            printf("Error\n");
            break;
        }
        if(waitrc > 0) 
        {
            break;
        }
        if(++i_timeout >= 10)
        {
            printf("Timeout\n");
            kill(nat_pid, SIGKILL);
            break;
        }
        sleep(1);
        
    }while(1);

    return(0);
}
