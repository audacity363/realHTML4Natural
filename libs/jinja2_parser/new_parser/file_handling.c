#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "file_handling.h"


FILE *openOutputFile(char *filename, char *error_str)
{
    FILE *p_output;

    if((p_output = fopen(filename, "w")) == NULL)
    {
        sprintf(error_str, "Can not open outputfile: [%s] because [%s]",
                filename, strerror(errno));
        return(NULL);
    }
    return(p_output);
}

int closeOutputFile(FILE *p_output, char *error_str)
{
    if(fflush(p_output) != 0)
    {
        sprintf(error_str, "Error while flushing outputfile: [%s]",
            strerror(errno));
        return(-1);
    }

    if(fclose(p_output) != 0)
    {
        sprintf(error_str, "Error while closing outputfile: [%s]",
            strerror(errno));
        return(-2);
    }
    return(0);
}

FILE *openTemplateFile(char *filename, char *error_str)
{
    FILE *p_template;

    if((p_template = fopen(filename, "r")) == NULL)
    {
        sprintf(error_str, "Can not open Template: [%s] because [%s]",
                filename, strerror(errno));
        return(NULL);
    }
    return(p_template);
}

char *getLinefromTemplate(FILE *p_template, char *error_str)
{
    static char line_buffer[LINELENGTH];

    if(fgets(line_buffer, sizeof(line_buffer), p_template) == NULL)
    {
        sprintf(error_str, "Error while reading templatefile [%s]",
                strerror(errno));
        return(NULL);
    }
    return(line_buffer);
}

int  closeTemplateFile(FILE *p_template, char *error_str)
{
    if(fclose(p_template) != 0)
    {
        sprintf(error_str, "Error while closing template: [%s]",
            strerror(errno));
        return(-1);
    }
    return(0);
}
