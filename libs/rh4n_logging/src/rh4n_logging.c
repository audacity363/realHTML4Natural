#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "rh4n_logging.h"

#define MAX_PREFIX_LEN 2024
#define DATETIME_LEN 24

const char *rh4nLoggingGetLevelStr(int);

RH4nLogrule *rh4nLoggingCreateRule(const char *library, const char *program, const int level, const char *logpath) {
    RH4nLogrule *a_logLevel = NULL;

    if(!(a_logLevel = calloc(1, sizeof(RH4nLogrule)))) {
        return(NULL);
    }

    a_logLevel->level = level;

    strncpy(a_logLevel->nat_library, library, sizeof(a_logLevel->nat_library)-1);
    strncpy(a_logLevel->nat_program, program, sizeof(a_logLevel->nat_program)-1);
    strncpy(a_logLevel->logpath, logpath, sizeof(a_logLevel->logpath)-1);

    return(a_logLevel);
}

void rh4n_del_log_rule(RH4nLogrule *a_rule) {
    free(a_rule);
}

void rh4n_log(RH4nLogrule *rule, int level, 
    const char *file, const char *func, long line,
    const char *format, ...) {

    if(rule == NULL)  { return; }
    if(level < rule->level) { return; } 

    char logprefix[MAX_PREFIX_LEN+1],
         datetime_buff[DATETIME_LEN+1],
         datebuff[11],
         *endformat = NULL,
         filepath[3000];
    const char *level_str = NULL;
    int formatlen = 0;
    time_t rawtime;
    struct tm *timeinfo;
    struct timeval millis;
    va_list args;
    pid_t pid = 0;

    FILE *output = NULL;

    memset(logprefix, 0x00, MAX_PREFIX_LEN);

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    gettimeofday(&millis, NULL);

    strftime(datetime_buff, sizeof(datetime_buff), "%d.%m.%Y - %H:%M:%S", timeinfo);

    if(!(level_str = rh4nLoggingGetLevelStr(level))) 
        return;

    pid = getpid();

    if(rule->level == RH4N_DEVELOP) {
        snprintf(logprefix, MAX_PREFIX_LEN, "%s.%06d [%-8s->%-8s@%d] %-7s (%s@%s:%ld) - ", datetime_buff, millis.tv_usec, 
            rule->nat_library, rule->nat_program, pid, level_str, func, file, line);
    } else {
        snprintf(logprefix, MAX_PREFIX_LEN, "%s.%06d [%-8s->%-8s@%d] %-7s - ", datetime_buff, millis.tv_usec, 
            rule->nat_library, rule->nat_program, pid, level_str);
    }

    strftime(datebuff, sizeof(datebuff), "%d.%m.%Y", timeinfo);
    sprintf(filepath, "%s/rh4n_%s_%s_%s.log", rule->logpath, rule->nat_library, rule->nat_program, datebuff);
    
   
    formatlen = snprintf(NULL, 0, "%s%s\n", logprefix, format);
    if((endformat = calloc(sizeof(char), formatlen+1)) == NULL) {
        fprintf(stderr, "Logging: calloc returned NULL!!\n");
        return;
    }
    snprintf(endformat, formatlen+1, "%s%s\n", logprefix, format);

    va_start(args, format);
    if((output = fopen(filepath, "a")) == NULL) {
        fprintf(stderr, "Could not open logfile [%s]. Error: [%s]\n", filepath, strerror(errno));
        return;
    }
    vfprintf(output, endformat, args);
    fclose(output);
    va_end(args);

    free(endformat);
    return;
}

const char *rh4nLoggingGetLevelStr(int level) {
    static char *level_strs[] = {
        "DEVELOP",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
   };

    if(level < 0 || level > (sizeof(level_strs)/sizeof(char*))) 
        return(NULL);

    return(level_strs[level]);
}

int rh4nLoggingConvertStrtoInt(const char* cloglevel) {
    if(strcmp(cloglevel, "DEVELOP") == 0) 
        return(RH4N_DEVELOP);
    if(strcmp(cloglevel, "DEBUG") == 0) 
        return(RH4N_DEBUG);
    if(strcmp(cloglevel, "INFO") == 0) 
        return(RH4N_INFO);
    if(strcmp(cloglevel, "WARN") == 0) 
        return(RH4N_WARN);
    if(strcmp(cloglevel, "WARNING") == 0) 
        return(RH4N_WARN);
    if(strcmp(cloglevel, "ERROR") == 0) 
        return(RH4N_ERROR);
    if(strcmp(cloglevel, "FATAL") == 0) 
        return(RH4N_FATAL);
    return(-1);
}
