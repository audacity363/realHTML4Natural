#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "standard.h"
#include "varhandle.h"

char varhandle_error_str[2048];

void printArrayfromPtrtoFile(struct variables *ptr, FILE *output)
{
    int x;

    fprintf(output, "[");
    if(ptr->type == U_STRINGARRAY)
    {
        for(x=0; x < ptr->x_length; x++)
        {
            fprintf(output, "\"%S\"", (wchar_t*)(ptr->data+((ptr->length)*sizeof(wchar_t)*x)));
            if(x+1 < ptr->x_length)
                fprintf(output, ",");

        }
    }
    else
    {
        for(x=0; x < ptr->length; x++)
        {
            if(ptr->type == STRINGARRAY)
                fprintf(output, "\"%s\"", ((char**)ptr->data)[x]);
            else
                fprintf(output, "%d", ((int*)ptr->data)[x]);
            if(x+1 < ptr->length)
                fprintf(output, ",");
        }
    }
    fprintf(output, "]");
}

void printArrayfromPtr(struct variables *ptr)
{
    int x;

    printf("[");

    if(ptr->type == U_STRINGARRAY)
    {
        for(x=0; x < ptr->x_length; x++)
        {
            printf("\"%S\"", (wchar_t*)(ptr->data+((ptr->length)*sizeof(wchar_t)*x)));
            if(x+1 < ptr->x_length)
                printf(",");
        }
    }
    else
    {
        for(x=0; x < ptr->length; x++)
        {
            if(ptr->type == STRINGARRAY)
                printf("\"%s\"", ((char**)ptr->data)[x]);
            else if(ptr->type == INTARRAY)
                printf("%d", ((int*)ptr->data)[x]);
            if(x+1 < ptr->length)
                printf(",");
        }
    }
    printf("]");
}

//TODO: returnstring auf wchar_t umstellen
void printArray(struct variables *anker, char *name, bool showname, 
                    char *returnstring)
{
    struct variables *ptr;
    int i, tmp;
    char buf[1000];

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name) && ptr->type == STRINGARRAY)
        {
            if(showname == true)
                printf("Name: [%s] = ", name);

            if(returnstring == NULL)
                printf("[");
            else
            {
                sprintf(returnstring, "[");
            }

            for(i=0; i < ptr->length; i++)
            {
                if(returnstring == NULL)
                    printf("\"%s\"", ((char**)ptr->data)[i]);
                else
                {
                    sprintf(buf, "\"%s\"", ((char**)ptr->data)[i]);
                    strcat(returnstring, buf);
                }
                if(i+1 < ptr->length)
                {
                    if(returnstring == NULL)
                        printf(",");
                    else
                    {
                        strcat(returnstring, ",");
                    }
                }
            }

            if(returnstring == NULL)
                printf("]");
            else
            {
                strcat(returnstring, "]");
            }
            if(showname == true)
                printf("\n");

        }
        else if(cmp(ptr->name, name) && ptr->type == U_STRINGARRAY)
        {
            if(showname == true)
                printf("Name: [%s] = ", name);

            if(returnstring == NULL)
                printf("[");
            else
            {
                sprintf(returnstring, "[");
            }

            for(i=0; i < ptr->x_length; i++)
            {
                if(returnstring == NULL)
                    printf("\"%S\"", getUStringArrayVal(anker, name, i));
                else
                {
                    sprintf(buf, "\"%s\"", ((char**)ptr->data)[i]);
                    strcat(returnstring, buf);
                }
                if(i+1 < ptr->x_length)
                {
                    if(returnstring == NULL)
                        printf(",");
                    else
                    {
                        strcat(returnstring, ",");
                    }
                }
            }

            if(returnstring == NULL)
                printf("]");
            else
            {
                strcat(returnstring, "]");
            }
            if(showname == true)
                printf("\n");

        }
        else if(cmp(ptr->name, name) && ptr->type == INTARRAY)
        {
            if(showname == true)
                printf("Name: [%s] = ", name);

            printf("[");
            for(i=0; i < ptr->length; i++)
            {
                memcpy(&tmp, ptr->data+(sizeof(int)*i), sizeof(int));
                /*printf("%.4d", *(int*)ptr->data+(sizeof(int)*i));*/
                printf("%d", tmp);
                if(i+1 < ptr->length)
                {
                    printf(",");
                }
            }
            printf("]");
            if(showname == true)
                printf("\n");

        }
        ptr = ptr->next;
    }
}


