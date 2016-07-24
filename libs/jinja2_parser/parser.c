#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "jinja2_parser.h"
#include "jinja_utils.h"
#include "if.h"
//#include "for.h"
//

int getIndex(char *word)
{
    char index[10];
    int i;
    int x = 0;
    bool found_open = false;
    bool glob_found = false;

    for(i=0; i < strlen(word);i++)
    {
        if(word[i] == '[')
        {
            found_open = true;
            glob_found = true;
            continue;
        }
        if(found_open == false && word[i] == ']')
        {
            strcpy(error_str, "Syntax Error");
            return(-1);
        }
        if(found_open == true && word[i] == ']')
        {
            word[i-(x+1)] = '\0';
            found_open = false;
        }

        if(found_open == true)
        {
            index[x] = word[i];
            x++;
        }
    }

    index[x] = '\0';
    if(glob_found == false)
    {
        strcpy(error_str, "Syntax Error");
        return(-1);
    }
    return(atoi(index));
}

int getIndex2D(char *word, int *x, int *y)
{
    char x_str[10];
    char y_str[10];
    int i;
    int index = 0;
    int length = strlen(word);
    bool found_open = false;
    bool glob_found = false;

    x_str[0] = '\0';
    y_str[0] = '\0';

    for(i=0; i < length;i++)
    {
        if(word[i] == '[')
        {
            found_open = true;
            glob_found = true;
            continue;
        }
        if(found_open == false && word[i] == ']')
        {
            strcpy(error_str, "Syntax Error");
            return(-1);
        }
        if(found_open == true && word[i] == ']')
        {
//            word[i-(index+1)] = '\0';
            found_open = false;
            break;
        }

        if(found_open == true)
        {
            x_str[index] = word[i];
            index++;
        }
    }

    if(glob_found == false || strlen(x_str) == 0)
    {
        strcpy(error_str, "Syntax Error");
        return(-2);
    }

    x_str[index] = '\0';
    *x = atoi(x_str);

    i++;
    if(word[i] != '[')
    {
        *y = -1;
        return(0);
    }
    i++;

    index = 0;
    while(i<length)
    {
        if(word[i] == ']')
            break;
        y_str[index] = word[i];
        index++;
        i++;
    }

    y_str[index] = '\0';
    if(strlen(y_str) == 0)
        return(-3);


    *y = atoi(y_str);
    return(0);
}

int handleArray(struct variables *anker, parser_info *status,
                    bool function, char *functionname)
{
    struct args arguments;
    int x_length, y_length;
    int type;

    if((type = getVarType(anker, status->statement)) < 0)
    {
        strcpy(error_str, varhandle_error_str);
        return(-1);
    }
    switch(type)
    {
        case TWO_DSTRINGARRAY:
            printf("index 2D: [%d]\n", status->index);
            break;
        case STRINGARRAY:
            if(status->index >= getArrayLength(anker, 
                                    status->statement, &x_length, &y_length))
            {
                fprintf(stderr, "Out of range\n");
                sprintf(error_str, "[%s] [%d] Out of Range", status->statement,
                        status->index);
                return(-1);
            }

            arguments.type = STRING;

            if(function == true)
            {
                arguments.cargs = getStringValuefromArray(anker, 
                        status->statement, status->index);
                callFunction(&arguments, anker, functionname, STRING);
                //printVar(arguments);
            }
            arguments.cargs = getStringValuefromArray(anker, status->statement, status->index);
            printf("%s", arguments.cargs);
            break;
    }

    status->index = -1;
    return(0);
}

void getAttributes(char *word, char *attr)
{
    int i, x = 0;
    bool found = false;
    int length = strlen(word);

    for(i=0; i < length; i++)
    {
        if(found == true)
        {
            attr[x] = word[i];
            x++;
        }

        if(word[i] == '[' && found == false)
        {
            attr[x] = word[i];
            word[i] = '\0';
            x++;
            found = true;
        }
    }
}

