#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh4n.h"
#include "rh4n_ldaparser.h"
#include "rh4n_v2n.h"

int main(int argc, char *argv[]) {
    RH4nVarList varlist;
    RH4nProperties props;
    RH4nLDAEntry_t *entries = NULL;
    char errorstr[2048];
    int arrlength[3] = {2, 0, 0};

    props.logging = rh4nLoggingCreateStreamingRule("VARLIB", "TEST", RH4N_DEVELOP, "");

    rh4nvarInitList(&varlist);
/*
 * 1 str1 (A) DYNAMIC
 * 1 str2 (A) DYNAMIC
 * 1 str3 (A) DYNAMIC
 * 1 str4 (A) DYNAMIC
 */
#if 0
    rh4nvarCreateNewString(&varlist, NULL, "str1", "val1");
    rh4nvarCreateNewString(&varlist, NULL, "str2", "val2");
    rh4nvarCreateNewString(&varlist, NULL, "str3", "val3");
    rh4nvarCreateNewString(&varlist, NULL, "str4", "val3");
#endif

/*
 * 1 grp1 (1:2)  
 *   2 str1 (A) DYNAMIC
 *   2 str2 (A) DYNAMIC
 *   2 str3 (A) DYNAMIC
 *   2 str4 (A) DYNAMIC
 *
 *   Expected Output:
 *      {"grp1":[{"str1":"","str2":"","str3":"","str4":""},{"str1":"","str2":"","str3":"","str4":""}]}
 */
#if 0
    rh4nvarCreateNewGroup(&varlist, "grp1");
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str1", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str3", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str4", 1, arrlength);
#endif

/*
 * 1 grp1 (1:2)  
 *   2 str1 (A) DYNAMIC
 *   2 str2 (A) DYNAMIC
 *   2 str3 (A) DYNAMIC
 *   2 str4 (A) DYNAMIC
 * 1 str5 (A) DYNAMIC
 *
 *   Expected Output:
 *      {"grp1": [{"str1": "", "str2": "", "str3": "","str4": ""},
 *      {"str1": "", "str2": "", "str3": "","str4": ""}],
 *      "str5": ["", ""]}
 */
#if 0
    rh4nvarCreateNewGroup(&varlist, "grp1");
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str1", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str3", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str4", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, NULL, "str5", 1, arrlength);
#endif

/*
 * 1 str1 (A/1:2) DYNAMIC
 * 1 str2 (A/1:2) DYNAMIC
 * 1 str3 (A/1:2) DYNAMIC
 * 1 str4 (A/1:2) DYNAMIC
 * 1 str5 (A/1:2) DYNAMIC
 *
 *  Expected Output:
 *      [{"str1":"","str2":"","str3":"","str4":"","str5":""},{"str1":"","str2":"","str3":"","str4":"","str5":""}]
 */
#if 1
    rh4nvarCreateNewStringArray(&varlist, NULL, "str1", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, NULL, "str2", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, NULL, "str3", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, NULL, "str4", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, NULL, "str5", 1, arrlength);
#endif

/* 1 grp1
 *   2 str1 (A/1:2) DYNAMIC
 *   2 str2 (A/1:2) DYNAMIC
 *   2 str3 (A/1:2) DYNAMIC
 *   2 str4 (A/1:2) DYNAMIC
 *   2 str5 (A) DYNAMIC
 *
 *   Expected Output:
 *      {"grp1":{"str2":["",""],"str3":["",""],"str4":["",""],"str5":["",""],"str1":"str1"}}
 */
#if 0
    rh4nvarCreateNewGroup(&varlist, "grp1");
    //rh4nvarCreateNewString(&varlist, "grp1", "str1", "str1");
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str3", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str4", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str5", 1, arrlength);
    rh4nvarCreateNewString(&varlist, "grp1", "str1", "str1");
#endif

/* 1 grp1
 *   2 str1 (A/1:2) DYNAMIC
 *   2 str2 (A/1:2) DYNAMIC
 *   2 grp2
 *     3 str3 (A/1:2) DYNAMIC
 *     3 str4 (A) DYNAMIC
 *   2 str5 (A/1:2) DYNAMIC
 *
 *   Expected Output:
 *      {"grp1":{"str1":["",""],"str2":["",""],"grp2":{"str3":["",""],"str5":""},"str4":["",""]}
 */
#if 0
    char *parents[] = { "grp1", NULL, NULL };
    rh4nvarCreateNewGroup(&varlist, "grp1");

    rh4nvarCreateNewString(&varlist, "grp1", "str1", "str1");
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);

    rh4nvarCreateNewGroup_m(&varlist, parents, "grp2");

    parents[1] = "grp2";
    rh4nvarCreateNewStringArray(&varlist, NULL, "str3", 1, arrlength);
    rh4nvarMoveVarToGroup_m(&varlist, "str3", parents);
    rh4nvarCreateNewString(&varlist, NULL, "str4", "str4");
    rh4nvarMoveVarToGroup_m(&varlist, "str4", parents);

    rh4nvarCreateNewStringArray(&varlist, "grp1", "str5", 1, arrlength);
