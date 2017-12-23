#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <wchar.h>
#include <string.h>
#include <signal.h>

#include "natuser.h"
#include "natni.h"

#include "vars.h"
#include "standard.h"
#include "rh4n.h"

#include "rh4n_json_mapper.h"
#include "natutils.h"
#include "var2name.h"

#define VARPTR_PARM_POS 0
#define LDA_STR_PARM_POS 1
#define SETTINGS_PARMS_POS 2

struct LDAInfos {
    char struct_name[NNI_LEN_NAME+1];
    char library[NNI_LEN_LIBRARY+1];
    char ldaname[NNI_LEN_MEMBER+1];
};

int parseFormatStr(pnni_611_functions nnifuncs, void *parmhandle, struct LDAInfos *ldainfos, struct settings_s*);
int parseSettings(void *parmhandle, pnni_611_functions nni_funcs,
    struct settings_s *settings, int);

/*
 *  Parms: 
 *      1: var ptr
 *      2: LDA Name and struct name (format: "struct-name@<libname>.ldaname")
 *      3: settings
 *      4: json struct
 */
long getJSONObject(WORD nparms, void *parmhandle, void *traditional) {
    if(traditional) 
        return(RH4N_RET_PARM_MISSMATCH);

    if(nparms < 4) {
        printf("parm missmatch. Expected min [4] got [%d]\n", nparms);
        return(RH4N_RET_PARM_MISSMATCH);
    }

    void *shNNILib = NULL;
    pnni_611_functions nnifuncs = NULL;
    int rc = 0, i = 0;
    vars_t *var_anker = NULL;
    struct LDAInfos ldainfos;
    struct settings_s settings;
    char *ldapath = NULL, *natsrc = NULL;

    memset(&ldainfos, NULL, sizeof(struct LDAInfos));

    if((nnifuncs = getNNIFunctions(&shNNILib)) == NULL) {
        printf("Could not open libnatural.so\n");
        return(RH4N_RET_SO_ERR);
    }

    parseSettings(parmhandle, nnifuncs, &settings, SETTINGS_PARMS_POS);

    for(i; i < settings.length; i++) {
        printf("Key: [%s]: value: [%s]\n", settings.key[i], settings.value[i]);
    }

    if((var_anker = getVarPtr(nnifuncs, parmhandle, VARPTR_PARM_POS)) == NULL) {
        return(RH4N_RET_VAR_PTR_ERR);
    }

    if((rc = parseFormatStr(nnifuncs, parmhandle, &ldainfos, &settings)) != RH4N_RET_OK) {
        printf("Format str not correct\n");
        return(rc);
    }

    //TODO: check settings    
  
    natsrc = getSetting(&settings, "natsrc");
    if((ldapath = malloc(sizeof(char)*(sizeof(struct LDAInfos)+strlen(natsrc)+10))) == NULL) {
        return(RH4N_RET_MEMORY_ERR);
    }

    sprintf(ldapath, "%s/%s/GP/%s.NGL", natsrc, ldainfos.library, ldainfos.ldaname);

    printf("User LDA: [%s]\n", ldapath);
    positioninfo_t postab = {-1, NULL };
    if((rc = startmatchPosition(ldapath, var_anker, ldainfos.struct_name, &postab)) != RH4N_RET_OK) {
        printf("Something went wrong\n");
        return(rc);
    }

    if(nparms < postab.length+3) {
        printf("Some arguments semse to be missing\n");
        return(RH4N_RET_PARM_MISSMATCH);
    }

    for(i=0; i < postab.length; i++) {
        printf("Var: [%s] on pos [%d]\n", postab.tab[i].vartarget->name, postab.tab[i].parm_position);
        if((rc = checkVarcompatibility(postab.tab[i].vartarget , nnifuncs, parmhandle, 
                postab.tab[i].parm_position+3)) != RH4N_RET_OK) {
            printf("Something went wrong while processing var [%s]\n", postab.tab[i].vartarget->name);
            return(rc);
        }
    }

    return(RH4N_RET_OK);
}

