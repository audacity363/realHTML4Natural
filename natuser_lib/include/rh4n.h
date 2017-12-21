#ifndef RH4N
#define RH4N

#define RH4N_NULL_LENGTH_ERR 100
#define RH4N_NO_MEMORY 3
#define RH4N_LDA_FORMAT_ERR 1
#define RH4N_TEMPLATE_FORMAT_ERR 400
#define RH4N_TMPFILE_FORMAT_ERR 500
#define RH4N_SETTING_FORMAT_ERR 500
#define RH4N_VAR_LIBRARY_ERR 2
#define RH4N_UNKOWN_INT_TYPE 700
#define RH4N_XARRAY_ERROR 4
#define RH4N_SO_LOAD_ERROR 5

#define RH4N_GRP_HEAD "page"

#define NAT_BOOL_SIZE 1

struct settings_s {
    char **key;
    char **value;
    int length;
};

struct rh4n_parms {
    char *lda_name;
    char *template_name;
    char *tmp_file;
    struct settings_s settings;
};

int checkNNIReturnCode(int);
void trimleadingSpaces(char *input);
char *getSetting(struct settings_s *settings, char *name);
int getNames(vars_t *anker, struct rh4n_parms *parms, char *groupname);


extern FILE *logfile;

#ifndef VAR2NAME

int handleAlphaVariable(int index, void *parmhandle, pnni_611_functions nni_funcs,
                    struct parameter_description pb, char *name, vars_t *anker);
#endif


#endif
