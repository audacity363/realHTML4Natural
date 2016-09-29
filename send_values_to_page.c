#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "natuser.h"
#include "natni.h"
#include "standard.h"
#include "varhandle.h"
#include "utils.h"
#include "hexdump.h"
#include "var2name.h"

#define GET_INTERFACE_FUNC "nni_get_interface"

int exec_stat = 0;

int shiftInt(int input, int length)
{
    int result;
    switch(length)
    {
        case 1:
            result = input >> 24;
            break;
        case 2:
            result = input >> 16;
            break;
        case 4:

            result = input;
            break;
    }
    return(result);
}

int convert2Bto4BString(char *inbuffer, wchar_t *outbuffer, int length)
{
    int i, offset;

    void *v_in, *v_out, *v_tmp;

    v_in = (void*)inbuffer;
    v_out = (void*)outbuffer;

    for(i=0; i < length; i++)
    {
        offset = (sizeof(wchar_t)*i)+2;

        v_tmp = v_out+offset;

        memcpy(v_tmp, v_in+(i*2), 2);
    }

    return(0);
}

int convert1Bto4BString(char *inbuffer, wchar_t *outbuffer, int length)
{
    int i, offset;

    void *v_in, *v_out, *v_tmp;

    v_in = (void*)inbuffer;
    v_out = (void*)outbuffer;

    for(i=0; i < length; i++)
    {
        offset = (sizeof(wchar_t)*i)+3;

        v_tmp = v_out+offset;

        memcpy(v_tmp, v_in+i, 1);
    }

    return(0);
}

int checkLogicVar(void *buff)
{
    if(memcmp(buff, 0x00, 1) == 0)
    {
        return(0);
    }
    return(1);
}

int OpenLib(void **shLib, char *name)
{
    char *error;

    *shLib = dlopen(name, RTLD_NOW);
    if(!*shLib)
    {
        error = dlerror();
        printf("Error while loading Module [%s]: [%s]\n", name, error);
        return(-1);
    }
    return(0);
}

void CloseLib(void **shLib)
{
    dlclose(*shLib);
}

/**
 * Return: 200: Nicht nur Zahlen im Buffer
 *         201: Memory Allocation schlug fehl
 *         299: Unbekannte Dimension
 *         000: OK
 *
 *
 *
 */
int handleNumericVar(pnni_611_functions sh_funcs, void *parmhandle, int index,
                     struct parameter_description var_info,
                     struct variables *nat_vars)
{
    char *c_tmp_buff, tmp_var_name[100];

    wchar_t *wc_tmp_buff;

    int i, x, y, z,
        nat_index[3] = {0, 0, 0};

    sprintf(tmp_var_name, "tmp%d", index);

    

    if((c_tmp_buff = (char*)malloc(var_info.length)) == NULL)
    {
        return(301);
    }

    if((wc_tmp_buff = (wchar_t*)malloc(var_info.length*sizeof(wchar_t)+4)) == NULL)
    {
        return(302);
    }

    bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);

    if(var_info.dimensions == 0)
    {

        sh_funcs->pf_nni_get_parm(sh_funcs, index, parmhandle, var_info.length_all, c_tmp_buff);
        //logHexDump(c_tmp_buff, var_info.length_all, stdout);

        newStringVar(nat_vars, tmp_var_name, c_tmp_buff);
    }
    else if(var_info.dimensions == 1)
    {

        bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);

        newUStringArray(nat_vars, tmp_var_name, var_info.length, var_info.occurrences[0]);

        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                    var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

            convert1Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);
            editUStringArray(nat_vars, tmp_var_name, wc_tmp_buff, nat_index[0]);

            bzero(c_tmp_buff, var_info.length);
            bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
        }
    }
    else if(var_info.dimensions == 2)
    {

        newU2DStringArray(nat_vars, tmp_var_name, var_info.length,
                          var_info.occurrences[0], var_info.occurrences[1]);

        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                        var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

                convert1Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);
                //editUString2DArray(nat_vars, tmp_var_name, wc_tmp_buff, nat_index[0], nat_index[1]);

                bzero(c_tmp_buff, var_info.length);
                bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
            }
        }
    }
    else if(var_info.dimensions == 3)
    {
    
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                for(nat_index[2]=0; nat_index[2] < var_info.occurrences[2];
                    nat_index[2]++) 
                {

                    sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                            var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

                    convert1Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);

                    bzero(c_tmp_buff, var_info.length);
                    bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
                }
            }
        }
    }
    else
    {
        return(399);
    }

    free(c_tmp_buff);
    free(wc_tmp_buff);

    return(0);

}

