#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "rh4n.h"

#include <jni.h>
#include "json/jsonhandling.h"
#include "json/json_utils.h"
#include "json/handlers.h"

int singleVarHandler(JNIEnv *env, HandlerArgs args);

int (*getHandlerFuncton(jint vartype))(JNIEnv*, HandlerArgs) {
    //Lookup table for the handler functions
    
    struct handlerfuncs handlers[] = {
            { JVAR_STRING, singleVarHandler},
            { JVAR_STRING1D, Handler1D },
            { JVAR_STRING2D, Handler2D },
            { JVAR_STRING3D, Handler3D },
            { JVAR_INT, singleVarHandler},
            { JVAR_INT1D, Handler1D },
            { JVAR_INT2D, Handler2D },
            { JVAR_INT3D, Handler3D },
            { JVAR_BOOLEAN, singleVarHandler},
            { JVAR_BOOLEAN1D, Handler1D },
            { JVAR_BOOLEAN2D, Handler2D },
            { JVAR_BOOLEAN3D, Handler3D },
            { JVAR_FLOAT, singleVarHandler},
            { JVAR_FLOAT1D, Handler1D },
            { JVAR_FLOAT2D, Handler2D },
            { JVAR_FLOAT3D, Handler3D },
        }; 

    int length_of_handlers_tab = sizeof(handlers)/sizeof(struct handlerfuncs), 
        i = 0;

    for(; i < length_of_handlers_tab; i++) {
        if(handlers[i].vartype == vartype)
            return(handlers[i].handlerfunc);
    }

    return(NULL);
}


int singleVarHandler(JNIEnv *env, HandlerArgs args) {
    int rc = 0;
    jobject entry = NULL;    

    if((entry = (*env)->GetObjectField(env, args.curptr, args.infos->value)) == NULL) {
        rh4n_log_error(args.infos->logging, "Value field is == NULL");
        return(-1);
    }

    switch(args.vartype) {
        case JVAR_STRING:
            if((rc = handleStringEntry(env, args, entry, NULL)) != RH4N_RET_OK) return(rc);
            break;
        case JVAR_INT:
            if((rc = handleIntEntry(env, args, entry, NULL)) != RH4N_RET_OK) return(rc);
            break;
        case JVAR_FLOAT:
            if((rc = handleFloatEntry(env, args, entry, NULL)) != RH4N_RET_OK) return(rc);
            break;
        case JVAR_BOOLEAN:
            if((rc = handleBooleanEntry(env, args, entry, NULL)) != RH4N_RET_OK) return(rc);
            break;
            
    }

    return(RH4N_RET_OK);
}
