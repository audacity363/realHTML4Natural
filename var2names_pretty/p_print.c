#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <sys/ioctl.h>

#define VAR2NAME

#include "vars.h"
#include "lda.h"
#include "rh4n.h"
#include "rh4n_utils.h"

#define NAME "Name: "
#define MAX_VARNAME_LENGTH 32

#define BOX_WIDTH (strlen(NAME)+MAX_VARNAME_LENGTH+4)


int p_print_entry(vars_t *entry1, vars_t *entry2, int ingrp1, int ingrp2, vars_t *highlight1, vars_t *highlight2);

void p_print(vars_t *lda_anker, vars_t *nat_anker, vars_t *highlight1, 
    vars_t *highlight2)
{
    vars_t *hptr1 = NULL, *hptr2 = NULL,
           *grphead1 = NULL, *grphead2 = NULL;
    int ingrp1 = 0, ingrp2 = 0;

    hptr1=lda_anker->next; hptr2=nat_anker->next;
    while(hptr1 || hptr2)
    {
        if(hptr1 == NULL && hptr2 != NULL)
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr2 = hptr2->next;
        }
        else if((hptr1->next_lvl != NULL && ingrp1 == 0) && hptr2 == NULL)
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            grphead1 = hptr1;
            hptr1 = hptr1->next_lvl;
            ingrp1 = 1;
        }
        else if(hptr1 != NULL && hptr2 == NULL)
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr1 = hptr1->next;
        }
        else if((hptr1->next_lvl != NULL && ingrp1 == 0) && (hptr2->next_lvl != NULL && ingrp2 == 0))
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            ingrp1 = ingrp2 = 1;
            grphead1 = hptr1;
            grphead2 = hptr2;
            hptr1=hptr1->next_lvl;
            hptr2=hptr2->next_lvl;

        }
        else if((hptr1->next_lvl != NULL && ingrp1 == 0) && (hptr2->next_lvl == NULL))
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            ingrp1 = 1;
            grphead1 = hptr1;
            hptr1 = hptr1->next_lvl;
            hptr2 = hptr2->next;
        }
        else if((hptr1->next == NULL && ingrp1 == 1) && (hptr2->next != NULL))
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr1 = grphead1->next;
            hptr2 = hptr2->next;
        }
        else if((hptr2->next == NULL && ingrp2 == 1) && (hptr1->next != NULL))
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr2 = grphead2->next;
            hptr1 = hptr1->next;
        }
        else if((hptr1->next == NULL && ingrp1 == 0) && (hptr2->next == NULL && ingrp2 == 0))
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr1=hptr1->next; hptr2=hptr2->next;
        }
        else if((hptr1->next == NULL && ingrp1 == 1) && hptr2->next == NULL)
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr2 = hptr2->next;
            ingrp1 = 0;
            hptr1 = grphead1->next;
        }
        else if((hptr1->next == NULL && ingrp1 == 1) && (hptr2->next == NULL && ingrp2 == 1))
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr1 = grphead1->next;
            hptr2 = grphead2->next;
            ingrp1 = ingrp2 = 0;
        }
        else if((hptr1->next_lvl == NULL) && (hptr2->next_lvl == NULL))
        {
            p_print_entry(hptr1, hptr2, ingrp1, ingrp2, highlight1, highlight2);
            hptr1=hptr1->next; hptr2=hptr2->next;
        }
    }
}

//Prints two var entry next to each other
int p_print_entry(vars_t *entry1, vars_t *entry2, int ingrp1, int ingrp2, vars_t *highlight1, vars_t *highlight2)
{
    struct winsize w;
    int size_width = 0, i = 0, x=0, grpoffset = 0, length1 = 0, length2 = 0, box_length= 0;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    size_width = (w.ws_col-2)/2;
    if(entry1)
    {
        if(ingrp1)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            putchar('|');
            for(i=0; i < 25; i++)
                putchar(' ');
            grpoffset = BOX_WIDTH+4;
        }

        //Print the head of the left box
        putchar('+');
        for(i=0; i < BOX_WIDTH-2; i++)
        {
            if(entry1 == highlight1)
                putchar('=');
            else
                putchar('-');
        }    
        putchar('+');


        //Spaces to the middle of the screen
        for(i=0; i < size_width-BOX_WIDTH-2-grpoffset; i++)
            putchar(' ');
    }
    else
        for(i=0; i < size_width-2; i++)
            putchar(' ');
    
    //Print the middle seperator
    printf("| ");

    if(entry2)
    {     
        if(ingrp2)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            putchar('|');
            for(i=0; i < 25; i++)
                putchar(' ');
        }
        //Print the head of the right box
        putchar('+');
        for(i=0; i < BOX_WIDTH-2; i++)
        {
            if(entry2 == highlight2)
                putchar('=');
            else
                putchar('-');
        }
        putchar('+');
    }
    putchar('\n');

