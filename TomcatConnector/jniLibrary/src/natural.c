#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>

#include "vars.h"

#include "realHTML_tomcat_connector_JNINatural.h"
#include "natni.h"
#include "jni_rh4n.h"

#define HTML_ERROR_HEAD "<html><head><title>Natural Error: [%d]</title></head><body>\n"

struct naturalparms *getParmByName(struct naturalparms *parms, int length, char *searchname);
int setValuelengthParm(pnni_611_functions s_funcs, JNIEnv *env, 
    struct naturalparms *parms, int length, void *natnniparms,
    int index);
void printNaturalParmsStruct(struct parameter_description *nniparams, FILE*);
void printNaturalException(struct natural_exception *pnatexcep, FILE *logfile);
int printErrortoFile(struct natural_exception ext, char *outfile, char *error);

char *natparmnames[] = {
    "keys",
    "vals",
    "val_length",
    "parm_count",
    "reg_type",
    "tmp_file",
    "settings",
    "bodyvars"
};

static struct parameter_description* generateNaturalParams(pnni_611_functions s_funcs, 
    JNIEnv *env, struct naturalparms *parms, int length, vars_t *bodyvars)
{
    static struct parameter_description nniparams[8];
    struct naturalparms *target;

    //Must this freed with nni_delete_parm??? What happens with the parms in nniparams??
    void *pvnniparms = NULL;
    int flags = 0, i,x,
        occ[3] = {0, 0, 0};

    s_funcs->pf_nni_create_parm(s_funcs, NATPARMSCOUNT+1, &pvnniparms);

    flags |= NNI_FLG_UBVAR_0;
    
    for(i=0; i < sizeof(natparmnames)/sizeof(char*); i++)
    {
        //Handle special parms
        if(strcmp(natparmnames[i], "val_length") == 0)
        {
            setValuelengthParm(s_funcs, env, parms, length, pvnniparms, i);
            s_funcs->pf_nni_get_parm_info(s_funcs, i, pvnniparms, &nniparams[i]);
            continue;
        }
        else if(strcmp(natparmnames[i], "parm_count") == 0)
        {
            setParmcountParm(s_funcs, env, parms, length, pvnniparms, i);
            s_funcs->pf_nni_get_parm_info(s_funcs, i, pvnniparms, &nniparams[i]);
            continue;
        }
        else if(strcmp(natparmnames[i], "bodyvars") == 0) {
            s_funcs->pf_nni_init_parm_s(s_funcs, i, pvnniparms, NNI_TYPE_BIN, sizeof(vars_t*), 0, NULL);
            s_funcs->pf_nni_put_parm(s_funcs, i, pvnniparms, sizeof(vars_t*), &bodyvars);
            s_funcs->pf_nni_get_parm_info(s_funcs, i, pvnniparms, &nniparams[i]);
            continue;
        }

        target = getParmByName(parms, length, natparmnames[i]);
        if(target->array_length != -1)
        {
            occ[0] = target->array_length;
            s_funcs->pf_nni_init_parm_da(s_funcs, i, pvnniparms, NNI_TYPE_ALPHA,
                1, occ, flags);
            for(x=0; x < target->array_length; x++)
            {
                occ[0] = x;
                s_funcs->pf_nni_put_parm_array(s_funcs, i, pvnniparms, 
                    strlen(target->getter(env, target, x)), 
                    target->getter(env, target, x), occ);
            }
        }
        else
        {
            s_funcs->pf_nni_init_parm_d(s_funcs, i, pvnniparms, NNI_TYPE_ALPHA, 0);
            s_funcs->pf_nni_put_parm(s_funcs, i, pvnniparms, 
                strlen(target->getter(env, target, x)), 
                target->getter(env, target, x));
        }
        s_funcs->pf_nni_get_parm_info(s_funcs, i, pvnniparms, &nniparams[i]);
    }

    return(nniparams);
}

int setValuelengthParm(pnni_611_functions s_funcs, JNIEnv *env, 
    struct naturalparms *parms, int length, void *natnniparms,
    int index)
{
    struct naturalparms *target;
    int i, occ[3] = {0,0,0}, 
        flags = 0, strlength = 0, rc = 0;

    flags |= NNI_FLG_UBVAR_0;

    target = getParmByName(parms, length, "vals");

    occ[0] = target->array_length;

    if((rc = s_funcs->pf_nni_init_parm_sa(s_funcs, index, natnniparms, NNI_TYPE_INT, 4, 0,
        1, occ, flags)) != NNI_RC_OK)
    {
        return(rc);
    }

    for(i=0; i < target->array_length; i++)
    {
        occ[0] = i;
        strlength = strlen(target->getter(env, target, i));
        if((rc = s_funcs->pf_nni_put_parm_array(s_funcs, index, natnniparms, 
            sizeof(int), &strlength, occ)) != NNI_RC_OK)
        {
            return(rc);
        }
    }
    return(0);
}

