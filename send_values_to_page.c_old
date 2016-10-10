#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#include "natuser.h"
#include "standard.h"
#include "varhandle.h"
#include "hexdump.h"
#include "utils.h"

#define UNIT_SEPERATOR 0x1F

void signalHandler(int signal);
int exec_stat = 0;

struct variables *gotoendofAnker(struct variables *anker)
{
    struct variables *ptr;

    ptr = anker;
    exec_stat = 500;
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    exec_stat = 600;

    return ptr;
}

int calculateBufferSize(struct variables *anker)
{
    int size = 0;

    struct variables *hptr = anker->next;
    while(hptr != NULL)
    {
        size += 48+hptr->full_length;
        hptr = hptr->next;
    }
    size += 12;

    return(size);
}

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

int getVar(void *parmhandle, int index, struct variables *nat_vars, int *complete_size)
{
    struct parameter_description var_info;
    exec_stat = 200;
    int ret;
	int dynamic_flag, protected_flag, x_array_flag,
        dimensions, x_length, y_length,
        var_type;

    int *indexes;
    int i, x, offset, i_tmpdata;
    char *data, *tmpdata, namebuff[20];

    if((ret = ncxr_get_parm_info(index, parmhandle, &var_info)) < 0)
    {
        switch(ret)
        {
            case -1:
                return(16);
            case -2:
                return(17);
            case -7:
                return(18);
        }
    }

    exec_stat = 300;
	dynamic_flag = var_info.flags & IF4_FLG_DYNVAR;
	protected_flag = var_info.flags & IF4_FLG_PROTECTED;
    x_array_flag = var_info.flags & IF4_FLG_XARRAY;

    printf("Format:     [%c]\n", var_info.format);
    printf("Length:     [%d]\n", var_info.length);
    printf("Length All: [%d]\n", var_info.length_all);
    printf("Dimensions: [%d]\n", var_info.dimensions);

    data = malloc(var_info.length_all);

    if(ncxr_get_parm(index, parmhandle, var_info.length_all, data) < 0)
    {
        printf("Error while read variable value\n");
        return(20);
    }

    if((var_info.format == 'A' || var_info.format == 'N') && var_info.dimensions == 0)
    {
        exec_stat = 450;
        sprintf(namebuff, "var%d", index);
        data = StripTrailingSpaces(data);
        newStringVar(nat_vars, namebuff, data);
    }
    else if((var_info.format == 'A' || var_info.format == 'N') && var_info.dimensions == 1)
    {
        exec_stat = 500;
        tmpdata = malloc(var_info.length+1);

        memcpy(tmpdata, data, var_info.length);
        tmpdata[var_info.length+1] = '\0';
        tmpdata = StripTrailingSpaces(tmpdata);

        sprintf(namebuff, "var%d", index);

        newStringArray(nat_vars, namebuff, tmpdata);
        for(i=1; i < var_info.occurrences[0]; i++)
        {
            bzero(tmpdata, var_info.length+1);
            memcpy(tmpdata, data+(i*var_info.length), var_info.length);
            tmpdata[var_info.length+1] = '\0';
            HexDump(tmpdata, var_info.length);
            tmpdata = StripTrailingSpaces(tmpdata);
            appendStringArray(nat_vars, namebuff, tmpdata);
        }
    }
    else if((var_info.format == 'A' || var_info.format == 'N') && var_info.dimensions == 2)
    {
        exec_stat = 600;
        tmpdata = malloc(var_info.length);
        //logHexDump(data, var_info.length_all, stdout);
        
        sprintf(namebuff, "var%d", index);

        memcpy(tmpdata, data, var_info.length);
        tmpdata = StripTrailingSpaces(tmpdata);

        new2DStringArray(nat_vars, namebuff, var_info.occurrences[0], var_info.occurrences[1]);
        for(i=0; i < var_info.occurrences[0]; i++)
        {
            for(x=0; x < var_info.occurrences[1]; x++)
            {
                offset = ((var_info.occurrences[1]*var_info.length)*i)+(var_info.length*x);
                printf("Offset [%d]Bytes\n", offset);
                memcpy(tmpdata, data+offset, var_info.length);
                tmpdata = StripTrailingSpaces(tmpdata);
                editStringVar2DArray(nat_vars, namebuff, tmpdata, i, x);
            }
        }
    }
    else if(var_info.format == 'I' && var_info.dimensions == 0)
    {
        sprintf(namebuff, "var%d", index);
        memcpy(&i_tmpdata, data, var_info.length);
        i_tmpdata = shiftInt(i_tmpdata, var_info.length);
        newIntVar(nat_vars, namebuff, i_tmpdata);
    }
    else if(var_info.format == 'I' && var_info.dimensions == 1)
    {
        sprintf(namebuff, "var%d", index);

        memcpy(&i_tmpdata, data, var_info.length);
        i_tmpdata = shiftInt(i_tmpdata, var_info.length);
        newIntArray(nat_vars, namebuff, i_tmpdata);

        for(i=1; i < var_info.occurrences[0]; i++)
        {
            memcpy(&i_tmpdata, data+(i*var_info.length), var_info.length);
            i_tmpdata = shiftInt(i_tmpdata, var_info.length);
            appendIntArray(nat_vars, namebuff, i_tmpdata);
        }
    }
    else if(var_info.format == 'I' && var_info.dimensions == 2)
    {
        sprintf(namebuff, "var%d", index);
        new2DIntArray(nat_vars, namebuff, var_info.occurrences[0], var_info.occurrences[1]);

        for(i=0; i < var_info.occurrences[0]; i++)
        {
            for(x=0; x < var_info.occurrences[1]; x++)
            {
                offset = ((var_info.occurrences[1]*var_info.length)*i)+(var_info.length*x);
                memcpy(&i_tmpdata, data+offset, var_info.length);
                i_tmpdata = shiftInt(i_tmpdata, var_info.length);
                editIntVar2DArray(nat_vars, namebuff, i_tmpdata, i, x);
            }
        }
    }


    *complete_size += var_info.length_all;

	printf("\n");

    return(0);
}

