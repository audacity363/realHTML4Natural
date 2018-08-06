#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

#include "rh4n.h"
#include "rh4n_json.h"

int rh4njsonPrintJSONToFile(RH4nVarList *varlist, char *filename, RH4nProperties *props) {
    //TODO: Open file and call rh4njsonPrintJSON
}

void rh4njsonPrintJSON(RH4nVarList *varlist, FILE *outputfile, RH4nProperties *props) {
    rh4njsonPrintObject(varlist->anker, outputfile, 0, props);
}

void rh4njsonPrintObject(RH4nVarEntry_t *anker, FILE *outputfile, int level, RH4nProperties *props) {
    RH4nVarEntry_t *hptr = NULL;

    if(rh4njsonCheckIfOnlyOneGroup(anker, props) == true) {
        if(rh4njsonCheckIfJSONArray(anker, props) == true)   {
            rh4n_log_debug(props->logging, "Generating group [%s] as array", anker->name);
            rh4njsonPrintJSONObjectArray(anker, outputfile, props);
            return;
        }

        //I have no idear why I have written this piece of code but i leave it in
        //in case i can remeber it....
#if 0
        rh4n_log_develop(props->logging, "Not only arrays in [%s]", anker->name);
        fprintf(outputfile, "}");
        fprintf(outputfile, "{");
        return;
#endif
    } else {
        rh4n_log_develop(props->logging, "Found multiple Groups in root");
    }
    rh4n_log_debug(props->logging, "Multiple entries on level %d", level);

    if(rh4njsonCheckIfJSONArray(anker, props) == true) {
        rh4n_log_debug(props->logging, "All entries are arrays. Generate as object array");
        rh4njsonPrintJSONObjectArray(anker, outputfile, props);
        return;
    }
    rh4n_log_debug(props->logging, "Just generate a normal object");

    if(anker->prev != NULL || level == 0) fprintf(outputfile, "{");

    for(hptr = anker; hptr != NULL; hptr = hptr->next) {
        fprintf(outputfile, "\"%s\":", hptr->name);
        if(hptr->var.type == RH4NVARTYPEGROUP) {
            rh4njsonPrintObject(hptr->nextlvl, outputfile, level+1, props);
        } else if(hptr->var.type == RH4NVARTYPEARRAY) {
            rh4njsonPrintJSONArray(&hptr->var, outputfile, props);
        } else {
            rh4nvarPrintVar(&hptr->var, props, outputfile); 
        }
        if(hptr->next != NULL) { fprintf(outputfile, ","); }
    }

    if(anker->prev != NULL || level == 0) fprintf(outputfile, "}");
}

