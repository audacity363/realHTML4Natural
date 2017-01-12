bool GrpisDefined(vars_t *anker, char *name);
int moveVariableToGroup(vars_t *anker, char *var_name, char *grp_name, vars_t **next_entry);
int addGroup(vars_t *anker, char *name, int x_length, int y_length, int z_length);
vars_t *getGrpPointer(vars_t *anker, char *name);