long print_all_vars(WORD nparm, void *parmhandle, void *traditional)
{
    int i, ret;
    void *value;

    FILE *logfile;
    int old_stdout;

    char ldaname[22];
    char templatename[22];
    char deliver_filename[100];
    char *settingsstr;

    int complete_size = 0;
    struct parameter_description lda, template, settings, deliver_file;

    logfile = fopen("/tmp/gen_page.log", "w");

    old_stdout = dup(1);
    dup2(fileno(logfile), 1);
    close(fileno(logfile));

    signal(SIGILL, signalHandler);
    signal(SIGSEGV, signalHandler);

    exec_stat = 1;

    struct variables *nat_vars = malloc(sizeof(struct variables));
    strcpy(nat_vars->name, "anker");
    nat_vars->next = NULL;

    complete_size = sizeof(struct variables);

    if(traditional != NULL)
        return((long)14);

    exec_stat = 2;

    printf("NPARM: [%hu]\n", nparm);
    exec_stat = 3;

    /*Read LDA Name Information*/
    switch(ncxr_get_parm_info(0, parmhandle, &lda))
    {
        case -1:
            printf("Error while reading lda infos\n");
            return((long)1);
        case -2:
            printf("Error while reading lda infos\n");
            return((long)2);
        case -7:
            printf("Error while reading lda infos\n");
            return((long)3);
    }

    if(lda.format != 'A' || lda.length_all != 20 || lda.dimensions != 0)
    {
        printf("LDA Formatierung stimmt nicht\n");
        return((long)4);
    }


    switch(ncxr_get_parm(0, parmhandle, 20, ldaname))
    {
        case -1:
            printf("Error while reading ldaname\n");
            return((long)4);
        case -2:
            printf("Error while reading ldaname\n");
            return((long)5);
        case -3:
            printf("Error while reading ldaname\n");
            return((long)6);
    }

    /*Read Template Name Information*/
    switch(ncxr_get_parm_info(1, parmhandle, &template))
    {
        case -1:
            printf("Error while reading template infos\n");
            return((long)7);
        case -2:
            printf("Error while reading template infos\n");
            return((long)8);
        case -7:
            printf("Error while reading template infos\n");
            return((long)9);
    }

    if(template.format != 'A' || template.length_all != 20 || template.dimensions != 0)
    {
        printf("Template Formatierung stimmt nicht\n");
        return((long)10);
    }

    switch(ncxr_get_parm(1, parmhandle, 20, templatename))
    {
        case -1:
            return((long)11);
        case -2:
            return((long)12);
        case -3:
            return((long)13);
    }

    /*Read deliver File name*/
    switch(ncxr_get_parm_info(2, parmhandle, &deliver_file))
    {
        case -1:
            return((long)14);
        case -2:
            return((long)15);
        case -7:
            return((long)16);
    }

    if(deliver_file.format != 'A' || deliver_file.length_all != 100 || deliver_file.dimensions != 0)
    {
        return((long)17);
    }

    switch(ncxr_get_parm(2, parmhandle, 100, deliver_filename))
    {
        case -1:
            return((long)18);
        case -2:
            return((long)19);
        case -3:
            return((long)20);
    }
    /*Read settings str*/
    switch(ncxr_get_parm_info(3, parmhandle, &settings))
    {
        case -1:
            return((long)14);
        case -2:
            return((long)15);
        case -7:
            return((long)16);
    }

    if(settings.format != 'A' || settings.dimensions != 0)
    {
        return((long)17);
    }

    settingsstr = malloc(settings.length_all);

    switch(ncxr_get_parm(3, parmhandle, settings.length_all, settingsstr))
    {
        case -1:
            return((long)18);
        case -2:
            return((long)19);
        case -3:
            return((long)20);
    }

    printf("LDA: [%s]\n", ldaname);
    printf("Template: [%s]\n", StripTrailingSpaces(templatename));
    printf("DeliverFile: [%s]\n", deliver_filename);
    printf("Settingstr: [%s]\n", settingsstr);

    for(i=4; i < nparm; i++)
    {
        printf("I: [%d]\n", i);
        exec_stat = 4 + i;
        complete_size += sizeof(struct variables);
        exec_stat = 500 + i;
        if((ret = getVar(parmhandle, i, nat_vars, &complete_size)) > 0)
        {
            printf("Error at i=[%d]\n", i);
            return((long)ret);
        }
    }

    printf("\n\n\n\n");
    //printFullAnker(nat_vars, stdout);
    printf("Complete size: [%d]\n", complete_size);

    //SendVarsToSocket(fp, nat_vars, complete_size);
    printf("Call generate_page\n");
    generate_page(nat_vars, ldaname, templatename, deliver_filename);
    printf("generate_page finished\n");

    dup2(old_stdout, 1);
    close(old_stdout);

    return((long)0);

}

void signalHandler(int signal)
{
    FILE *error_file;

    error_file = fopen("/tmp/engemann_nat_error", "w");
    if(signal == SIGILL)
    {
        fprintf(error_file, "SIGILL\n");
        fprintf(error_file, "execstat : [%d]\n", exec_stat);
    }
    else if(signal == SIGSEGV)
    {
        fprintf(error_file, "SIGSEGV\n");
        fprintf(error_file, "execstat : [%d]\n", exec_stat);
    }
    else
    {
        fprintf(error_file, "Unkown");
    }
    fclose(error_file);
    return;
}