wchar_t *generateStringValuefromArray(struct variables *anker, char *name)
{

    struct variables *hptr;
    wchar_t *returnstr;
    wchar_t buf[1024];
    int i, x;

    hptr = anker;

    while(hptr != NULL || strcmp(hptr->name, name) != 0)
    {
        if(strcmp(hptr->name, name) == 0)
            break;
        hptr = hptr->next;
    }

    if(hptr == NULL)
    {
        sprintf(varhandle_error_str, "Unkown Variable [%s]", name);
        return(NULL);
    }
   
    if(hptr->type == STRINGARRAY)
    {
        returnstr = malloc(2*sizeof(wchar_t));
        swprintf(returnstr, 2, L"[");

        for(i=0; i < hptr->length; i++)
        {
            swprintf(buf, sizeof(buf), L"\"%s\"", ((char**)hptr->data)[i]);
            returnstr = realloc(returnstr,
                        (wcslen(returnstr)+wcslen(buf)+1)*sizeof(wchar_t));
            wcscat(returnstr, buf);
            if(i+1 < hptr->length)
            {
                returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
                wcscat(returnstr, L",");
            }
        }
        returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
        wcscat(returnstr, L"]");
    }
    else if(hptr->type == U_STRINGARRAY)
    {
        returnstr = malloc((hptr->length*hptr->x_length+hptr->x_length*3+2)*sizeof(wchar_t));
        swprintf(returnstr, 2, L"[");
        for(i=0; i < hptr->x_length; i++)
        {
            swprintf(buf, sizeof(buf), L"\"%S\"", getUStringArrayVal(anker, name, i));
            wcscat(returnstr, buf);
            if(i+1 < hptr->x_length)
            {
                wcscat(returnstr, L",");
            }
        }
        wcscat(returnstr, L"]");
    }
    else if(hptr->type == INTARRAY)
    {
        returnstr = malloc(2*sizeof(wchar_t));
        swprintf(returnstr, 2, L"[");

        for(i=0; i < hptr->length; i++)
        {
            swprintf(buf, sizeof(buf), L"%d", ((int*)hptr->data)[i]);
            returnstr = realloc(returnstr,
                        (wcslen(returnstr)+wcslen(buf)+1)*sizeof(wchar_t));
            wcscat(returnstr, buf);
            if(i+1 < hptr->length)
            {
                returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
                wcscat(returnstr, L",");
            }
        }
        returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
        wcscat(returnstr, L"]");
    }
    else if(hptr->type == TWO_DSTRINGARRAY)
    {
        returnstr = malloc(3*sizeof(wchar_t));
        swprintf(returnstr, 2, L"[");

        for(i=0; i < hptr->x_length; i++)
        {
            wcscat(returnstr, L"[");

            for(x=0; x < hptr->y_length; x++)
            {
                swprintf(buf, sizeof(buf), L"\"%s\"", ((char**)hptr->data)[hptr->x_length*i+x]);
                returnstr = realloc(returnstr,
                            (wcslen(returnstr)+wcslen(buf)+1)*sizeof(wchar_t));
                wcscat(returnstr, buf);
                if(x+1 < hptr->y_length)
                {
                    returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
                    wcscat(returnstr, L",");
                }
            }
            returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
            wcscat(returnstr, L"]");
            if(i+1 < hptr->x_length)
            {
                returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
                wcscat(returnstr, L",");
            }
        }
        returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
        wcscat(returnstr, L"]");
    }
    else if(hptr->type == U_TWO_DSTRINGARRAY)
    {
        returnstr = malloc(((hptr->length*hptr->x_length+hptr->x_length*3+2)*hptr->y_length+hptr->y_length*3+2)*sizeof(wchar_t));
        swprintf(returnstr, 2, L"[");
        for(i=0; i < hptr->x_length; i++)
        {
            wcscat(returnstr, L"[");
            for(x = 0; x < hptr->y_length; x++)
            {

                swprintf(buf, sizeof(buf), L"\"%S\"", getUString2DArrayVal(anker, name, i, x));
                wcscat(returnstr, buf);
                if(x+1 < hptr->y_length)
                {
                    wcscat(returnstr, L",");
                }   
            }
            wcscat(returnstr, L"]");
            if(i+1 < hptr->x_length)
            {
                wcscat(returnstr, L",");
            }
        }
        wcscat(returnstr, L"]");
    }
    else if(hptr->type == TWO_DINTARRAY)
    {
        returnstr = malloc(3*sizeof(wchar_t));
        swprintf(returnstr, 2, L"[");

        for(i=0; i < hptr->x_length; i++)
        {
            wcscat(returnstr, L"[");

            for(x=0; x < hptr->y_length; x++)
            {
                swprintf(buf, sizeof(buf), L"%d", ((int*)hptr->data)[hptr->x_length*i+x]);
                returnstr = realloc(returnstr,
                            (wcslen(returnstr)+wcslen(buf)+1)*sizeof(wchar_t));
                wcscat(returnstr, buf);
                if(x+1 < hptr->y_length)
                {
                    returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
                    wcscat(returnstr, L",");
                }
            }
            returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
            wcscat(returnstr, L"]");
            if(i+1 < hptr->x_length)
            {
                returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
                wcscat(returnstr, L",");
            }
        }
        returnstr = realloc(returnstr, (wcslen(returnstr)+2)*sizeof(wchar_t));
        wcscat(returnstr, L"]");
    }
    return(returnstr);
}


