#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <sys/ioctl.h>

#define VAR2NAME

#include "vars.h"
#include "error_vars.h"
#include "lda.h"
#include "rh4n.h"
#include "rh4n_utils.h"

#define NAME "Name: "
#define MAX_VARNAME_LENGTH 32

#define BOX_WIDTH (strlen(NAME)+MAX_VARNAME_LENGTH+4)

int main()
{
    vars_t *lda_anker = NULL, *nat_anker = NULL;
    int ret;

    char error_buff[2024];

    initVarAnker(&nat_anker);
    initVarAnker(&lda_anker);

    addString(nat_anker, NULL, "var1", L"val1", 5);
    addString(nat_anker, NULL, "var2", L"val2", 5);
    addString(nat_anker, NULL, "var3", L"val3", 5);
    addString(nat_anker, NULL, "var4", L"val4", 5);

    if((ret = startLDAParser("./TEST.NGL", lda_anker, NULL, error_buff)) != LDA_OK)
    {
        printf("Something went wrong [%s]\n", error_buff);
        return(-1);
    }

    printfork(lda_anker, stdout);

    p_print(lda_anker, nat_anker, NULL, NULL);

    return(0);
}

