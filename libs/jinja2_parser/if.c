#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "jinja2_parser.h"
#include "jinja_utils.h"
#include "if.h"

bool ifhandler(struct variables *anker, char *cmd)
{
    struct args firstarg, secondarg;

    char *first, *vergleich, *second;
    char functionname[200];
    int i;
    bool function = false;

    first = strtok(cmd, " ");
    first = strtok(NULL, " ");
    vergleich = strtok(NULL, " ");
    second = strtok(NULL, " ");


    if(first[0] == '"')
    {
        firstarg.cargs = malloc(100);
        strncpy(firstarg.cargs, first+sizeof(char), strlen(first)-2);
        firstarg.type = STRING;
    }
    else if(checkInt(first) == true)
    {
        firstarg.iargs = atoi(first);
        firstarg.type = INT;
    }
    else
    {
        function = hasFunction(first);
        if(function == true)
        {
            getFunction(first, functionname);
//            printf("Functionname: [%s]\n", functionname);
        }

        if((firstarg.type = getVarType(anker, first)) < 0)
        {
            strcpy(error_str, varhandle_error_str);
            return(-1);
        }
        switch(firstarg.type)
        {
            case STRING:
                firstarg.cargs = getStringValue(anker, first);
                break;
            case INT:
                firstarg.iargs = getIntValue(anker, first);
                break;
            case STRINGARRAY:
            default:
                sprintf(error_str, "Not Supported (%s)\n", first); 
                return(-1);
        }

        if(function == true)
        {
            callFunction(&firstarg, functionname);
        }
    } 

    if(second[0] == '"')
    {
        secondarg.cargs = malloc(100);
        strncpy(secondarg.cargs, second+sizeof(char), strlen(second)-2);
        secondarg.type = STRING;
    }
    else if(checkInt(second) == true)
    {
        secondarg.iargs = atoi(second);
        secondarg.type = INT;
    }
    else
    {
        secondarg.type = getVarType(anker, second);
        switch(firstarg.type)
        {
            case STRING:
                secondarg.cargs = getStringValue(anker, second);
                break;
            case INT:
                secondarg.iargs = getIntValue(anker, second);
                break;
            case STRINGARRAY:
            default:
                sprintf(error_str, "Not Supported (%s)\n", second); 
                return(-1);
        }
    }

    if((firstarg.type == STRING && secondarg.type == INT) ||
       (firstarg.type == INT && secondarg.type == STRING))
    {
        sprintf(error_str, "Type Error");
        return(-1);
    }

    if(cmp(vergleich, "=="))
    {
        if(firstarg.type == INT)
        {
            if(firstarg.iargs == secondarg.iargs)
            {
                return(true);
            }
            else
            {
                return(false);
            }
        }
        else
        {
            if(cmp(firstarg.cargs, secondarg.cargs))
            {
                return(true);
            }
            else
            {
                return(false);
            }
        }
    }
    else if(cmp(vergleich, "<"))
    {
        if(firstarg.type == STRING)
        {
            strcpy(error_str, "Syntax Error");
            return(-1);
        }

        if(firstarg.iargs < secondarg.iargs)
        {
            return(true);
        }
        else
        {
            return(false);
        }

    }
    else if(cmp(vergleich, "<="))
    {
        if(firstarg.type == STRING)
        {
            strcpy(error_str, "Syntax Error");
            return(-1);
        }

        if(firstarg.iargs <= secondarg.iargs)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if(cmp(vergleich, ">="))
    {
        if(firstarg.type == STRING)
        {
            strcpy(error_str, "Syntax Error");
            return(-1);
        }

        if(firstarg.iargs >= secondarg.iargs)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if(cmp(vergleich, ">"))
    {
        if(firstarg.type == STRING)
        {
            strcpy(error_str, "Syntax Error");
            return(-1);
        }

        if(firstarg.iargs > secondarg.iargs)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }
    else if(cmp(vergleich, "in"))
    {
        struct variables *ptr;
        ptr = anker;
        while(ptr != NULL)
        {
            if(cmp(ptr->name, second))
                break;
            ptr = ptr->next;
        }
        if(ptr == NULL)
        {
            sprintf(error_str, "Unkown Variable [%s]\n", second);
            return(-1);
        }

        for(i=0; i<ptr->length; i++)
        {
            if(cmp(*(char**)(ptr->data+sizeof(char*)*i), firstarg.cargs))
                return(true);
        }
        return(false);
    }
    else
    {
        strcpy(error_str, "Syntax Error");
        return(-1);
    }
}
