#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "token_handling.h"
#include "command_parsing.h"
#include "if.h"
#include "for.h"
#include "json.h"
#include "macro.h"

#include "parser_errno.h"

int printVars_handling(token_t*, status_t*);
int typeof_handling(token_t*, status_t*);
int genJSON_handling(token_t*, status_t*);
int exit_handling(token_t*, status_t*);

#define FUNCTION_DIC_LENGTH 10
wchar_t function_dic_c[FUNCTION_DIC_LENGTH][50] = {
        L"if",
        L"end-if",
        L"for",
        L"end-for",
        L"printVars",
        L"typeof",
        L"genJSON",
        L"exit",
        L"macro",
        L"end-macro"
    };

int (*function_dic_v[FUNCTION_DIC_LENGTH])(token_t *anker, status_t *stat) = {
        if_handling,
        end_if_handling,
        for_handling,
        end_for_handling,
        printVars_handling,
        typeof_handling,
        genJSON_handling,
        exit_handling,
        macro_handling,
        end_macro_handling
    };

int exit_handling(token_t *anker, status_t *stat)
{
    if(stat->just_save)
        return(JUSTSAVE);

    return(EXIT);
}

int typeof_handling(token_t *anker, status_t *stat)
{
    if(stat->just_save)
        return(JUSTSAVE);

    int ret = 0, i = 0;
    char c_name_buff[500];
    wchar_t wc_name_buff[500];
    token_t *hptr = anker;
    
    while(hptr) 
    {
        if(hptr->type == CLINGTO)
        {
            hptr = hptr->next;
            continue;
        }
        if(hptr->type == CLAMPS)
            break;
        if(hptr->type == CHAR)
            memcpy(wc_name_buff+(i++), &hptr->val, sizeof(wchar_t));

        hptr = hptr->next;
    }
    memset(wc_name_buff+i, 0x00, sizeof(wchar_t));
    
    //TODO: Add Error Handling
    wcstombs(c_name_buff, wc_name_buff, wcslen(wc_name_buff)+1);

    if((ret = getVarType(vars_anker, NULL, c_name_buff)) == -1)
    {
        parser_errno = NOT_DEFINED_VAR;
        return(EXIT);
    }
    else if(ret == -2)
    {
        parser_errno = NOT_DEFINED_VAR;
        return(EXIT);
    }
    switch(ret)
    {
        case GROUP:
            fprintf(f_output, "Group Leader\n");
            break;
        case INTEGER:
            fprintf(f_output, "integer\n");
            break;
        case ONEDINTEGER:
            fprintf(f_output, "one dimension integer array\n");
            break;
        case TWODINTEGER:
            fprintf(f_output, "two dimension integer array\n");
            break;
        case THREEDINTEGER:
            fprintf(f_output, "three dimension integer array\n");
            break;
        case STRING:
            fprintf(f_output, "string\n");
            break;
        case ONEDSTRING:
            fprintf(f_output, "one dimension string array\n");
            break;
        case TWODSTRING:
            fprintf(f_output, "two dimension string array\n");
            break;
        case THREEDSTRING:
            fprintf(f_output, "three dimension string array\n");
            break;
        case BOOL:
            fprintf(f_output, "boolean\n");
            break;
        case ONEDBOOL:
            fprintf(f_output, "one dimension boolean array\n");
            break;
        case TWODBOOL:
            fprintf(f_output, "two dimension boolean array\n");
            break;
        case THREEDBOOL:
            fprintf(f_output, "three dimension boolean array\n");
            break;
        case FLOAT:
            fprintf(f_output, "float\n");
            break;
        case ONEDFLOAT:
            fprintf(f_output, "one dimension float array\n");
            break;
        case TWODFLOAT:
            fprintf(f_output, "two dimension float array\n");
            break;
        case THREEDFLOAT:
            fprintf(f_output, "three dimension float array\n");
            break;
    }
    return(0);
}

int genJSON_handling(token_t *anker, status_t *stat)
{
    //TODO: Add index parsing
    if(stat->just_save)
        return(JUSTSAVE);
    exec_json(anker, stat);
    return(0);
}

int printVars_handling(token_t *anker, status_t *stat)
{
    if(stat->just_save)
        return(JUSTSAVE);
    fprintf(f_output, "<pre>");
    printAllVarsToFile(vars_anker, f_output);
    fprintf(f_output, "</pre>\n");
    return(0);
}

int (*getCommandFunction(wchar_t *cmd_name))(token_t *anker, status_t *stat)
{
    int i;

    for(i=0; i <= FUNCTION_DIC_LENGTH; i++)
    {
        if(wcscmp(function_dic_c[i], cmd_name) == 0)
            return(function_dic_v[i]);
    }
    return(NULL);
}

