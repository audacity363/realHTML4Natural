#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

#include "rh4n.h"
#include "rh4n_json.h"

bool rh4njsonCheckIfOnlyOneGroup(RH4nVarEntry_t *anker, RH4nProperties *props) {
    RH4nVarEntry_t *hptr = NULL;
    int groups_found = 0;

    for(hptr = anker; hptr != NULL; hptr = hptr->next) {
        if(hptr->var.type != RH4NVARTYPEGROUP) {
            if(++groups_found > 1) { return(false); }
        } else {
            return(false);
        }
    }
    return(true);
}
