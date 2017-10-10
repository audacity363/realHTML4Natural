/***********************************************************************
**
** file    : jumptab.c
** purpose : linkage between natural and the user defined functions
**
** (c) copyright 1991,92,93 by Software AG
**
************************************************************************
**
** section 1: define structure of a jump table entry.
**
** DO NOT CHANGE ANYTHING IN THIS SECTION !
*/
#include "nattab.h"

/*
** section 2: define external example routines
**
** START OF CODE TO BE CHANGED BY THE USER
*/

extern long gen_page();
extern long gen_json();
extern long gen_json_objs();


/*
** section 3: initialize the jumptable
**
** the function name visible to NATURAL must not be longer than 8
** uppercase characters.  they must contain only uppercase letters.
** the entries in the array must be alphabetically (!!!!!!!!!!!!!!) sorted by the
** function names.
*/

TAB_STRUCT n_call_table[] = {

  { "GENJSONO", gen_json_objs},
  { "GENJSON", gen_json},
  { "GENPAGE", gen_page}

};


/*
** section 4: make number of elements in the jump table availale to
** NATURAL.
**
** END OF CODE TO BE CHANGED BY THE USER
*/

TAB_SIZE n_call_tab = sizeof(n_call_table) / sizeof(n_call_table[0]);

/*
** end of file jumptab.c
*/