/**
 * Return: 301: Memory Allocation schlug fehl
 *         399: Unbekannte Dimension
 *         000: OK
 */
int handleUnicodeVar(pnni_611_functions sh_funcs, void *parmhandle, int index,
                     struct parameter_description var_info,
                     struct variables *nat_vars)
{
    char *c_tmp_buff;

    wchar_t *wc_tmp_buff;

    int i, x, y, z,
        nat_index[3] = {0, 0, 0};

    if((c_tmp_buff = (char*)malloc(var_info.length*2)) == NULL)
    {
        return(399);
    }

    if((wc_tmp_buff = (wchar_t*)malloc((var_info.length*sizeof(wchar_t))+4)) ==NULL)
    {
        free(c_tmp_buff);
        return(399);
    }

    bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);

    if(var_info.dimensions == 0)
    {

        sh_funcs->pf_nni_get_parm(sh_funcs, index, parmhandle, var_info.length_all, c_tmp_buff);

        convert2Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);

    }
    else if(var_info.dimensions == 1)
    {

        bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);

        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                    var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

            convert2Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);

            bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
        }
    }
    else if(var_info.dimensions == 2)
    {
        bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);

        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                        var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

                convert2Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);

                bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
            }
        }
    }
    else if(var_info.dimensions == 3)
    {
    
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                for(nat_index[2]=0; nat_index[2] < var_info.occurrences[2];
                    nat_index[2]++) 
                {

                    sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                            var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

                    convert2Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);

                    bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
                }
            }
        }
    }
    else
    {
        return(399);
    }

    free(c_tmp_buff);
    free(wc_tmp_buff);

    return(0);
}

int handleAlphaNumericVar(pnni_611_functions sh_funcs, void *parmhandle, int index,
                     struct parameter_description var_info,
                     struct variables *nat_vars)
{
    char *c_tmp_buff, tmp_var_name[100];

    wchar_t *wc_tmp_buff;

    int i, x, y, z,
        nat_index[3] = {0, 0, 0};
    int rc;

    sprintf(tmp_var_name, "tmp%d", index);

    if((c_tmp_buff = (char*)malloc(var_info.length)) == NULL)
    {
        return(401);
    }

    if((wc_tmp_buff = (wchar_t*)malloc(var_info.length*sizeof(wchar_t)+4)) == NULL)
    {
        return(302);
    }

    bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);

    if(var_info.dimensions == 0)
    {
        rc = sh_funcs->pf_nni_get_parm(sh_funcs, index, parmhandle, var_info.length, c_tmp_buff);
        switch(rc)
        {
            case NNI_RC_OK:
                printf("NNI_RC_OK\n");
                break;
            case NNI_RC_NOT_INIT:
                printf("NNI_RC_NOT_INIT\n");
                break;
            case NNI_RC_PARM_ERROR:
                printf("NNI_RC_PARM_ERROR\n");
                break;
            case NNI_RC_ILL_PNUM:
                printf("NNI_RC_ILL_PNUM\n");
                break;
            case NNI_RC_DATA_TRUNC:
                printf("NNI_RC_DATA_TRUNC\n");
                break;
        }
        if(rc > 0)
            printf("only [%d] Bytes were read\n", rc);

        logHexDump(c_tmp_buff, var_info.length, stdout);
        convert1Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);
        printf("Add new U Var\n");
        logHexDump(wc_tmp_buff, var_info.length*sizeof(wchar_t)+4, stdout);
        if(newUStringVar(nat_vars, tmp_var_name, wc_tmp_buff, var_info.length) < 0)
        {
            printf("Error in Varhandle: [%s]\n", varhandle_error_str);
        }
    }
    else if(var_info.dimensions == 1)
    {

        bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
        newUStringArray(nat_vars, tmp_var_name, var_info.length+1, var_info.occurrences[0]);

        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                    var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

            convert1Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);
            editUStringArray(nat_vars, tmp_var_name, wc_tmp_buff, nat_index[0]);

            bzero(c_tmp_buff, var_info.length);
            bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
        }
    }
    else if(var_info.dimensions == 2)
    {
        bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);

        //newUString2DArray(nat_vars, tmp_var_name, var_info.length,
        //                  var_info.occurrences[0], var_info.occurrences[1]);

        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                        var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

                convert1Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);
                //editUString2DArray(nat_vars, tmp_var_name, wc_tmp_buff, nat_index[0], nat_index[1]);

                bzero(c_tmp_buff, var_info.length);
                bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
            }
        }
    }
    else if(var_info.dimensions == 3)
    {
    
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                for(nat_index[2]=0; nat_index[2] < var_info.occurrences[2];
                    nat_index[2]++) 
                {

                    sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                            var_info.length*sizeof(wchar_t), (void*)c_tmp_buff, nat_index);

                    convert1Bto4BString(c_tmp_buff, wc_tmp_buff, var_info.length);

                    bzero(c_tmp_buff, var_info.length);
                    bzero(wc_tmp_buff, (var_info.length*sizeof(wchar_t))+4);
                }
            }
        }
    }
    else
    {
        return(499);
    }

    free(c_tmp_buff);
    free(wc_tmp_buff);

    return(0);
}

