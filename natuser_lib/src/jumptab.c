/**
 * @file jumptab.c
 *
 * @brief Lookup table for natural for the user exists in this shared Library
 *
 * (I shouldn't edit anything at this position in the file. Upsss....)
 *
 * @author SoftwareAG / Tom Engemann
 */

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
extern long printvars();
extern long getVar();
extern long getJSONObject();


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
  { "GENPAGE", gen_page},
  { "GETVAR", getVar},
  { "GETJSONO", getJSONObject },
  { "PRITVARS", printvars}

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
