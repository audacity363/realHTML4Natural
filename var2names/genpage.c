#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "standard.h"
#include "varhandle.h"
#include "lda_parser.h"
#include "jinja_parser.h"
#include "utils.h"


int generate_500errPage_parser(char *output, struct variables *anker, 
    char *error_str, int line, char *template)
{
    char *c_line;
    char *parser_template = malloc(strlen(webserver_settings.templatepath)+19);

    sprintf(parser_template, "%s/error/500_parser.html",
        webserver_settings.templatepath);
    newStringVar(anker, "error_str", error_str);

    c_line = getLineFromFile(template, line);
    newStringVar(anker, "error_line", c_line);
    newStringVar(anker, "error_template", template);
    newIntVar(anker, "error_line_nr", line);

    start_jinjaparser(anker, output, parser_template, error_str, &line);
    return(0);
}

int generate_500errPage_lda(char *output, char *error_str, char *ldaname)
{
    struct variables *anker = malloc(sizeof(struct variables));
    char *lda_template = malloc(strlen(webserver_settings.templatepath)+19);
    int error_line;

    anker->next = NULL;
    strcpy(anker->name, "anker");

    sprintf(lda_template, "%s/error/500_lda.html",
        webserver_settings.templatepath);

    printf("working with template: [%s]\n", lda_template);

    newStringVar(anker, "error_str", error_str);
    newStringVar(anker, "lda_name", ldaname);

    start_jinjaparser(anker,output, lda_template, error_str, &error_line);
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

    printf("ldaname : [%s]\n", ldaname);

    sprintf(template, "%s/%s", webserver_settings.templatepath, StripTrailingSpaces(templatename));
    sprintf(lda, "%s/%s/SRC/%s.NSL", webserver_settings.nat_sourcepath,
                            webserver_settings.natlibrary, StripTrailingSpaces(ldaname));

#ifdef DEBUG
    printf("Working with Template: [%s]\n", template);
    printf("Working with LDA     : [%s]\n", lda);
#endif

    /*
     * TODO: Get ReturnValues from these functions ans redner custom 
     * HTTP 500 Error pages
     */
    printf("Start var2name\n");
    if(var2name(anker, lda, error_str) < 0)
    {
        printf("%s\n", error_str);
        generate_500errPage_lda(output_file, error_str, lda);
        return(-1);
    }
    printf("Start jinja Parser\n");
    if(start_jinjaparser(anker, output_file, template, error_str, &error_zeile) < 0)
    {
        printf("Error in zeile %d [%s]\n", error_zeile, error_str);
        generate_500errPage_parser(output_file, anker, error_str, error_zeile, 
            template);
        return(-1);
    }
    free(template);
    return(0);
}

