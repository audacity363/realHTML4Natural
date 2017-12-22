#ifndef RH4N_JSON_MAPPER_NATUSER
#define RH4N_JSON_MAPPER_NATUSER

#define MAXI1 0xFF
#define MAXI2 0xFFFF
#define MAXI4 0xFFFFFFFF

#define I1SIZE 1
#define I2SIZE 2
#define I4SIZE 4
#define LSIZE 1
#define F4SIZE 4
#define F8SIZE 8

#define MAX_NAME_LENGTH (NNI_LEN_NAME*2+1)

#define GET_VAR_RET_PARM_POS 2
#define MAX_NAT_N_SIZE 29

int json_i_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_1di_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_2di_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_3di_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);

int json_a_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_1da_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_2da_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_3da_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);

int json_aton_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_1daton_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_2daton_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_3daton_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);

int json_l_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_1dl_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_2dl_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
int json_3dl_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);

int json_f_handling(int, struct parameter_description, pnni_611_functions, void*, vars_t*);
#endif