struct variables* getVarPointer(struct variables *anker, char* name)
{
    struct variables *ptr;

    ptr = anker;
    while(ptr != NULL)
    {
        if(cmp(ptr->name, name))
            return(ptr);
        ptr = ptr->next;
    }
    return(NULL);
}

int print2DArray(struct variables *anker, char *name, bool rt,
                    char *returnstring)
{
    int x, y;
    struct variables *ptr;
    char buf[1000];

    if((ptr = getVarPointer(anker, name)) == NULL)
    {
        fprintf(stderr, "Var not found\n");
        sprintf(varhandle_error_str, "Unkown variable [%s]", name);
        return(-1);
    }

    if(rt == true)
        printf("Name: [%s] = ", ptr->name);

    printf("%s[%s", rt==true?"\n":"", rt==true?"\n":"");
    for(x=0; x < ptr->x_length; x++)
    {  
        if(returnstring == NULL)
            printf("%s[", rt==true?"\t":"");
        else
            sprintf("%s[", rt==true?"\t":"");

        for(y=0; y < ptr->y_length; y++)
        {
            if(ptr->type == TWO_DSTRINGARRAY)
            {
                if(returnstring == NULL)
                    printf("\"%s\"", ((char**)ptr->data)[ptr->x_length*x+y]);
                else
                {
                    sprintf(buf, "\"%s\"",
                        ((char**)ptr->data)[ptr->x_length*x+y]);
                    strcat(returnstring, buf);
                }
            }
            else if(ptr->type == U_TWO_DSTRINGARRAY)
            {
                if(returnstring == NULL)
                    printf("\"%S\"", getUString2DArrayVal(anker, name, x, y));
                else
                {
                    sprintf(buf, "\"%s\"",
                        ((char**)ptr->data)[ptr->x_length*x+y]);
                    strcat(returnstring, buf);
                }
            }
            else if(ptr->type == TWO_DINTARRAY)
            {

                if(returnstring == NULL)
                    printf("%d", ((int*)ptr->data)[ptr->x_length*x+y]);
                 else
                 {
                    sprintf(buf, "%d", ((int*)ptr->data)[ptr->x_length*x+y]);
                    strcat(returnstring, buf);
                 }
            }

            if(y+1 < ptr->y_length)
            {
                if(returnstring == NULL)
                    printf(", ");
                else
                    strcat(returnstring, ", ");
            }
        }
        if(returnstring == NULL)
            printf("],%s", rt==true?"\n":"");
        else
            strcat(returnstring, "],");
    }
    if(returnstring == NULL)
        printf("]%s",rt==true?"\n":"");
    else
        strcat(returnstring, "]");
}