int handleIntegerVar(pnni_611_functions sh_funcs, void *parmhandle, int index,
                     struct parameter_description var_info,
                     struct variables *nat_vars)
{
    int *p_i_tmp_buff,
        i_tmp_buff;

    int i, x, y, z,
        nat_index[3] = {0, 0, 0};

    char tmp_var_name[100];

    sprintf(tmp_var_name, "tmp%d", index);

    if((p_i_tmp_buff = (int*)malloc(var_info.length)) == NULL)
    {
        return(501);
    }

    if(var_info.dimensions == 0)
    {
        sh_funcs->pf_nni_get_parm(sh_funcs, index, parmhandle, var_info.length, p_i_tmp_buff);
        i_tmp_buff = shiftInt(*p_i_tmp_buff, var_info.length);
        newIntVar(nat_vars, tmp_var_name, i_tmp_buff);
    }
    else if(var_info.dimensions == 1)
    {

        newIntArray(nat_vars, tmp_var_name, var_info.occurrences[0]);
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                    var_info.length, (void*)p_i_tmp_buff, nat_index);
            i_tmp_buff = shiftInt(*p_i_tmp_buff, var_info.length);
            editIntVarArray(nat_vars, tmp_var_name, i_tmp_buff, nat_index[0]);
        }
    }
    else if(var_info.dimensions == 2)
    {
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                        var_info.length, (void*)p_i_tmp_buff, nat_index);
                i_tmp_buff = shiftInt(*p_i_tmp_buff, var_info.length);
            }
        }
    }
    else if(var_info.dimensions == 3)
    {
    
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                for(nat_index[2]=0; nat_index[2] < var_info.occurrences[2];
                    nat_index[2]++) 
                {

                    sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                            var_info.length, (void*)p_i_tmp_buff, nat_index);
                    i_tmp_buff = shiftInt(*p_i_tmp_buff, var_info.length);
                }
            }
        }
    }
    else
    {
        return(599);
    }

    free(p_i_tmp_buff);
    return(0);
}

int handleControlVar(pnni_611_functions sh_funcs, void *parmhandle, int index,
                     struct parameter_description var_info,
                     struct variables *nat_vars)

{
    return(0);
}

int handleLogicVar(pnni_611_functions sh_funcs, void *parmhandle, int index,
                   struct parameter_description var_info,
                   struct variables *nat_vars)

{
    void *v_tmp_buff;
    int i_tmp_buff;

    int i, x, y, z,
        nat_index[3] = {0, 0, 0};

    char tmp_var_name[100];

    sprintf(tmp_var_name, "tmp%d", index);

    if((v_tmp_buff = malloc(var_info.length)) == NULL)
    {
        return(601);
    }