#endif

/* 1 grp1 (1:2)
 *   2 str1 (A) DYNAMIC
 *   2 str2 (A) DYNAMIC
 *   2 grp2
 *     3 str3 (A/1:2) DYNAMIC
 *     3 str4 (A) DYNAMIC
 *   2 str5 (A) DYNAMIC
 *
 *   Expected Output:
 *   {"grp1":[{"str1":"","str2":"","grp2":{"str3":["",""],"str4":""},"str5":""},{"str1":"","str2":"","grp2":{"str3":["",""],"str4":""},"str5":""}]}
 */
#if 0
    char *parents[] = { "grp1", NULL, NULL };
    rh4nvarCreateNewGroup(&varlist, "grp1");

    rh4nvarCreateNewStringArray(&varlist, "grp1", "str1", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);

    rh4nvarCreateNewGroup_m(&varlist, parents, "grp2");

    parents[1] = "grp2";
    arrlength[1] = 2;
    rh4nvarCreateNewStringArray(&varlist, NULL, "str3", 2, arrlength);
    arrlength[1] = 0;
    rh4nvarMoveVarToGroup_m(&varlist, "str3", parents);
    rh4nvarCreateNewStringArray(&varlist, NULL, "str4", 1, arrlength);
    rh4nvarMoveVarToGroup_m(&varlist, "str4", parents);

    rh4nvarCreateNewStringArray(&varlist, "grp1", "str5", 1, arrlength);
#endif

/*
 * 1 grp1 (1:2)  
 *   2 str1 (A) DYNAMIC
 *   2 str2 (A) DYNAMIC
 *   2 str3 (A) DYNAMIC
 *   2 str4 (A) DYNAMIC
 * 1 grp2
 *   2 str5 (A) DYNAMIC
 *
 *   Expected Output:
 *      {"grp1": [{"str1": "", "str2": "", "str3": "","str4": ""},
 *      {"str1": "", "str2": "", "str3": "","str4": ""}],
 *      "str5": ["", ""]}
 */
#if 0
    rh4nvarCreateNewGroup(&varlist, "grp1");
    rh4nvarCreateNewGroup(&varlist, "grp2");
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str1", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str3", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str4", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp2", "str5", 1, arrlength);
#endif

/*
 * 1 grp1 (1:2)  
 *   2 str1 (A) DYNAMIC
 *   2 str2 (A) DYNAMIC
 *   2 str3 (A) DYNAMIC
 *   2 str4 (A) DYNAMIC
 *
 *   Expected Output:
 *      {"grp1": [{"str1": "", "str2": "", "str3": "","str4": ""},
 *      {"str1": "", "str2": "", "str3": "","str4": ""}],
 *      "str5": ["", ""]}
 */
#if 0
    rh4nvarCreateNewGroup(&varlist, "grp1");
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str1", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str2", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str3", 1, arrlength);
    rh4nvarCreateNewStringArray(&varlist, "grp1", "str4", 1, arrlength);
    rh4nvarCreateNewString(&varlist, "grp1", "str5", "str5");
#endif



    rh4nvarPrintList(&varlist, &props);
    rh4njsonPrintJSON(&varlist, stderr, &props);

    rh4nvarFreeList(&varlist);
    rh4n_del_log_rule(props.logging);
    return(0);
}