int parseFormatStr(pnni_611_functions nnifuncs, void *parmhandle, struct LDAInfos *ldainfos, struct settings_s *settings) {
    int i = 0, rc = 0;
    struct parameter_description ldastr_pd;
    char formatstr_buff[sizeof(struct LDAInfos)+3],
        *at_pos = NULL, *point_pos = NULL;

    memset(formatstr_buff, NULL, sizeof(struct LDAInfos)+3);

    if((rc = nnifuncs->pf_nni_get_parm_info(nnifuncs, LDA_STR_PARM_POS, parmhandle, &ldastr_pd)) != NNI_RC_OK) {
        return(RH4N_RET_NNI_ERR);
    }

    if(ldastr_pd.format != NNI_TYPE_ALPHA && ldastr_pd.dimensions != 0 && ldastr_pd.length_all < (sizeof(struct LDAInfos)+2)) {
        printf("Something is not right with the format str");
        return(RH4N_RET_PARM_TYPE_MISSMATCH);
    }

    if((rc = nnifuncs->pf_nni_get_parm(nnifuncs, LDA_STR_PARM_POS, parmhandle, ldastr_pd.length_all, 
            formatstr_buff)) != NNI_RC_OK)  {
        return(RH4N_RET_NNI_ERR);
    }

    for(i = 0; i < strlen(formatstr_buff); i++) {
        if(formatstr_buff[i] == '.' && at_pos == NULL) {
            //Found a point before a '@' char
            return(RH4N_RET_MALFORMED_FORMAT_STR);
        } else if(formatstr_buff[i] == '.' && point_pos != NULL) {
            //Found second point
            return(RH4N_RET_MALFORMED_FORMAT_STR);
        } else if(formatstr_buff[i] == '@' && at_pos != NULL) {
            //Found a second @
            return(RH4N_RET_MALFORMED_FORMAT_STR);
        } else if(formatstr_buff[i] == '.') {
            point_pos = &formatstr_buff[i];
        } else if(formatstr_buff[i] == '@') {
            at_pos = &formatstr_buff[i];
        }
    }

    if(at_pos == NULL) {
        //Didn't found an '@' char
        return(RH4N_RET_MALFORMED_FORMAT_STR);
    } else if(at_pos == formatstr_buff) {
        //struct name is empty
        return(RH4N_RET_MALFORMED_FORMAT_STR);
    } 

    *at_pos = '\0';
    if(strlen(formatstr_buff) > NNI_LEN_NAME) {
        //struct name to long
        return(RH4N_RET_BUFFER_OVERFLOW);
    }

    strcpy(ldainfos->struct_name, formatstr_buff);

    if(point_pos) {
        if(point_pos == at_pos+1) {
            //library entry is empty
            return(RH4N_RET_MALFORMED_FORMAT_STR);
        }
        *point_pos = '\0';
        if(strlen(at_pos+1) > NNI_LEN_LIBRARY) {
            //lib name is to long
            return(RH4N_RET_BUFFER_OVERFLOW);
        }

        strcpy(ldainfos->library, at_pos+1);

        if(strlen(point_pos+1) == 0) {
            //LDA name is empty
            return(RH4N_RET_MALFORMED_FORMAT_STR);
        } else if(strlen(point_pos+1) > NNI_LEN_MEMBER) {
            //LDA name is to long
            return(RH4N_RET_BUFFER_OVERFLOW);
        }

        strcpy(ldainfos->ldaname, point_pos+1);
    } else {
        if(strlen(at_pos+1) == 0) {
            //LDA name is emptty
            return(RH4N_RET_MALFORMED_FORMAT_STR);
        } else if(strlen(at_pos+1) > NNI_LEN_MEMBER) {
            //LDA name is to long
            return(RH4N_RET_BUFFER_OVERFLOW);
        }
        strcpy(ldainfos->ldaname, at_pos+1);

        strcpy(ldainfos->library, getSetting(settings, "lib"));
    } 

    printf("json-struct: [%s]\n", ldainfos->struct_name);
    printf("lib: [%s]\n", ldainfos->library);
    printf("lda: [%s]\n", ldainfos->ldaname);

    return(RH4N_RET_OK);
}