int getVarType(struct variables *anker, char *name)
{
    struct variables *ptr;

    if((ptr = searchVar(anker, name)) == NULL)
    {
        sprintf(varhandle_error_str, "Unknown Variable: [%s]", name);
        return(-1);
    }
    return(ptr->type);
}

void printVars(struct variables *anker)
{
    struct variables *ptr;

    printf("--------------------------------------------------------------\n");

    ptr = anker;
    while(ptr != NULL)
    {
        switch(ptr->type)
        {
            case STRING:
                printf("Name:[%s] = [%s]\n", ptr->name, (char*)ptr->data);
                break;
            case U_STRING:
                printf("Name:[%s] = [%S]\n", ptr->name, (wchar_t*)ptr->data);
                break;
            case INT:
                printf("Name:[%s] = [%d]\n", ptr->name, *(int*)ptr->data);
                break;
            case STRINGARRAY:
                printArray(anker, ptr->name, true, NULL);
                break;
            case U_STRINGARRAY:
                printArray(anker, ptr->name, true, NULL);
                break;
            case INTARRAY:
                printArray(anker, ptr->name, true, NULL);
                break;
            case TWO_DSTRINGARRAY:
                print2DArray(anker, ptr->name, true, NULL);
                break;
            case U_TWO_DSTRINGARRAY:
                print2DArray(anker, ptr->name, true, NULL);
                break;
            case TWO_DINTARRAY:
                print2DArray(anker, ptr->name, true, NULL);
                break;


        }
        ptr = ptr->next;
    }
    printf("--------------------------------------------------------------\n");
    return;
}

void printVar(struct variables *anker, char *name)
{
    struct variables *hptr;

    hptr = anker;
    while(hptr != NULL)
    {
        if(strcmp(name, hptr->name) == 0)
        {
            break;
        }
        hptr = hptr->next;
    }
    if(hptr == NULL)
        return;

    switch(hptr->type)
        {
            case STRING:
                printf("Name:[%s] = [%s]\n", hptr->name, (char*)hptr->data);
                break;
            case INT:
                printf("Name:[%s] = [%d]\n", hptr->name, *(int*)hptr->data);
                break;
            case STRINGARRAY:
                printArray(anker, hptr->name, true, NULL);
                break;
            case INTARRAY:
                printArray(anker, hptr->name, true, NULL);
                break;
            case TWO_DSTRINGARRAY:
                print2DArray(anker, hptr->name, true, NULL);
                break;
            case U_TWO_DSTRINGARRAY:
                print2DArray(anker, hptr->name, true, NULL);
                break;
            case TWO_DINTARRAY:
                print2DArray(anker, hptr->name, true, NULL);
                break;


        }
}

