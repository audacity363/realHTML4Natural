#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "varhandle.h"
#include "hexdump.h"

int main()
{
    struct variables anker;

    setlocale(LC_ALL, "");

    anker.next = NULL;

    newUStringVar(&anker, "test", L"hellö", 10);

    wchar_t *rc = getUStringVal(&anker, "test");
    if(rc == NULL)
    {
        printf("%s\n", varhandle_error_str);
    }
    else
    {
        printf("value: [%S]\n", rc);
    }

    editUStringVar(&anker, "test", L"lölölöl");

    rc = getUStringVal(&anker, "test");
    if(rc == NULL)
    {
        printf("%s\n", varhandle_error_str);
    }
    else
    {
        printf("value: [%S]\n", rc);
    }




    int i, x, z;
    wchar_t hvar[10];

    newUStringArray(&anker, "testarray", 10, 10);

    for(i=0; i < 10; i++)
    {
        swprintf(hvar, 10, L"hööö%d", i);

        if(editUStringArray(&anker, "testarray", hvar, i) < 0)
        {
            printf("%s\n", varhandle_error_str);
            exit(0);
        }
    }

    /*for(i=0; i < 10; i++)
    {
        rc = getUStringArrayVal(&anker, "testarray", i);
        if(rc == NULL)
        {
            printf("%s\n", varhandle_error_str);
        }
        else
        {
            printf("value: [%S]\n", rc);
            logHexDump(rc, (wcslen(rc)+1)*sizeof(wchar_t), stdout);
        }
    }*/

    newU2DStringArray(&anker, "test2d", 10, 5, 6);

    for(i=0; i < 5; i++)
    {
        for(x=0; x < 6; x++)
        {
            swprintf(hvar, 10, L"ööö%d %d", i, x);
            editU2DStringArray(&anker, "test2d", hvar, i, x);
        }
    }
    printVars(&anker);
    //printf("str: [%S]\n", generateStringValuefromArray(&anker, "testarray"));
    //printf("str: [%S]\n", generateStringValuefromArray(&anker, "test2d"));

    struct variables *test = createTmpArrayOut2DArray(&anker, "test2d", 1);
    logHexDump(test->data, (test->length*sizeof(wchar_t))*test->x_length, stdout);
    printArrayfromPtrtoFile(test, stdout);
    printf("\n");

    exit(0);

    for(i=0; i < 5; i++)
    {
        for(x=0; x < 6; x++)
        {
            rc = getUString2DArrayVal(&anker, "test2d", i, x);
            if(rc == NULL)
            {
                printf("%s\n", varhandle_error_str);
            }
            else
            {
                printf("value: [%S]\n", rc);
                //logHexDump(rc, 40, stdout);
            }
        }
    }

    newU3DStringArray(&anker, "test3d", 10, 5, 6, 5);

    for(i=0; i < 5; i++)
    {
        for(x=0; x < 6; x++)
        {
            for(z=0; z < 5; z++)
            {
                swprintf(hvar, 10, L"öö%d %d %d", i, x, z);
                editU3DStringArray(&anker, "test3d", hvar, i, x, z);
            }
        }
    }

    for(i=0; i < 5; i++)
    {
        for(x=0; x < 6; x++)
        {
            for(z=0; z < 5; z++)
            {
                rc = getUString3DArrayVal(&anker, "test3d", i, x, z);
                if(rc == NULL)
                {
                    printf("%s\n", varhandle_error_str);
                }
                else
                {
                    printf("value: [%S]\n", rc);
                    //logHexDump(rc, 40, stdout);
                }
            }
        }
    }

}