int local_jinja_parser(parser_info *status, struct variables *anker, char *line, int length)
{
    int i, type, cmdindex = 0;
    int anzahl_for = 0;
    int x_index, y_index, intvalue;
    bool function;
    char functionname[100];
    struct args arguments;
    struct variables *tmparray;

//    status->forbuffer[0] = '\0';

    for(i=0; i < length; i++)
    {
        if(varopen(line, i) && status->in_for == false)
        {
            status->found_var_open = true;
            i = i+2;
        }
        else if(varclose(line, i) && status->in_for == false)
        {
            status->statement[i] = '\0';
            RemoveSpaces(status->statement);
            if((function = hasFunction(status->statement)) == true) 
            {
                getFunction(status->statement, functionname);
            }

            getAttributes(status->statement, status->attrbuf);

            switch((type = getVarType(anker, status->statement)))
            {
                case -1:
                    strcpy(error_str, varhandle_error_str);
                    return(-1);
                case STRING:
                    arguments.cargs = malloc(strlen(status->statement));
                    strcpy(arguments.cargs, status->statement);
                    arguments.type = STRING;
                    if(function == true)
                    {
                        arguments.cargs = getStringValue(anker, arguments.cargs);
                        arguments.type = type;
                        callFunction(&arguments, anker, functionname, type);
                        //printVar(arguments);
                    }
                    else
                    {
                        printf("%s", getStringValue(anker, status->statement));
                    }
                    break;
                case INT:
                    printf("%d", getIntValue(anker, status->statement));
                    break;
                case STRINGARRAY:
                    status->index = getIndex(status->attrbuf);
                    if(function == true)
                    {
                        arguments.cargs = malloc(strlen(status->statement));
                        strcpy(arguments.cargs, status->statement);
                        arguments.type = type;
                        callFunction(&arguments, anker, functionname, arguments.type);
                        //printVar(arguments);
                    }
                    else if(status->index > -1)
                    {
                        if(handleArray(anker, status, function, functionname) < 0)
                            return(-1);
                        break;
                    }
                    else
                        printArray(anker, status->statement, false, NULL);
                    break;
                case INTARRAY:
                    status->index = getIndex(status->attrbuf);
                    printArray(anker, status->statement, false, NULL);
                    break;
                case TWO_DSTRINGARRAY:
                    if(getIndex2D(status->attrbuf, &x_index, &y_index) == 0)
                    {
                        if(y_index > -1)
                        {
                            //just One Value
                            printf("%s", getStringValuefrom2DArray(anker,
                                        status->statement, x_index, y_index));
                            break;
                        }
                        //Ein Array aus dem x Index erzeugen
                        tmparray = createTmpArrayOut2DArray(anker,
                                                status->statement, x_index);
                        printArrayfromPtr(tmparray);
//                        free(tmparray->data);
//                        free(tmparray);
                        break;
                    }
                    else
                    {
                        return(-1);
                    }
                    if(status->index > -1)
                    {
                        if(handleArray(anker, status, function, functionname) < 0)
                            return(-1);
                    }
                    print2DArray(anker, status->statement, false, NULL);
                    break;
                case TWO_DINTARRAY:
                    if(getIndex2D(status->attrbuf, &x_index, &y_index) == 0)
                    {
                        if(y_index > -1)
                        {
                            if(getIntValuefrom2DArray(anker, status->statement,
                                        x_index, y_index, &intvalue) < 0)
                            {
                                sprintf(error_str, "Unkown Variable %s",
                                    status->statement);
                                break;
                            }
                            printf("%d", intvalue);
                            break;
                        }
                        tmparray = createTmpArrayOut2DArray(anker, status->statement, x_index);
                        printArrayfromPtr(tmparray);
//                        free(tmparray->data);
//                        free(tmparray);
                        break;
                    }
                    else
                    {
                        return(-1);
                    }
                    print2DArray(anker, status->statement, false, NULL);
                    break;
                deafult:
                    strcpy(error_str, "TypeError: Unkown Variable type");
                    return(-1);
            }
            status->found_var_open = false;
            i = i+2;
            bzero(status->statement, sizeof(status->statement));
            cmdindex = 0;
        }
        else if(cmdopen(line, i))
        {
            i = i+2;
            status->found_cmd_open = true;
        }
        else if(cmdclose(line, i))
        {
            i=i+2;
            status->found_cmd_open = false;

            if(strncmp(status->statement, IF, sizeof(IF)-1) == 0)
            {
                if((status->if_ret = ifhandler(anker, status->statement)) < 0)
                {
                    return(-1);
                }
                status->in_if = true;
            }
            else if(strncmp(status->statement, ENDIF, sizeof(ENDIF)-1) == 0)
            {
                status->in_if = false;
            }
            else if(strncmp(status->statement, IMPORT, sizeof(IMPORT)-1) == 0)
            {
                if(importHandler(anker, status->statement) < 0)
                {
                    return(-1);
                }
            }
            else if(ncmp(status->statement, FOR))
            {
                status->in_for = true;
                status->count_for++;
                break;
            }
            else if(ncmp(status->statement, ENDFOR))
           {
               if(status->count_for != 1)
                   status->count_for--;
               else
               {
                   status->count_for = 0;
                   status->in_for = false;
                   status->for_length = 0;
                   forhandle(anker, status->forbuffer);
                   //free(forbuffer);
                   status->forbuffer = malloc(1);
                   break;
               }
            }
            else if(ncmp(status->statement, PRINTVARS))
            {
                printVars(anker);
            }
            else
            {
                sprintf(error_str, "Unknown Command [%s]", status->statement);
                return(-1);
            }
        }

        if(status->found_var_open == true || status->found_cmd_open == true)
        {
            status->statement[cmdindex] = line[i];
            cmdindex++;
        }
        else if(status->in_if && status->if_ret == false)
        {
            continue;
        }
        else if(status->in_for == false)
        {
            printf("%c", line[i]);
        }
    }

    if(status->in_for == true)
    {
        status->for_length = status->for_length+length;
        status->forbuffer = realloc(status->forbuffer, status->for_length);
        strcat(status->forbuffer, line);
    }
    bzero(status->statement, sizeof(status->statement));
    return(0);
}