    if(var_info.dimensions == 0)
    {
        sh_funcs->pf_nni_get_parm(sh_funcs, index, parmhandle, var_info.length, v_tmp_buff);
        i_tmp_buff = checkLogicVar(v_tmp_buff);
        newIntVar(nat_vars, tmp_var_name, i_tmp_buff);
    }
    else if(var_info.dimensions == 1)
    {


        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                    var_info.length, v_tmp_buff, nat_index);
            i_tmp_buff = checkLogicVar(v_tmp_buff);
        }
    }
    else if(var_info.dimensions == 2)
    {
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                        var_info.length, v_tmp_buff, nat_index);
                i_tmp_buff = checkLogicVar(v_tmp_buff);
            }
        }
    }
    else if(var_info.dimensions == 3)
    {
    
        for(nat_index[0]=0; nat_index[0] < var_info.occurrences[0];
            nat_index[0]++) 
        {
            for(nat_index[1]=0; nat_index[1] < var_info.occurrences[1];
                nat_index[1]++) 
            {
                for(nat_index[2]=0; nat_index[2] < var_info.occurrences[2];
                    nat_index[2]++) 
                {

                    sh_funcs->pf_nni_get_parm_array(sh_funcs, index, parmhandle,
                            var_info.length, v_tmp_buff, nat_index);
                    i_tmp_buff = checkLogicVar(v_tmp_buff);
                }
            }
        }
    }
    else
    {
        return(699);
    }

    free(v_tmp_buff);

    return(0);
}

int handleBinaryVar(struct parameter_description var_info, void *buffer,
                    struct variables *nat_vars)
{
    return(0);
}


int readVar(void *parmhandle, pnni_611_functions sh_funcs, int index,
            struct variables *nat_vars, FILE *logfile)
{
    struct parameter_description var_info;

    void *buffer;

    int i, x, y, z, x_length, y_length, rc,
        dynamic_flag;

    if(sh_funcs->pf_nni_get_parm_info(sh_funcs, index, parmhandle, &var_info) != NNI_RC_OK)
    {
        return(100);
    }

    printf("Format:     [%c]\n", var_info.format);
    printf("Length:     [%d]\n", var_info.length);
    printf("Length All: [%d]\n", var_info.length_all);
    printf("Dimensions: [%d]\n", var_info.dimensions);
    if(var_info.dimensions > 0)
    {
        for(i=0; i != var_info.dimensions; i++)
        {
            fprintf(logfile, "\tOcc: [%d]\n", var_info.occurrences[i]);
        }

    }

    dynamic_flag = var_info.flags & IF4_FLG_DYNVAR;
    fprintf(logfile, "Dynamic: [%d]\n", dynamic_flag);

    if(var_info.dimensions > 0 && dynamic_flag)
    {
        fprintf(logfile, "Error: Length == 0\n");
        return(101);
    }

    if((buffer = (void*)malloc(var_info.length_all)) == NULL)
    {
        return(102);
    }

    if(sh_funcs->pf_nni_get_parm(sh_funcs, index, parmhandle, var_info.length_all,
      buffer) != NNI_RC_OK)
    {
        return(103);
    }


    switch(var_info.format)
    {
        case 'N':
            if((rc = handleNumericVar(sh_funcs, parmhandle, index, var_info, nat_vars))>0)
                return(rc);
            break;
        case 'A':
            if((rc = handleAlphaNumericVar(sh_funcs, parmhandle, index, var_info, nat_vars))>0)
                return(rc);
            break;
        case 'U':
            if((rc = handleUnicodeVar(sh_funcs, parmhandle, index, var_info, nat_vars))>0)
                return(rc);
            break;
        case 'I':
            if((rc = handleIntegerVar(sh_funcs, parmhandle, index, var_info, nat_vars))>0)
                return(rc);
            break;
        case 'C':
            if((rc = handleControlVar(sh_funcs, parmhandle, index, var_info, nat_vars))>0)
                return(rc);
            break;
        case 'L':
            if((rc = handleLogicVar(sh_funcs, parmhandle, index, var_info, nat_vars))>0)
                return(rc);
            break;
        case 'B':
            if((rc = handleBinaryVar(var_info, buffer, nat_vars))>0)
                return(rc);
            break;
        default:
            fprintf(logfile, "Error: Unkown Variable\n");
    }

    free(buffer);

    fprintf(logfile, "\n\n");
    return(0);
}

