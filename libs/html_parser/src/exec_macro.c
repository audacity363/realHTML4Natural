#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "command_parsing.h"
#include "macro.h"

#define DEBUG

int getArguments(token_t *start, macro_parms *parms);

int exec_macro(token_t *anker, macro_definition_t *macro)
{
    int i = 0, ret = 0;
    vars_t *macro_vars;

    macro_parms parms = {0, NULL, NULL, NULL};

    getArguments(anker, &parms);

    if(parms.parm_number > macro->parms->parm_number)
    {
        //To much arguments
        fprintf(stderr, "to much prameters\n");
        return(EXIT);
    }
    if(parms.parm_number < macro->parms->parm_number)
    {
        for(i = parms.parm_number; i < macro->parms->parm_number; i++)
        {
            //Check if the rest if the arguments are optional
            if(macro->parms->type[i] == -1)
            {
                //Found an argument with no default value
                fprintf(stderr, "Parameters does not match\n");
                return(EXIT);
            }
        }
    }

    //Everything is ok. Every default value gets overwritten when any other 
    //value is given. Even the type gets overwritten

    if((ret = initVarAnker(&macro_vars)) != 0)
        return(ret);

    addVariables(macro->parms, &parms, macro_vars);

#if 1
    printAllVars(macro_vars);
#endif

    /*for(; i < macro->sizeof_body; i++)
        printf("%S\n", macro->body[i]);

    printf("---End Marco---\n");*/
}


/*
 * TODO: Add parsing for index type (x,y,z) and append macro_parms with these
 */
