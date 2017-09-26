#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "vars.h"
#include "parser.h"
#include "parser_errno.h"
#include "exec_if.h"
#include "error_vars.h"

/*
 * Currently the if just works with one query. The rest of the
 * head is ignored
 */
int exec_if(if_parms_t *parms, wchar_t **body, int body_length)
{
    int i = 0,
        length = -1, memcmpret = 0,
        ret = 0;

    int truth_lookup[7] =  
    {
        IFGREATERTHEN,
        IFGREATERTHENEQUALS,
        IFEQUALS,
        IFUNEQUALS,
        IFLESSTHENEQUAL,
        IFLESSTHEN
    };
    
    // Lookup if the if is true or false
    int truth_table[7][3] =
    {
      // 0  1 -1 <-- memcmp return value
        {0, 1, 0}, // >
        {1, 1, 0}, // >=
        {1, 0, 0}, // ==
        {0, 1, 1}, // !=
        {1, 0, 1}, // <=
        {0, 0, 1}  // <
    };

    if_parms_t *leftval = NULL,
               *compare = NULL,
               *rightval = NULL;

    status_t status = {-1, 0, 0, 0, 0, 0, NULL};

    //Check if the first three parms are set
    if(!parms->next)
       return(-1);
    if(!parms->next->next)
       return(-2);
    if(!parms->next->next->next)
       return(-3);

    leftval = parms->next; compare = parms->next->next; rightval = parms->next->next->next;

    //Get the shortest length from the two parms
    if(leftval->sizeof_data > rightval->sizeof_data)
        length = rightval->sizeof_data;
    else if(leftval->sizeof_data > rightval->sizeof_data)
        length = leftval->sizeof_data;
    else
        length = leftval->sizeof_data;

    
    memcmpret = memcmp(leftval->data, rightval->data, length);
    fprintf(logfile, "Memret: [%d]\n", memcmpret);

    for(i=0; i < 6; i++)
    {
        if(compare->type == truth_lookup[i])
            break;
    }

    if(memcmpret < 0)
        memcmpret += 3;

    fprintf(logfile, "the truth: [%d]\n", truth_table[i][memcmpret]);
    if(truth_table[i][memcmpret] == 0)
    {
        return(0);
    }

    fprintf(logfile, "Body length: [%d]\n", body_length);

    fprintf(logfile, "---------If body-------\n");
    for(i=1; i < body_length; i++)
    {
        fprintf(logfile, "line: [%S]\n", body[i]);
        if((ret = parseLine(body[i], &status)) == EXIT)
            break;
        //fprintf(logfile, "ret: [%d]\n", ret);
    }
    //fprintf(logfile, "last ret: [%d]\n", ret);
    fprintf(logfile, "---------end-if--------\n");
    freeLineBuff(&status);
    return(ret);
}
