#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

#include "natuser.h"
#include "natni.h"

#define GET_INTERFACE_FUNC "nni_get_interface"

long flipBuffer(WORD nparm, void *parmhandle, void *traditional)
{
    void *shlib = NULL;

    int i, x;

    char *error;
    char buffer_one[29], buffer_two[29];

    struct parameter_description pd_buffer_one, pd_buffer_two;

    PF_NNI_GET_INTERFACE pf_nni_get_interface = NULL;
    pnni_611_functions s_funcs;

    if(OpenLib(&shlib, "libnatural.so") < 0)
    {
        return(2);
    }

    pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(shlib,
        GET_INTERFACE_FUNC);

    if(!pf_nni_get_interface)
    {
        CloseLib(&shlib);
        return(3);
    }


    if(((pf_nni_get_interface)(NNI_VERSION_CURR, (void**)&s_funcs)) != NNI_RC_OK)
    {
        CloseLib(&shlib);
        return(4);
    }

    memset(buffer_two, 0x30, sizeof(buffer_two));


    s_funcs->pf_nni_get_parm_info(s_funcs, 0, parmhandle, &pd_buffer_one);
    s_funcs->pf_nni_get_parm_info(s_funcs, 1, parmhandle, &pd_buffer_two);

    s_funcs->pf_nni_get_parm(s_funcs, 0, parmhandle, pd_buffer_one.length_all, buffer_one);

    i=0;
    while((buffer_one[i] != '0' && buffer_one[i] != 0 && buffer_one[i] != ' ') && i < pd_buffer_two.length_all) i++;

    printf("------------------------------------------------------------------------\n");

    logHexDump(buffer_one, sizeof(buffer_one), stdout);
    logHexDump(buffer_two, sizeof(buffer_two), stdout);


    memcpy(buffer_two+((pd_buffer_two.length_all-i)), buffer_one, i);

    logHexDump(buffer_two, sizeof(buffer_two), stdout);
    printf("------------------------------------------------------------------------\n");

    s_funcs->pf_nni_put_parm(s_funcs, 1, parmhandle, pd_buffer_two.length_all, buffer_two);

    return 0;

}