int getArguments(token_t *start, macro_parms *parms)
{
    token_t *hptr = NULL;
    wchar_t name[500], index_buff[5];
    int type = 0, index = 0, i_index = 0;
    bool in_var = false, in_index = false;
    int index_type = 0,
        a_index[3] = {0, 0, 0};

    if(memcmp(&start->val, L"(", sizeof(wchar_t)) != 0)
    {
        fprintf(stderr, "Missing \"(\"\n");
        return(-1);
    }

    hptr = start->next;

    parms->type = malloc(sizeof(int));
    parms->val = malloc(sizeof(wchar_t*));

    while(hptr)
    {
        if(hptr->type == CLAMPS)
        {
            //save parm
            memset(name+(index++), 0x00, sizeof(wchar_t));
#ifdef DEBUG
            printf("parm: [%S]\n", name);
#endif
            saveParm(name, parms, index_type, a_index);
            index_type = i_index = 0;
            break;
        }
        else if(hptr->type == SPACE && in_var == true)
        {
            memcpy(name+(index++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == STR)
        {
            in_var = !in_var;
            memcpy(name+(index++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == CHAR && in_index == false)
        {
            memcpy(name+(index++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == CHAR && in_index == true)
        {
            memcpy(index_buff+(i_index++), &hptr->val, sizeof(wchar_t));
        }
        else if(hptr->type == INDEXCLOSE)
        {

            memset(index_buff+(i_index++), 0x00, sizeof(wchar_t));
#ifdef DEBUG
            printf("found INDEX: [%S]\n", index_buff);
#endif
            if(index_type+1 >=4)
            {
                //four dimensions are not supported
                return(EXIT);
            }
            a_index[index_type++] = wcstol(index_buff, NULL, 10);
            i_index = 0;
            in_index = false;

        }
        else if(hptr->type == INDEXOPEN)
        {
            in_index = true;
            i_index = 0;
        }
        else if(hptr->type == COMMA)
        {
            if(in_var == true)
            {
                fprintf(stderr, "missing '\"'\n");
                return(-2);
            }
            //save parm
            memset(name+(index++), 0x00, sizeof(wchar_t));
#ifdef DEBUG
            printf("parm: [%S]\n", name);
#endif
            saveParm(name, parms, index_type, a_index);
            index=0;
        }
        
        hptr = hptr->next;
    }

}

/*
 * TODO: free the parms after using them
 * TODO: add group handling
 * TODO: dynamic memory allocation for index and index_type
 */
int saveParm(wchar_t *arg, macro_parms *parms, int index_type, int index_array[3])
{
    int index = parms->parm_number;
    double d_buff = 0;
    char *c_name = NULL, *index_pos = NULL;


    //Look for a String
    if(memcmp(arg, L"\"", sizeof(wchar_t)) == 0)
    {
        parms->type[index] = STRING;
        parms->val[index] = malloc(wcslen(arg)*sizeof(wchar_t));
        wcsncpy((wchar_t*)parms->val[index], arg+1, wcslen(arg)-1);
    }
    //Look for a float
    else if(wcschr(arg, L'.') != NULL)
    {
        parms->type[index] = FLOAT;

        d_buff = wcstod(arg, NULL);
        parms->val[index] = malloc(sizeof(double));
        *((double*)parms->val[index]) = d_buff;
    }
    //Look for a true boolean
    else if(wcscmp(arg, L"true") == 0)
    {
        parms->type[index] = BOOL;
        parms->val[index] = malloc(sizeof(bool));
        *((bool*)parms->val[index]) = true;
    }
    //Look for a false boolean
    else if(wcscmp(arg, L"false") == 0)
    {
        parms->type[index] = BOOL;
        parms->val[index] = malloc(sizeof(bool));
        *((bool*)parms->val[index]) = false;
    }
    //must be an integer or an variable
    else
    {
        c_name = malloc(wcslen(arg)+1);
        wcstombs(c_name, arg, wcslen(arg)+1);
        if((index_pos = strchr(c_name, '[')) != NULL)
        {
            //Found an index bracked.
            index_pos[0]='\0';
#ifdef DEBUG
            printf("Index: (%s)\n", index_pos+1);
#endif
        }
        if(isDefinedBool(vars_anker, c_name) == false)
        {
            //Var ist not defined. It is an integer
            free(c_name);
            parms->type[index] = INTEGER;
        }
        else
        {
            parms->val[index] = malloc(strlen(c_name)+1);
            strcpy((char*)parms->val[index], c_name);
            //-1 means here that it is a variable
            parms->type[index] = -1;
            parms->index_type[index] = index_type;
            memcpy(parms->index[index], index_array, sizeof(index_array));
            /*parms->index[index][0] = index_array[0];
            parms->index[index][1] = index_array[1];
            parms->index[index][2] = index_array[2];*/
            free(c_name);
        }
    }

    index = (++parms->parm_number);
    //just so I have to write less
    index++;
    parms->val = realloc(parms->val, index*sizeof(void*));
    parms->type = realloc(parms->type, index*sizeof(int));
}

//creates the variables for the macro block.
//defaults == the default Parameters with the values wich was given at the 
//            macro declaration
//given    == the given arguments on the macro call
//vars     == the new anker for the variables
int addVariables(macro_parms *defaults, macro_parms *given, vars_t *vars)
{
    int i = 0, ret = 0;

    //first add all given vars
    for(; i < given->parm_number; i++)
    {
        //It is a static value. Create a variable with this the default name 
        //and teh given value
        if(given->type[i] != -1)
        {
            addVariableBasedOnType(vars, given->type[i], defaults->name[i], 
                                    given->val[i]);
        }
        //No index as been found copy hole variable to the new target with the
        //new name
        else if(given->type[i] == -1 && given->index_type[i] == 0)
        {
            if((ret = copyVariableNewName(vars_anker, vars, NULL,
                        (char*)given->val[i], NULL, defaults->name[i])) != 0)
           {
               fprintf(stderr, "Varerror: [%d]\n", ret);
               return(ret);
           }
        }
        else if(given->type[i] == -1 && given->index_type[i] != 0)
        {
            copyVariableNewNameWithIndex(vars_anker, vars,
                NULL, (char*)given->val[i], NULL, defaults->name[i],
                given->index_type[i], given->index[i]);
        }
    }
}

#undef DEBUG
