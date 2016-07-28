#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "file_handling.h"

int parse_line(struct variables *anker, char *line, FILE *p_output,
               char **cmd_buff, int *parser_status, char *error_str)
{
    char *pos_open_var, *pos_close_var;
    char *pos_open_cmd, *pos_close_cmd;
    int cmd_type;

    if((pos_open_var = strstr(line, "{{")) != NULL && *parser_status == 0)
    {
        pos_open_var[0] = '\0';
        fprintf(p_output, "%s", line);

        //Variablen an den Anfang verschieben
        memmove(line, line+(strlen(line)+2), strlen(pos_open_var+1));
        if((pos_close_var = strstr(line, "}}")) == NULL)
        {
            strcpy(error_str, "Syntax Error: Missing \"}}\"");
            return(-1);
        }
        pos_close_var[0] = '\0';

        if(handle_variable(anker, line, p_output, error_str) < 0)
        {
            return(-2);
        }

        fprintf(p_output, "%s", pos_close_var+2);
    }
    else if((pos_open_cmd = strstr(line, "{%")) != NULL) 
    {
        if((pos_close_cmd = strstr(line, "%}")) == NULL)
        {
            strcpy(error_str, "Syntax Error: Missing \"%}\"");
            return(-3);
        }
        
        pos_close_cmd[0] = '\0';
        memmove(line, pos_open_cmd+2, strlen(pos_open_cmd));
        switch(searchCommand(line))
        {
            case -1:
                strcpy(error_str, "Name Error: Unkown Function");
                return(-4);
            //Forschleife:
            case 1:
                if(*cmd_buff != NULL && parser_status != 0)
                {
                    free(*cmd_buff);
                    *cmd_buff = NULL;
                }
                *parser_status = 1;
                *cmd_buff = malloc(strlen(line)+1);
                strcpy(*cmd_buff, line);
                strcat(*cmd_buff, "\n");
                return(0);
            //Forschleife ist geschlossen worden 
            case 2:
                printf("Found end-for\n");
                if(for_handle(anker, *cmd_buff, p_output, error_str) < 0)
                {
                    free(*cmd_buff);
                    return(-1);
                }
                free(*cmd_buff);
                *cmd_buff = NULL;
                *parser_status = 0;
                return(0);
            default:
                strcpy(error_str, "Internal Error: searchCommand() return something unkown");
                return(-5);
        }
    }
    else if(*parser_status > 0)
    {
        *cmd_buff = realloc(*cmd_buff, strlen(*cmd_buff)+strlen(line));
        strcat(*cmd_buff, line);
    }
    else
    {
        fprintf(p_output, "%s", line);
    }

    return(0);
}

int close_jinjaparser(FILE *p_output, FILE *p_template)
{
    char error_str[2048];

    if(p_output != NULL)
        closeOutputFile(p_output, error_str);

    if(p_template != NULL)
        closeTemplateFile(p_template, error_str);
}

int start_jinjaparser(struct variables *anker, char *outputfile,
                      char *templatefile, char *error_str)
{
    FILE *p_template, *p_output;
    char *line, *cmd_buff = NULL;
    int parser_status = 0;

    if((p_template = openTemplateFile(templatefile, error_str)) == NULL)
    {
        return(-1);
    }

    if((p_output = openOutputFile(outputfile, error_str)) == NULL)
    {
        close_jinjaparser(p_output, p_template);
        return(-2);
    }

    while(1)
    {
        if((line = getLinefromTemplate(p_template, error_str)) == NULL)
        {
            if(feof(p_template))
            {
                strcpy(error_str, "");
                break;
            }
            close_jinjaparser(p_output, p_template);
            return(-3);
        }
        /*if(line[strlen(line)-1] != '\n');
        {
            strcpy(error_str, "Length Error: Could not read a full Line");
            close_jinjaparser(p_output, p_template);
            return(-5);
        }*/
        if(parse_line(anker, line, p_output, &cmd_buff, &parser_status, 
                      error_str) < 0)
        {
            close_jinjaparser(p_output, p_template);
            return(-4);
        }
    }

    close_jinjaparser(p_output, p_template);
    return(0);
}