void printArraytoFile(struct variables *anker, char *name, FILE *p_output)
{
    struct variables *ptr;
    int i, tmp;
    char buf[1000];

    ptr = anker;

    while(ptr != NULL)
    {
        if(cmp(ptr->name, name) && ptr->type == STRINGARRAY)
        {
            fprintf(p_output, "Name: [%s] = ", name);
            fprintf(p_output, "[");

            for(i=0; i < ptr->length; i++)
            {
                fprintf(p_output, "\"%s\"", ((char**)ptr->data)[i]);
                if(i+1 < ptr->length)
                {
                    fprintf(p_output, ",");
                }
            }

            fprintf(p_output, "]");
            fprintf(p_output, "\n");
        }
        else if(cmp(ptr->name, name) && ptr->type == U_STRINGARRAY)
        {
            fprintf(p_output, "Name: [%s] = ", name);
            fprintf(p_output, "[");

            for(i=0; i < ptr->x_length; i++)
            {
                fprintf(p_output, "\"%S\"", getUStringArrayVal(anker, name, i));
                if(i+1 < ptr->x_length)
                {
                    fprintf(p_output, ",");
                }
            }

            fprintf(p_output, "]");
            fprintf(p_output, "\n");
        }
        else if(cmp(ptr->name, name) && ptr->type == INTARRAY)
        {
            fprintf(p_output, "Name: [%s] = ", name);

            fprintf(p_output, "[");
            for(i=0; i < ptr->length; i++)
            {
                memcpy(&tmp, ptr->data+(sizeof(int)*i), sizeof(int));
                /*printf("%.4d", *(int*)ptr->data+(sizeof(int)*i));*/
                fprintf(p_output, "%d", tmp);
                if(i+1 < ptr->length)
                {
                    fprintf(p_output, ",");
                }
            }
            fprintf(p_output, "]");
            fprintf(p_output, "\n");

        }
        ptr = ptr->next;
    }
}

int print2DArraytoFile(struct variables *anker, char *name, FILE *p_output)
{
    int x, y;
    struct variables *ptr;
    char buf[1000];

    if((ptr = getVarPointer(anker, name)) == NULL)
    {
        fprintf(stderr, "Var not found\n");
        sprintf(varhandle_error_str, "Unkown variable [%s]", name);
        return(-1);
    }

    fprintf(p_output, "Name: [%s] = ", ptr->name);

    fprintf(p_output, "\n[\n");
    for(x=0; x < ptr->x_length; x++)
    {  
        fprintf(p_output, "\t[");

        for(y=0; y < ptr->y_length; y++)
        {
            if(ptr->type == TWO_DSTRINGARRAY)
            {
                fprintf(p_output, "\"%s\"", ((char**)ptr->data)[ptr->x_length*x+y]);
            }
            else if(ptr->type == U_TWO_DSTRINGARRAY)
            {
                fprintf(p_output, "\"%S\"", getUString2DArrayVal(anker, name, x, y));
            }
            else if(ptr->type == TWO_DINTARRAY)
            {
                fprintf(p_output, "%d", ((int*)ptr->data)[ptr->x_length*x+y]);
            }

            if(y+1 < ptr->y_length)
            {
                fprintf(p_output, ", ");
            }
        }
        fprintf(p_output, "]");
        if(x+1 < ptr->x_length)
            fprintf(p_output, ",");
        fprintf(p_output, "\n");

    }
    fprintf(p_output, "]\n");
}


void printVarstoFile(struct variables *anker, FILE *p_output)
{
    struct variables *ptr;

    ptr = anker;
    while(ptr != NULL)
    {
        switch(ptr->type)
        {
            case STRING:
                fprintf(p_output, "Name:[%s] = [%s]\n", ptr->name, (char*)ptr->data);
                break;
            case U_STRING:
                fprintf(p_output, "Name:[%s] = [%S]\n", ptr->name, (wchar_t*)ptr->data);
                break;
            case INT:
                fprintf(p_output, "Name:[%s] = [%d]\n", ptr->name, *(int*)ptr->data);
                break;
            case U_STRINGARRAY:
            case STRINGARRAY:
                printArraytoFile(anker, ptr->name, p_output);
                break;
            case INTARRAY:
                printArraytoFile(anker, ptr->name, p_output);
                break;
            case U_TWO_DSTRINGARRAY:
            case TWO_DSTRINGARRAY:
                print2DArraytoFile(anker, ptr->name, p_output);
                break;
            case TWO_DINTARRAY:
                print2DArraytoFile(anker, ptr->name, p_output);
                break;
        }
        ptr = ptr->next;
    }
    return;
}
