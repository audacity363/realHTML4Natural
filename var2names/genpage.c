#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "lda_parser.h"
#include "utils.h"

extern int parser_start(char *, struct variables*, char*, char*, int*);

int generate_500errPage_parser(char *output, struct variables *anker, 
    char *error_str, int line, char *template)
{
    char *c_line;
    char *parser_template = malloc(strlen(webserver_settings.templatepath)+19);

    //"/home/tom/Documents/realHtml4Natural/web_server/templates/error/500_parser.html";
    sprintf(parser_template, "%s/error/500_parser.html",
        webserver_settings.templatepath);
    newStringVar(anker, "error_str", error_str);

    c_line = getLineFromFile(template, line);
    newStringVar(anker, "error_line", c_line);
    newStringVar(anker, "error_template", template);
    newIntVar(anker, "error_line_nr", line);

    parser_start(parser_template, anker, output, error_str, &line);
    return(0);
}

int generate_500errPage_lda(char *output, char *error_str, char *ldaname)
{
    struct variables *anker = malloc(sizeof(struct variables));
    char *lda_template = malloc(strlen(webserver_settings.templatepath)+19);
    int error_line;

    sprintf(lda_template, "%s/error/500_lda.html",
        webserver_settings.templatepath);

    newStringVar(anker, "error_str", error_str);
    newStringVar(anker, "lda_name", ldaname);

    parser_start(lda_template, anker, output, error_str, &error_line);
    return(0);
}

int generate_page(struct variables *anker, char *ldaname, char *templatename,
                  char *output_file)
{
    char *template = malloc(sizeof(webserver_settings.templatepath)
                            +strlen(templatename)+1);
    char *lda = malloc(strlen(webserver_settings.nat_sourcepath)
                            +strlen(webserver_settings.natlibrary)
                                    +strlen(ldaname)+10);
    char error_str[2048];
    int error_zeile;

    sprintf(template, "%s/%s", webserver_settings.templatepath, templatename);
    sprintf(lda, "%s/%s/SRC/%s.NSL", webserver_settings.nat_sourcepath,
                            webserver_settings.natlibrary, StripTrailingSpaces(ldaname));
    strcpy(lda, ldaname);

#ifdef DEBUG
    printf("Working with Template: [%s]\n", template);
    printf("Working with LDA     : [%s]\n", lda);
#endif

    /*
     * TODO: Get ReturnValues from these functions ans redner custom 
     * HTTP 500 Error pages
     */
    if(var2name(anker, lda, error_str) < 0)
    {
        printf("%s\n", error_str);
        return(-1);
    }
    if(parser_start(template, anker, output_file, error_str, &error_zeile) < 0)
    {
        printf("Error in zeile %d [%s]\n", error_zeile, error_str);
        generate_500errPage_parser(output_file, anker, error_str, error_zeile, 
            template);
        return(-1);
    }
    free(template);

}

