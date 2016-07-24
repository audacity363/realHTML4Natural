#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>

#include "natuser.h"
#include "standard.h"
#include "varhandle.h"
#include "utils.h"

#define VARLENGTH 20 
#define ARRAYLENGTH 20
#define LOGFILEPATH "/tmp/t_engemann.log"
#define NATTIMEOUT 5
#define FILELENGTH 100

int call_subprog(char*, char*);

/*
 * This function just starts the webserver with the context of natural.
 * It is importent for the callnat function from the SAG.
 */
long start_web_server( WORD nparm, void *parmhandle, void *traditional )
{
    start_web_server_from_nat(&call_subprog);
    return((long)0);
}

void nat_signalHandler(int signal)
{
    fprintf(stderr, "Recv Signal: [%d]\n", signal);
}

/*
 * This function will redner a 500 Error Page with the Name 
 * of the called subprog and the errorcode from Natural.
 * The Templatefile can be found in the template/error folder.
 */
int handle_timeout(char *deliver_file, char *program)
{
    struct variables *anker = malloc(sizeof(struct variables));
    char *error_filename = malloc(strlen(deliver_file)+6);
    char *template = malloc(sizeof(webserver_settings.templatepath)+15);
    char error_str[1024];
    FILE *p_errorfile;


    /*Init Anker*/
    strcpy(anker->name, "anker");
    anker->next = NULL;

    sprintf(error_filename, "%s_error", deliver_file);
    sprintf(template, "%s/error/500.html", webserver_settings.templatepath);

    if((p_errorfile = fopen(error_filename, "r")) == NULL)
    {
        printf("%s\n", strerror(errno));
        return(-1);
    }

    fseek(p_errorfile, 1371, SEEK_SET);

    fgets(error_str, sizeof(error_str), p_errorfile);
    fclose(p_errorfile);
    remove(error_filename);

    error_str[strlen(error_str)-5] = '\0';

    newStringVar(anker, "error_str", error_str);
    newStringVar(anker, "programm", program);
    parser_start(template, anker, deliver_file);
}

/*
 * This function will call the given Natural Subprog with the parameters:
 *    Request Values (Length: 20; Type Alpha; Array with index of 20)
 *    Request Keys   (Length: 30; Type Alpha; Array with index of 20)
 *    Request Type   (Length:  4; Type Alpha)
 *    Request File   (!!!Only for internal use!!! 
 *                    This is the file where the rendered template will writen to)
 *                   (Length: 100; Type Alpha)
 *    Request Settings (!!!Only for internal use!!)
 *                      Settings from the webserver for the jinja parser with settings
 *                      in the format: "key1=value1;key2=value2"
 *                      (Length: Dynamic; Type Alpha)
 */      
int call_subprog(char *programm, char *server_deliver_filename)
{
    int i,
        arraylength = ARRAYLENGTH;
    pid_t nat_pid;
    sigset_t mask;
    sigset_t orig_mask;
    int timeout_bool = 0;

    char req_type[] = "POST";
    char req_settings[] = "debug=1";
    char req_file[] = " ";
    char keys[VARLENGTH];
    char testbuff[100];
    char values[VARLENGTH];
    FILE *f_deliver_file;
    key_t sharedmem_key;

    char *deliver_file = tempnam("/tmp", "rh4N");
    strcpy(server_deliver_filename, deliver_file);
    /*Touch tmp file*/
    if((f_deliver_file = fopen(deliver_file, "w")) == NULL)
    {
        printf("Error: [%s]\n", strerror(errno));
        return(-1);
    }
    fclose(f_deliver_file);
    

    printf("generated Deliver File: [%s]\n", deliver_file);

    printf("Starting Subprog [%s]\n", programm);
    nat_pid = fork();
    if(nat_pid == 0)
    {
        //Child
        FILE *error_file;
        int nat_rc;
        struct parameter_description pass[5], test;
        void *myparmhandle;
        char *error_logfile = malloc(strlen(deliver_file)+6);
        int arraylength = ARRAYLENGTH,
            shID = 0;

        sprintf(error_logfile, "%s_error", deliver_file);
        printf("Logging Nat Error into [%s]\n", error_logfile);

        error_file = fopen(error_logfile, "w");
        dup2(fileno(error_file), 1);
        close(fileno(error_file));

        fprintf(stderr, "Create Signal handler\n");
        for(i=0; i < NSIG; i++)
        {
            signal(i, nat_signalHandler);
        }

        
        /*Create Datasets for Subprog Call*/
        ncxr_create_parm(5, &myparmhandle);
        ncxr_init_parm_sa(0, myparmhandle, 'A', VARLENGTH, 0, 1, &arraylength, 0);
        ncxr_init_parm_sa(1, myparmhandle, 'A', VARLENGTH, 0, 1, &arraylength, 0);
        ncxr_init_parm_s (2, myparmhandle, 'A', 4,         0, 0);
        ncxr_init_parm_s (3, myparmhandle, 'A', FILELENGTH,0, 0);
        ncxr_init_parm_d (4, myparmhandle, 'A', 0);


        for(i=0; i < 20; i++)
        {
            memset(keys, 0x20, VARLENGTH);
            memset(values, 0x20, VARLENGTH);
            sprintf(keys, "key %d", i);
            sprintf(values, "value %d", i);
            ncxr_put_parm_array(0, myparmhandle, VARLENGTH, values, &i);
            ncxr_put_parm_array(1, myparmhandle, VARLENGTH, keys, &i);
        }

        if(ncxr_put_parm(2, myparmhandle, 4, req_type) < 0)
        {
            exit(7);
        }

        ncxr_put_parm(3, myparmhandle, strlen(deliver_file), deliver_file);
        ncxr_put_parm(4, myparmhandle, sizeof(req_settings), req_settings);


        memset(pass, 0, sizeof(pass));

        ncxr_get_parm_info(0, myparmhandle, &pass[0]);
        ncxr_get_parm_info(1, myparmhandle, &pass[1]);
        ncxr_get_parm_info(2, myparmhandle, &pass[2]);
        ncxr_get_parm_info(3, myparmhandle, &pass[3]);
        ncxr_get_parm_info(4, myparmhandle, &pass[4]);
        /*End Datasets*/

        fprintf(stderr, "Calling ncxr_if4_callnat\n"); 

        nat_rc = ncxr_if4_callnat(programm, 5, pass);
        exit(0);
    }
    //Parent

    int status, waitrc, i_timeout = 0,
        shID = 0;
    bool timeout = false;
    char *mem;
    char deliver_filename[FILELENGTH];

    /*Wait NATTIMEOUT Seconds for the Natural Subprog
     * to exit. After that it will be killed*/
    printf("Wait for Child to exit\n");    
    do
    {
        waitrc = waitpid(nat_pid, &status, WNOHANG);
        if(waitrc < 0)
        {
            printf("ErrorNr: [%d]\nError [%s]\n", errno, strerror(errno));
            printf("Exit Stat: [%d]\n", status);
            break;
        }
        if(waitrc > 0) 
        {
            printf("Exit Stat: [%d]\n", status);
            break;
        }
        if(++i_timeout >= NATTIMEOUT)
        {
            printf("Timeout\n");
            timeout = true;
            kill(nat_pid, SIGKILL);
            break;
        }
        sleep(1);
        
    }while(1);
    if(timeout == true)
    {    
        /*The Natural Prozess timed out. Probably with an Error*/
        handle_timeout(deliver_file, programm);
    }

    /*Return to the Webserver*/
    return(0);
}
