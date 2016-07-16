#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/standard.h"
#include "../parser.h"

int main()
{
    int test = 15;
    int i, x, y;
    int stringlength = 100;
    int arraylength = 4;
    struct variables *anker;

    int x_size = 5;
    int y_size = 5;

    int array_size = x_size * y_size;
    i=0;

    anker = malloc(sizeof(struct variables));


    anker->data = malloc(sizeof(char*)*array_size);
    
    for(x=0; x < 5; x++)
    {
        for(y=0; y < y_size; y++)
        {
            ((char**)anker->data)[x_size*x+y] = malloc(50*sizeof(char));
            sprintf(((char**)anker->data)[x_size*x+y], "Hello World [%d]", i);
            i++;
        }
    }

    for(x=0; x < 5; x++)
    {
        for(y=0; y < y_size; y++)
        {
            printf("%s\n", ((char**)anker->data)[x_size*x+y]);
        }
    }




//-----------------------------------------------------------------------------    
//  Ein 2D Int Array
    /*anker->data = malloc(sizeof(int)*array_size);

    for(x=0; x < 5; x++)
    {
        for(y=0; y < 5; y++)
        {
            ((int*)anker->data)[x_size*x+y] = i;
            *                    x_size
             * * -------------------------------------->
             * | +--+--+--+--+--+--+--+--+--+--+--+--+--+
             * | |  |  |  |  |  |  |  |  |  |  |  |  |  |
             * | +--+--+--+--+--+--+--+--+--+--+--+--+--+
             * | |  |  |  |  |  |  |  |  |  |  |  |  |  |
             * | +--+--+--+--+--+--+--+--+--+--+--+--+--+
             * | |  |  |  |  |  |  |  |  |  |  |  |  |  |
             *X| +--+--+--+--+--+--+--+--+--+--+--+--+--+
             * v |  |  |  |  |X |  |  |  |  |  |  |  |  |
             *   +--+--+--+--+--+--+--+--+--+--+--+--+--+
             *   |  |  |  |  |  |  |  |  |  |  |  |  |  |
             *   +--+--+--+--+--+--+--+--+--+--+--+--+--+
             *   |  |  |  |  |  |  |  |  |  |  |  |  |  |
             *   +--+--+--+--+--+--+--+--+--+--+--+--+--+
             * + -------------> 
             *       Y
             *
            i++;
        }
    }

    for(x=0; x < 5; x++)
    {
        for(y=0; y < 5; y++)
        {
            printf("x:[%d] y:[%d] = %d\n", x, y, ((int*)anker->data)[x_size*x+y]);
        }
    }*/


//-----------------------------------------------------------------------------
    //Ein String Array mit der laenge von vier
    /*anker = malloc(sizeof(struct variables));

    anker->length = 4;

    anker->data = malloc(sizeof(char*)*arraylength);

    *(char**)(anker->data+sizeof(char*)*0) = malloc(sizeof(char)*stringlength);
    strcpy(*(char**)(anker->data+sizeof(char*)*0), "Hello World1");

    *(char**)(anker->data+sizeof(char*)*1) = malloc(sizeof(char)*stringlength);
    strcpy(*(char**)(anker->data+sizeof(char*)*1), "Hello World2");

    *(char**)(anker->data+sizeof(char*)*2) = malloc(sizeof(char)*stringlength);
    strcpy(*(char**)(anker->data+sizeof(char*)*2), "Hello World3");

    *(char**)(anker->data+sizeof(char*)*3) = malloc(sizeof(char)*stringlength);
    strcpy(*(char**)(anker->data+sizeof(char*)*3), "Hello World4");

    for(i=0; i < anker->length; i++)
        printf("String index=[%d] value=[%s]\n", i,
            *(char**)(anker->data+(sizeof(char*)*i)));*/

//-----------------------------------------------------------------------------
    //Ein einfacher String
    /*anker = malloc(sizeof(struct variables));
    strcpy(anker->name, "Testint");

    anker->data = malloc(sizeof(char)*stringlength);
    strcpy((char*)anker->data, "Hello World");

    printf("String: [%s]\n", (char*)anker->data);*/
 
//-----------------------------------------------------------------------------
    //Int Array mit der groesse von vier.
    /*anker->data = malloc(sizeof(int)*arraylength);
    anker->length = arraylength;

    *(int*)anker->data = 15;
    *(int*)(anker->data+(sizeof(int)*1)) = 16;
    *(int*)(anker->data+(sizeof(int)*2)) = 17;
    *(int*)(anker->data+(sizeof(int)*3)) = 18;

    for(i=0; i < anker->length; i++)
        printf("INTArray: index=[%d] value=[%d]\n", i, *(int*)(anker->data+(sizeof(int)*i)));*/

//-----------------------------------------------------------------------------
    //Int Var erstellen und darauf zugreifen
    //anker->data = malloc(sizeof(int));
    //*(int*)anker->data = test;
    //printf("Int: [%d]\n", *(int *)anker->data);

    return(0);
}
