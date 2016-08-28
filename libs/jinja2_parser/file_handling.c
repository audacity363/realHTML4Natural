#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "file_handling.h"
/**
 * @file file_handling.c
 * @brief Bearbeitet alles was mit Dateien zu tun hat
 */

/**
 * @brief Oeffnet ein file im schreib Modus
 *
 * @param filename Pfad zum file
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Erfolg: ein offenes Filehandle; Error: NULL
 */
FILE *openOutputFile(char *filename, char *error_str)
{
    FILE *p_output;

    if((p_output = fopen(filename, "w")) == NULL)
    {
        sprintf(error_str, "Can not open outputfile: [%s] because [%s]",
                filename, strerror(errno));
        return(NULL);
    }
    return(p_output);
}

/**
 * @brief Schließt ein welches im schreib Modus geoeffnet ist
 *
 * @param p_output offener Filepointer
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Erfolg: 0; Error: < 0
 */
int closeOutputFile(FILE *p_output, char *error_str)
{
    if(fflush(p_output) != 0)
    {
        sprintf(error_str, "Error while flushing outputfile: [%s]",
            strerror(errno));
        return(-1);
    }

    if(fclose(p_output) != 0)
    {
        sprintf(error_str, "Error while closing outputfile: [%s]",
            strerror(errno));
        return(-2);
    }
    return(0);
}

/**
 * @brief Oeffnet ein file im lese Modus
 *
 * @param filename Pfad zum file
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Erfolg: ein offenes Filehandle; Error: NULL
 */
FILE *openTemplateFile(char *filename, char *error_str)
{
    FILE *p_template;

    if((p_template = fopen(filename, "r")) == NULL)
    {
        sprintf(error_str, "Can not open Template: [%s] because [%s]",
                filename, strerror(errno));
        return(NULL);
    }
    return(p_template);
}

/**
 * @brief ließt eine Zeile vom File \p p_template
 *
 * @param p_template Offenes Filepointer
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Erfolg: ein C-String; Error: NULL
 */
char *getLinefromTemplate(FILE *p_template, char *error_str)
{
    static char line_buffer[LINELENGTH];

    if(fgets(line_buffer, sizeof(line_buffer), p_template) == NULL)
    {
        sprintf(error_str, "Error while reading templatefile [%s]",
                strerror(errno));
        return(NULL);
    }
    return(line_buffer);
}

/**
 * @brief Schließt ein welches im lese Modus geoeffnet ist
 *
 * @param p_output offener Filepointer
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return Erfolg: 0; Error: < 0
 */
int  closeTemplateFile(FILE *p_template, char *error_str)
{
    if(fclose(p_template) != 0)
    {
        sprintf(error_str, "Error while closing template: [%s]",
            strerror(errno));
        return(-1);
    }
    return(0);
}