long user_exit(WORD nparm, void *parmhandle, void *traditional)
{
    void *shlib = NULL;

    int i, rc;

    char *error;
    void *buffer;
    wchar_t *var_value,
            *tmp_value;

    char ldaname[22], templatename[22], deliver_filename[100],
         *settingsstr;


    FILE *logfile;

    struct variables nat_vars;

    struct parameter_description lda, template, settings, deliver_file;

    PF_NNI_GET_INTERFACE pf_nni_get_interface = NULL;
    pnni_611_functions s_funcs;

    setlocale(LC_ALL, "");

    nat_vars.next = NULL;

    if((logfile = fopen("/tmp/a140734_logfile.log", "w")) == NULL)
    {
        return(1);
    }

    if(OpenLib(&shlib, "libnatural.so") < 0)
    {
        fprintf(logfile, "Error loading so\n");
        fclose(logfile);
        return(2);
    }

    pf_nni_get_interface = (PF_NNI_GET_INTERFACE)dlsym(shlib,
        GET_INTERFACE_FUNC);

    if(!pf_nni_get_interface)
    {
        error = dlerror();
        fprintf(logfile, "Error while loading Function [%s]: [%s]\n", GET_INTERFACE_FUNC, error);
        CloseLib(&shlib);
        fclose(logfile);
        return(3);
    }


    if(((pf_nni_get_interface)(NNI_VERSION_CURR, (void**)&s_funcs)) != NNI_RC_OK)
    {
        fprintf(logfile, "...Error while gettings Function Table\n");
        CloseLib(&shlib);
        fclose(logfile);
        return(4);
    }


    /*Start LDA*/
    if(s_funcs->pf_nni_get_parm_info(s_funcs, 0, parmhandle, &lda) != NNI_RC_OK)
    {
        return(100);
    }

    if(lda.format != 'A' || lda.length_all != 20 || lda.dimensions != 0)
    {
        printf("LDA Formatierung stimmt nicht\n");
        return((long)4);
    }
    s_funcs->pf_nni_get_parm(s_funcs, 0, parmhandle, 20, ldaname);
    ldaname[21] = '\0';
    /*End LDA*/

    /*Start template*/
    if(s_funcs->pf_nni_get_parm_info(s_funcs, 1, parmhandle, &template) != NNI_RC_OK)
    {
        return(100);
    }

    if(template.format != 'A' || template.length_all != 20 || template.dimensions != 0)
    {
        printf("Template Formatierung stimmt nicht\n");
        return((long)10);
    }
    s_funcs->pf_nni_get_parm(s_funcs, 1, parmhandle, 20, templatename);
    /*End Template*/

    /*Start deliver File*/
    if(s_funcs->pf_nni_get_parm_info(s_funcs, 2, parmhandle, &deliver_file) != NNI_RC_OK)
    {
        return(100);
    }

    if(deliver_file.format != 'A' || deliver_file.length_all != 100 || deliver_file.dimensions != 0)
    {
        return((long)17);
    }
    s_funcs->pf_nni_get_parm(s_funcs, 2, parmhandle, 100, deliver_filename);
    /*End deliver File*/

    /*Start Settingsstr*/
    if(s_funcs->pf_nni_get_parm_info(s_funcs, 3, parmhandle, &settings) != NNI_RC_OK)
    {
        return(100);
    }

    settingsstr = malloc(settings.length_all);

    s_funcs->pf_nni_get_parm(s_funcs, 3, parmhandle, settings.length_all, settingsstr);
    /*End Settingsstr*/
    printf("Parms: [%d]\n", nparm);

    printf("LDA:      [%s]\n", ldaname);
    printf("Template: [%s]\n", templatename);
    printf("Filename: [%s]\n", settingsstr);

    for(i=4; i < nparm; i++)
    {
        if((rc = readVar(parmhandle, s_funcs, i, &nat_vars, logfile))>0)
        {
            CloseLib(&shlib);
            fclose(logfile);
            return((long)rc);
        }
    }
    

    printVars(&nat_vars);

    char error_str[1024];
    strcpy(webserver_settings.templatepath, "/u/it/a140734/C/realHtml4Natural/web_server/templates/");
    strcpy(webserver_settings.nat_sourcepath, "/VAW/natural/logi/fuser63/");
    strcpy(webserver_settings.natlibrary, "TGAP0734");

    generate_page(&nat_vars, ldaname, templatename, deliver_filename);


    /*if(var2name(&nat_vars, "/VAW/natural/logi/fuser63/TGAP0734/SRC/LSHUS.NSL", error_str) < 0)
    {
        printf("Error: [%s]\n", error_str);
    }
    else
    {
        printVars(&nat_vars);
    }*/
    CloseLib(&shlib);
    fclose(logfile);


    return((long)0);

}
