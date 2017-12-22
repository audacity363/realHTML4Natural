#ifndef RH4N_VAR2NAME
#define RH4N_VAR2NAMe

int startvar2name(vars_t*, char*, bool, FILE*, char*, char*);
int startmatchPosition(char*, vars_t *varanker, char*);

typedef struct varposition {
    //Entry in the ll
    vars_t *vartarget;

    //Position in the paramlist from natural
    int parm_position;
} varposition_t;

typedef struct posinfo {
    int length;
    varposition_t *tab;
} positioninfo_t;

#endif
