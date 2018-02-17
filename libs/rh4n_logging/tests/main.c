#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "rh4n_logging.h"

int main() {
    long i = 0;
    RH4nLogrule *rule = rh4n_create_log_rule("TESTLIB", "TESTPROG", RH4N_INFO, "./");
  
    for(;i < 10000; i++) {
        rh4n_log_fatal(rule, "Hello World");
        rh4n_log_debug(rule, "long i = %ld", i);
    }
    
    rh4n_del_log_rule(rule);
}