int setParmcountParm(pnni_611_functions s_funcs, JNIEnv *env, 
    struct naturalparms *parms, int length, void *natnniparms,
    int index)
{
    int rc = 0;
    struct naturalparms *target;

    target = getParmByName(parms, length, "keys");

    if((rc = s_funcs->pf_nni_init_parm_s(s_funcs, index, natnniparms, NNI_TYPE_INT, 
        sizeof(int), 0, 0)) != NNI_RC_OK)
    {
        return(rc);
    }

    if((rc = s_funcs->pf_nni_put_parm(s_funcs, index, natnniparms,
	    sizeof(int), &(target->array_length))) != NNI_RC_OK)
    {
        return(rc);
    }

    return(NNI_RC_OK);
}

int callNatural(JNIEnv *env, struct naturalparms *parms, int length, 
    char *error, FILE *logfile, char ***envvars, int envvars_len, vars_t *bodyvars)
{
    void *shNNILib = NULL, *nat_parmhandle = NULL;
    pnni_611_functions nnifuncs;
    PF_NNI_GET_INTERFACE pf_nni_get_interface = 0;
    struct natural_exception nat_ex;
    struct naturalparms *target;

    struct parameter_description *natNNIparms;
    char *natlibrary = NULL, *natprogram = NULL, *natcliparms = NULL,
         *errormsg, *outfile = NULL;
    int rc = 0, i = 0, nat_parmhandle_length = 0;

    target = getParmByName(parms, length, "nat_library");
    natlibrary = target->getter(env, target, 0);

    target = getParmByName(parms, length, "nat_program");
    natprogram = target->getter(env, target, 0);

    target = getParmByName(parms, length, "natparams");
    natcliparms = target->getter(env, target, 0);

    for(; i < envvars_len; i++) {
        putenv((*envvars)[i]);
    }

    if((errormsg = OpenLib(&shNNILib, "libnatural.so")) != NULL)
    {
        strcpy(error, errormsg);
        return(-1);
    }

    pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(shNNILib, "nni_get_interface");

    (pf_nni_get_interface)(NNI_VERSION_CURR, (void**)&nnifuncs);

    debugFileprint(logfile, "Initialize Natural...");
    if((rc = nnifuncs->pf_nni_initialize(nnifuncs, natcliparms, 0, 0)) != NNI_RC_OK)
    {
        strcpy(error, natErrno2Str(rc));
        debugFileprint(logfile, "...Fail [%s](%d)\n", error, rc);
        return(-1);
    }
    debugFileprint(logfile, "...Done\n");

    // Is this really needed?! This is the only Natural session in the process
    debugFileprint(logfile, "Wait for exclusive access...\n");
    nnifuncs->pf_nni_enter(nnifuncs);
    debugFileprint(logfile, "... I have the session all for myself\n");


    natNNIparms = generateNaturalParams(nnifuncs, env, parms, length, bodyvars);
    //printNaturalParmsStruct(natNNIparms, logfile);

    //logon into natural library
    debugFileprint(logfile, "Logon to [%s]...", natlibrary);
    if((rc = nnifuncs->pf_nni_logon(nnifuncs, natlibrary, NULL, NULL)) != NNI_RC_OK)
    {
        strcpy(error, natErrno2Str(rc));
        debugFileprint(logfile, "...Fail [%s]\n", error);
        return(-2);
    }
    debugFileprint(logfile, "...Done\n");

    //Check if the last parm (B8) is defined. 
    //Just hacked in to get backwards compatibility. See TODO
    //TODO: Write a function which gets the signature of the PDA and creates it dynamiclly

    printf("Getting parm signature...");
    if((rc = nnifuncs->pf_nni_create_module_parm(nnifuncs, 'N', natprogram, &nat_parmhandle)) != NNI_RC_OK) {
        strcpy(error, natErrno2Str(rc));
        debugFileprint(logfile, "...Fail [%s]\n", error);
        return(-3);
    }
    debugFileprint(logfile, "...Done\n");

    printf("Getting nat parm length...");
    if((rc = nnifuncs->pf_nni_parm_count(nnifuncs, nat_parmhandle, &nat_parmhandle_length)) != NNI_RC_OK) {
        strcpy(error, natErrno2Str(rc));
        debugFileprint(logfile, "...Fail [%s]\n", error);
        return(-3);
    }
    debugFileprint(logfile, "...Done\n");

    //Call the natural program
    debugFileprint(logfile, "Calling program [%s]...", natprogram);
    if((rc = nnifuncs->pf_nni_callnat(nnifuncs, natprogram, nat_parmhandle_length, natNNIparms, 
        &nat_ex)) != NNI_RC_OK)
    {
        //Not an NNI error rather a natural error
        if(rc > 0)
        {
            sprintf(error, "Natural Error %d", rc);
            target = getParmByName(parms, length, "tmp_file");
            outfile = target->getter(env, target, 0);
            debugFileprint(logfile, "\n\tWrite Natural Exception to [%s]...", outfile);
            if(printErrortoFile(nat_ex, outfile, error) != 0)
            {
                debugFileprint(logfile, "...Fail [%s]\n", error);
                return(-1);
            }
            debugFileprint(logfile, "...Done\n");
        }
        else
            strcpy(error, natErrno2Str(rc));
        debugFileprint(logfile, "...Fail [%s]\n", error);
        printNaturalException(&nat_ex, logfile);
        return(rc);
    }
    debugFileprint(logfile, "...Done\n");

    nnifuncs->pf_nni_leave(nnifuncs);

    //close everything natural related
    debugFileprint(logfile, "Uninitialize and freeing nni...");
    nnifuncs->pf_nni_uninitialize(nnifuncs);
    nnifuncs->pf_nni_free_interface(nnifuncs);
    debugFileprint(logfile, "...Done\n");

    CloseLib(&shNNILib);
    return(0);
}

