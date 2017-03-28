#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "vars.h"
#include "parser.h"
#include "token_handling.h"
#include "macro.h"

#include "parser_errno.h"

//Returns the type of block found
//Variablenblock = VARIABLENBLOCK
//Commandblock = COMMANDBLOCK
//false = no block was found
int searchBlockBegin(wchar_t *str, wchar_t **pos)
{
    wchar_t *begin;
    int x = 0,
        length = wcslen(str);


    for(; x < length; x++)
    {
        if(memcmp(str+x, VARIABLEBEGIN_STR, sizeof(wchar_t)*2) == 0)
        {
            *pos = str+x;
            return(VARIABLEBLOCK);
        }
        else if(memcmp(str+x, COMMANDBEGIN_STR, sizeof(wchar_t)*2) == 0)
        {
            *pos = str+x;
            return(COMMANDBLOCK);
        }
    }

    return(false);
}

int searchBlockEnd(wchar_t *str, wchar_t **pos, int type)
{
    wchar_t *end;

    if(type == VARIABLEBLOCK)
    {
        if((end = wcsstr(str, VARIABLEEND_STR)) != NULL)
        {
            *pos = end;
            return(true);
        }
        return(false);
    }
    else if(type == COMMANDBLOCK)
    {
        if((end = wcsstr(str, COMMANDEND_STR)) != NULL)
        {
            *pos = end;
            return(true);
        }
        return(false);
    }
    return(false);
}

int saveLine(wchar_t *line, status_t *stat)
{
    if(stat->sizeof_sav_buff == 0)
    {
        stat->save_buff = malloc(sizeof(wchar_t*));
        stat->save_buff[0] = malloc((wcslen(line)+1)*sizeof(wchar_t));
        wcscpy(stat->save_buff[0], line);
        stat->sizeof_sav_buff = 1;
    }
    else
    {
        stat->sizeof_sav_buff++;
        stat->save_buff = realloc(stat->save_buff, sizeof(wchar_t*)*stat->sizeof_sav_buff);
        stat->save_buff[stat->sizeof_sav_buff-1] = NULL;
        stat->save_buff[stat->sizeof_sav_buff-1] = malloc((wcslen(line)+1)*sizeof(wchar_t)+1);
        wcscpy(stat->save_buff[stat->sizeof_sav_buff-1], line);
    }
    return(0);
}

void freeLineBuff(status_t *stat)
{
    int i;

    for(i=stat->sizeof_sav_buff-1; i > -1; i--)
    {
        free(stat->save_buff[i]);
    }
    free(stat->save_buff);
    stat->save_buff = NULL;
    stat->sizeof_sav_buff = 0;
}

int parseLine(wchar_t *line, status_t *status)
{
    wchar_t *inputstr = line,
         *begin,
         *end,
         *prev_end = NULL,
         *backup_line = line;
    size_t len = wcslen(line),
           restlength = len;
    int i,
        inblock = false,
        between_len = 0,
        ret = 0;

    //printf("parse: [%S]\n", inputstr);
    while(restlength != 0) 
    { 
        if((inblock = searchBlockBegin(inputstr, &begin)) != false)
        {
            //Text zwischen den Bloecken ausgeben
            if(prev_end)
            {
                between_len = begin - prev_end;
                //printf("between_len: [%d]\n", between_len);
                fprintf(f_output, "%.*S", between_len, prev_end);
            }
            //Text vor dem ersten Block ausgeben
            if(!prev_end)
            {
                between_len = begin - inputstr;
                if(!status->just_save)
                    fprintf(f_output, "%.*S", between_len, inputstr);
            }
            if(searchBlockEnd(begin, &end, inblock) == false)
            {
                fprintf(logfile, "Syntax Error Missing Endbracked\n");
                parser_errno = MISSING_CMD_BLOCK_END;
                break;
            }
            else if(inblock == VARIABLEBLOCK)
            {
                if(status->just_save)
                {
                    saveLine(line, status);
                    break;
                }
                parseVariable(begin, end);
            }
            else if(inblock == COMMANDBLOCK)
            {
                if((ret = parseCommand(begin, end, status)) == JUSTSAVE)
                {
                    saveLine(backup_line, status);
                    break;
                }
                else if(ret == EXIT)
                    return(EXIT);
                else if(ret < 0)
                    return(EXIT);
            }
            restlength=wcslen(end+2);
            prev_end = inputstr = end+2;
        }
        //Text hinter dem letzen Block ausgeben
        else
        {
            //printf("kenn ich nicht: [%S]\n", inputstr);
            restlength = restlength - wcslen(inputstr);
            if(status->just_save)
                saveLine(line, status);
            else
                fprintf(f_output, "%S", inputstr);
        }
    }

}