int getCommandLength(token_t *anker, int *end_cmd_offset)
{
    token_t *hptr = anker->next;
    int length = 0,
        found_start = false,
        offset = 0;

    while(hptr)
    {
        if(hptr->type == SPACE && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == false)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-1);
        }
        else if(hptr->type == CLAMPS)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-2);
        }
        else if(hptr->type == CHAR)
        {
            found_start = true;
            length++;
        }
        hptr = hptr->next;
        offset++;
    }
    *end_cmd_offset = offset;
    return(length);
}

int getCommandName(token_t *anker, wchar_t *cmd_name)
{
    token_t *hptr = anker->next;
    int length = 0,
        found_start = false,
        offset = 0;

    while(hptr)
    {
        if(hptr->type == SPACE && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == true)
        {
            break;
        }
        else if(hptr->type == CLINGTO && found_start == false)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-1);
        }
        else if(hptr->type == CLAMPS)
        {
            fprintf(stderr, "Unexpected Token\n");
            return(-2);
        }
        else if(hptr->type == CHAR)
        {
            memcpy(cmd_name+length, &hptr->val, sizeof(wchar_t));
            found_start = true;
            length++;
        }
        hptr = hptr->next;
        offset++;
    }

    memset(cmd_name+length, 0x00, sizeof(wchar_t));
    return(0);
}


int lineToTokens(token_t *anker, wchar_t *begin, wchar_t *end)
{
    wchar_t *curpos = begin;
    int i = 0;

    while(curpos+i != end)
    {
        if(wcsncmp(curpos+i, L"{", 1) == 0)
            addToken(anker, curpos+i, BLOCKSTART);
        else if(wcsncmp(curpos+i, L"%", 1) == 0)
            addToken(anker, curpos+i, CMDSTARTEND);
        else if(wcsncmp(curpos+i, L"}", 1) == 0)
            addToken(anker, curpos+i, BLOCKEND);
        else if(wcsncmp(curpos+i, L"[", 1) == 0)
            addToken(anker, curpos+i, INDEXOPEN);
        else if(wcsncmp(curpos+i, L"]", 1) == 0)
            addToken(anker, curpos+i, INDEXCLOSE);
        else if(wcsncmp(curpos+i, L" ", 1) == 0)
            addToken(anker, curpos+i, SPACE);
        else if(wcsncmp(curpos+i, L"\"", 1) == 0)
            addToken(anker, curpos+i, STR);
        else if(wcsncmp(curpos+i, L"=", 1) == 0)
            addToken(anker, curpos+i, EQUALS);
        else if(wcsncmp(curpos+i, L">", 1) == 0)
            addToken(anker, curpos+i, GREATERTHEN);
        else if(wcsncmp(curpos+i, L"<", 1) == 0)
            addToken(anker, curpos+i, LESSTHEN);
        else if(wcsncmp(curpos+i, L"(", 1) == 0)
            addToken(anker, curpos+i, CLINGTO);
        else if(wcsncmp(curpos+i, L")", 1) == 0)
            addToken(anker, curpos+i, CLAMPS);
        else if(wcsncmp(curpos+i, L",", 1) == 0)
            addToken(anker, curpos+i, COMMA);
        else
            addToken(anker, curpos+i, CHAR);
        i++;
    }
    return(0);
}


int parseCommand(wchar_t *begin, wchar_t *end, status_t *stat)
{
    token_t anker, *offset = NULL;
    wchar_t *cmd_name = NULL;

    int cmd_name_length, end_cmd_offset, ret;
    int (*cmd_func)(token_t *anker, status_t *stat);

    macro_definition_t *macro = NULL;

    anker.next = NULL;
    anker.prev = NULL;

    lineToTokens(&anker, begin, end);

    cmd_name_length = getCommandLength(&anker, &end_cmd_offset);

    if((cmd_name = malloc((cmd_name_length+1)*sizeof(wchar_t))) == NULL)
    {
        parser_errno = MEM_ALLOC_ERROR;
        return(-1);
    }

    getCommandName(&anker, cmd_name);

    if((offset = jumpToOffset(&anker, end_cmd_offset)) == NULL)
    {
        free(cmd_name);
        fprintf(logfile, "Offset to high\n");
        return(-3);
    }

    if((cmd_func = getCommandFunction(cmd_name)) == NULL)
    {
        if((macro = findMacro(cmd_name)) == NULL)
        {
            parser_errno = UNKOWN_FUNCTION;
            return(-2);
        }
        free(cmd_name);
        ret = exec_macro(offset, macro);
        return(ret);
    }

    ret = cmd_func(offset, stat);
    //fprintf(logfile, "Return code from the called handle: [%d]\n", ret);

    free(cmd_name);
    cleanUpTokenList(&anker);

    return(ret);
}
