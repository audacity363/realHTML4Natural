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

extern long rh4nnatGenJSON();
extern long rh4nnatGetURLObj();
extern long rh4nnatGetBodyObj();
/*
** section 3: initialize the jumptable
**
** the function name visible to NATURAL must not be longer than 8
** uppercase characters.  they must contain only uppercase letters.
** the entries in the array must be alphabetically (!!!!!!!!!!!!!!) sorted by the
** function names.
*/

TAB_STRUCT n_call_table[] = {

  { "GENJSON", rh4nnatGenJSON},
  { "GETURLO", rh4nnatGetURLObj},
  { "GETBODO", rh4nnatGetBodyObj}

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
