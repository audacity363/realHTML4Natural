#ifndef RH4N_VAR2NAME
#define RH4N_VAR2NAMe

int startvar2name(vars_t*, char*, bool, FILE*, char*, char*);

typedef struct varposition {
    //Entry in the ll
    vars_t *vartarget;

    //Position in the paramlist from natural
    int parm_position;
} varposition_t;

#endif