//------------------END OF HEAD------------------

    if(entry1)
    {
        //Print varname of the left entry
        if(ingrp1)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            putchar('|');
            for(i=0; i < 25; i++)
                putchar(' ');
            /*for(i=0; i < BOX_WIDTH+4; i++)
                putchar(' ');*/
        }
        printf("| %s%-32.32s |", NAME, entry1->name);
        grpoffset = 0;
        if(entry1->next_lvl && ingrp1 == 0)
        {
            printf("----");
            for(i=0; i < 19; i++)
                putchar('-');

            putchar('+');
            grpoffset = 24;
        }
        //Spaces to the middle of the screen
        if(ingrp1)
        {
            for(i=0; i < size_width-(BOX_WIDTH+BOX_WIDTH+6); i++)
                putchar(' ');
        }
        else
        {
            for(i=0; i < size_width-BOX_WIDTH-2-grpoffset; i++)
                putchar(' ');
        }
    }
    else
        for(i=0; i < size_width-2; i++)
            putchar(' ');

    //Print the middle seperator
    printf("| ");
    
    if(entry2)
    {
        if(ingrp2)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            putchar('|');
            for(i=0; i < 25; i++)
                putchar(' ');

            //for(i=0; i < BOX_WIDTH+4; i++)
            //    putchar(' ');
            grpoffset = BOX_WIDTH+4;
        }
        //Print varname of the right entry
        printf("| %s%-32.32s |", NAME, entry2->name);
        if(entry2->next_lvl)
        {
            printf("----");
            for(i=0; i < 19; i++)
                putchar('-');

            putchar('+');
        }
    }
    putchar('\n');

//------------------END OF VAR-------------------
    grpoffset = 0;
    if(entry1)
    {
        if(ingrp1)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            putchar('|');
            for(i=0; i < 25; i++)
                putchar(' ');
            /*for(i=0; i < BOX_WIDTH+4; i++)
                putchar(' ');*/
            grpoffset = BOX_WIDTH+4;
        }
        //Print the bottom of the left box
        putchar('+');
        for(i=0; i < BOX_WIDTH-2; i++)
        {
            if(entry1 == highlight1)
                putchar('=');
            else
                putchar('-');
        }
        putchar('+');

        
        //Spaces to the middle of the screen
        if(entry1->next_lvl)
        {
            for(i=0; i < 23; i++)
                putchar(' ');
            putchar('|');
            for(i=0; i < size_width-(BOX_WIDTH+26); i++)
                putchar(' ');
        }
        else
        {
            for(i=0; i < size_width-BOX_WIDTH-2-grpoffset; i++)
                putchar(' ');
        }
    }
    else
        for(i=0; i < size_width-2; i++)
            putchar(' ');
    
    //Print the middle seperator
    printf("| ");

    if(entry2)
    {
        if(ingrp2)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            putchar('|');
            for(i=0; i < 25; i++)
                putchar(' ');
        }
        //Print the bottom of the right box
        putchar('+');
        for(i=0; i < BOX_WIDTH-2; i++)
        {
            if(entry2 == highlight2)
                putchar('=');
            else
                putchar('-');
        }
        putchar('+');
        if(entry2->next_lvl)
        {
            for(i=0; i < 23; i++)
                putchar(' ');
            putchar('|');
        }
    }
    putchar('\n');

    //Print connection to the next entry
    for(x=0; x < 2; x++)
    {
        if(entry1)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            if(!entry1->next && x==1 && ingrp1 == 0)
                putchar('X');
            else
                putchar('|');
            for(i=0; i < 20; i++)
                putchar(' ');
            if(ingrp1)
            {
                for(i=0; i < 24; i++)
                    putchar(' ');
                if(!entry1->next && x==1)
                    putchar('X');
                else
                    putchar('|');
                for(i=0; i < 20; i++)
                    putchar(' ');
                for(i=0; i < size_width-(BOX_WIDTH+BOX_WIDTH+4);i++)
                    putchar(' ');
            }
            else
            {
                //Spaces to the middle of the screen
                if(entry1->next_lvl)
                {
                    for(i=0; i < 24; i++)
                        putchar(' ');
                    putchar('|');
                    for(i=0; i < size_width-(BOX_WIDTH+26); i++)
                        putchar(' ');
                }
                else
                {
                    for(i=0; i < size_width-BOX_WIDTH-1; i++)
                        putchar(' ');
                }
            }
        }
        else
            for(i=0; i <size_width-2; i++)
                putchar(' ');

        
        printf("| ");
        
        if(entry2)
        {
            for(i=0; i < 20; i++)
                putchar(' ');
            if(!entry2->next && x==1 && ingrp2 == 0)
                putchar('X');
            else
                putchar('|');
            if(ingrp2)
            {
                for(i=0; i < 44; i++)
                    putchar(' ');
                if(!entry1->next && x==1)
                    putchar('X');
                else
                    putchar('|');
            }
            else
            {
                //Spaces to the middle of the screen
                if(entry2->next_lvl)
                {
                    for(i=0; i < 44; i++)
                        putchar(' ');
                    putchar('|');
                    for(i=0; i < size_width-(BOX_WIDTH+26); i++)
                        putchar(' ');
                }
                else
                {
                    for(i=0; i < size_width-BOX_WIDTH-1; i++)
                        putchar(' ');
                }
            }
        }
        
        putchar('\n');
    }

    return(0);
}
