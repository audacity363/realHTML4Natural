#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "rh4n_nat.h"

long rh4nnatGenJSON(WORD nparms, void *parmhandle, void *traditional) {
    if(traditional)  return(RH4N_RET_TARADITIONAL); 
    if(nparms <= 2)  return(RH4N_RET_PARM_MISSMATCH);

    char errorstr[2048];
    int utilsret = 0;
    RH4nNatInit_t initparms = {NULL, NULL, NULL, NULL};
    struct RH4nNatLDAInfos ldainfos;
    RH4nVarList varlist;

    if((utilsret = rh4nnatInit(parmhandle, &initparms, errorstr)) != RH4N_RET_OK) {
        return(utilsret);
    }

    rh4n_log_debug(initparms.props->logging, "Sucessfully got default parms:");
    rh4n_log_debug(initparms.props->logging, "formatstr: [%s]", initparms.formatstr);

    rh4n_log_debug(initparms.props->logging, "Start parsing formatstr");
    if((utilsret = rh4nnatParseFormatStr(initparms.formatstr, &ldainfos, initparms.props)) != RH4N_RET_OK) {
        rh4nUtilscloseSharedLibrary(initparms.sharedlibrary);
        free(initparms.formatstr);
        return(utilsret);
    }
    rh4n_log_info(initparms.props->logging, "Got following LDA infos:");
    rh4n_log_info(initparms.props->logging, "Struct name: [%s]", ldainfos.struct_name);
    rh4n_log_info(initparms.props->logging, "Library: [%s]", ldainfos.library);
    rh4n_log_info(initparms.props->logging, "LDA: [%s]", ldainfos.ldaname);

    rh4nvarInitList(&varlist);

    if((utilsret = rh4nnatStartVariableReadout(nparms, parmhandle, initparms.nnifuncs, 
            &varlist, initparms.props, errorstr)) != RH4N_RET_OK) {
        rh4n_log_error(initparms.props->logging, "Error while reading variables: %d %s", utilsret, errorstr);
        rh4nUtilscloseSharedLibrary(initparms.sharedlibrary);
        free(initparms.formatstr);
        return(utilsret);
    }

    rh4nvarPrintJSONToFile(&varlist, "./out.json", initparms.props);

    rh4nUtilscloseSharedLibrary(initparms.sharedlibrary);
    free(initparms.formatstr);

    return(RH4N_RET_OK);
}