char *OpenLib(void **shLib, char *name)
{
    static char error[1024];

    *shLib = dlopen(name, RTLD_NOW);
    if(!*shLib)
    {
        sprintf(error, "Error while loading Module [%s]: [%s]", name, dlerror());
        return(error);
    }
    return(NULL);
}

void CloseLib(void **shLib)
{
        dlclose(*shLib);
}

struct naturalparms *getParmByName(struct naturalparms *parms, int length, char *searchname)
{
    int i=0;

    for(; i < length; i++)
    {
        if(strcmp(parms[i].jname, searchname) == 0)
        {
            return(&parms[i]);
        }
    }

    return(NULL);
}

void printNaturalParmsStruct(struct parameter_description *nniparams, FILE *logfile)
{
    int i=0;

    for(;i < 80; i++) debugFileprint(logfile, "-");
    debugFileprint(logfile, "\n");

    for(i=0; i < 7; i++)
    {
       debugFileprint(logfile, "Parms Nr.%d\n", i);
       debugFileprint(logfile, "\tAddress: [%p]\n", nniparams[i].address);
       debugFileprint(logfile, "\tFormat:  [%c]\n", nniparams[i].format);
       debugFileprint(logfile, "\tLength:  [%d]\n", nniparams[i].length_all);
       debugFileprint(logfile, "\tDims:    [%d]\n", nniparams[i].dimensions);
    }
    for(;i < 80; i++) debugFileprint(logfile, "-");
    debugFileprint(logfile, "\n");
}

void printNaturalException(struct natural_exception *pnatexcep, FILE *logfile)
{
    char error_str[2024];
    int error_line, i=0;
    for(; i < 80; i++) printf("-");
    printf("\n");

    if (pnatexcep)
    {
        debugFileprint(logfile, "\tMessageNumber: %d.\n",
            pnatexcep->natMessageNumber );
        debugFileprint(logfile, "\tMessageText:   %s\n",
            pnatexcep->natMessageText ? pnatexcep->natMessageText : "0" );
        debugFileprint(logfile,  "\tLibrary:       %s.\n",
            pnatexcep->natLibrary ? pnatexcep->natLibrary : "0" );
        debugFileprint(logfile, "\tMember:        %s.\n",
            pnatexcep->natMember ? pnatexcep->natMember : "0" );
        debugFileprint(logfile, "\tName:          %s.\n",
            pnatexcep->natName ? pnatexcep->natName : "0" );
        debugFileprint(logfile, "\tMethod:        %s.\n",
            pnatexcep->natMethod ? pnatexcep->natMethod : "0" );
        debugFileprint(logfile, "\tLine:          %d.\n",
            pnatexcep->natLine );
    }
    for(i=0; i < 80; i++) printf("-");
    printf("\n");
}

int printErrortoFile(struct natural_exception ext, char *outfile, char *error)
{
    FILE *output;

    if((output = fopen(outfile, "w")) == NULL)
    {
        strcpy(error, strerror(errno));
        return(-1);
    }

    fprintf(output, HTML_ERROR_HEAD, ext.natMessageNumber);
    fprintf(output, "<h1>A Natural Runtime Error occurred</h1>\n");
    fprintf(output, "<p><span>Message Text:</span>%s</p>\n", ext.natMessageText);
    fprintf(output, "<p><span>Error Number:</span>%d</p>\n", ext.natMessageNumber);
    fprintf(output, "<p><span>Library:</span>%s</p>\n", ext.natLibrary);
    fprintf(output, "<p><span>Error Line:</span>%d</p>\n", ext.natLine);
    fprintf(output, "</body></html>");

    fclose(output);
    return(0);
}
