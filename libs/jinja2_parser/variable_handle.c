#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "standard.h"
#include "varhandle.h"
#include "file_handling.h"
#include "utils.h"

/**
 * @file variable_handle.c
 * @brief Parst den Variablen Befehl und gibt diese aus
 */

/**
 * @brief Parst den Index aus dem Variablen Namen
 *
 * @param anker Anker Punkt fuer Variablen
 * @param variable Buffer welcher die Variable beinhaltet
 * @param x Buffer in dender X Index geschrieben wird
 * @param y Buffer in dender y Index geschrieben wird
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return 0: kein Index gefunden; 1: X Index gefunden;
 *         2: X und Y Index gefunden; <0: Error
 */
int getIndex(struct variables *anker, char *variable, int *x, int *y,
             char *error_str)
{
    char *index_buff;
    char *open_bracked, *close_bracked;

    if((open_bracked = strchr(variable, '[')) == NULL)
    {
        //Kein Index angeben
        return(0);
    }
    if((index_buff = malloc(strlen(open_bracked))) == NULL)
    {
        sprintf(error_str, "Error while allocating memory: [%s]", strerror(errno));
        return(-1);
    }
    strcpy(index_buff, open_bracked);
    open_bracked[0] = '\0';

    memmove(index_buff, index_buff+1, strlen(index_buff));

    if((close_bracked = strchr(index_buff, ']')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"]\"");
        free(index_buff);
        return(-2);
    }
    close_bracked[0] = '\0';

    if(strcmp(index_buff, "loop.i") == 0)
    {
        if((*x = getIntValue(anker, "loop.i")) < 0)
        {
            strcpy(error_str, "Unkown Variable [loop.i]");
            free(index_buff);
            return(-3);
        }
        return(1);
    }
    else
    {
        if(str_isdigit(index_buff) < 0)
        {
            strcpy(error_str, "Syntax Error: Index is not a number");
            free(index_buff);
            return(-4);
        }
        *x = atoi(index_buff);
    }

    memmove(index_buff, index_buff+(strlen(index_buff)+1), strlen(close_bracked+1)+1);

    //Search for Y Index
    if(strlen(index_buff) == 0)
    {
        //Kein Y Index vorhanden
        free(index_buff);
        return(1);
    }

    if(index_buff[0] != '[')
    {
        sprintf(error_str, "Syntax Error: Unexpected Token: [%s]", index_buff);
        free(index_buff);
        return(-5);
    }
    memmove(index_buff, index_buff+1, strlen(index_buff));

    if((close_bracked = strchr(index_buff, ']')) == NULL)
    {
        strcpy(error_str, "Syntax Error: Missing \"]\"");
        free(index_buff);
        return(-6);
    }
    close_bracked[0] = '\0';

    if(strcmp(index_buff, "loop.i") == 0)
    {
        if((*y = getIntValue(anker, "loop.i")) < 0)
        {
            strcpy(error_str, "Unkown Variable [loop.i]");
            free(index_buff);
            return(-7);
        }
    }
    else
    {
        if(str_isdigit(index_buff) < 0)
        {
            strcpy(error_str, "Syntax Error: Index is not a number");
            free(index_buff);
            return(-8);
        }
        *y = atoi(index_buff);
    }
    free(index_buff);
    return(2);
}

/**
 * @brief Parst einen Funktionsnamen aus dem Variablennamen
 *
 * @param anker Anker Punkt fuer Variablen
 * @param variable Buffer welcher die Variable beinhaltet
 * @param function_buffer Buffer in welchen der Funktionsname geschrieben wird
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 *
 * @return 0: Keine Funktion gefunden; 1: Funktion gefunden
 */
int getFunction(struct variables *anker, char *variable, char *function_buffer,
                char* error_str)
{
    char *point;

    if((point = strchr(variable, '.')) == NULL)
    {
        //Keine Function gefunden
        return(0);
    }
    point[0] = '\0';

    strcpy(function_buffer, point+1);
    return(1);
}

/**
 * @brief Gibt eine Int Variable aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleIntVar(struct variables *anker, FILE *p_output, char *variable,
                 int has_function, char *functionbuffer, char *error_str)
{
    int value;
    if(has_function == 1)
    {
        //Funktion auf Variable anwenden
    }
    else if(has_function == 0)
    {
        if((value = getIntValue(anker, variable)) < 0)
        {
            sprintf(error_str, "Unkown Variable: [%s]", variable);
            return(-1);
        }
        fprintf(p_output, "%d", value);
    }
    return(0);
}

/**
 * @brief Gibt ein 1D Int Array aus.
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben
 * @param x_index Gefundene X Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleInt1DArray(struct variables *anker, FILE *p_output, char *variable,
                     int index_type, int x_index, int has_function,
                     char *functionbuffer, char *error_str)
{
    int value;
    wchar_t *str_array;
    if(index_type == 0 && has_function == 0)
    {
        //Komplettes Array ausgeben
        if((str_array = generateStringValuefromArray(anker, variable)) == NULL)
        {
            strcpy(error_str, varhandle_error_str);
            return(-1);
        }
        fprintf(p_output, "%S", str_array);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //funktion auf komplettes Array anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //Index aus Array ausgeben
        if((value = getIntValuefromArray(anker, variable, x_index)) == -1)
        {
            sprintf(error_str, "Unkown Variable: [%s]", variable);
            return(-1);
        }
        else if(value == -2)
        {
            strcpy(error_str, "Index Error: Index out of Range");
            return(-2);
        }
        fprintf(p_output, "%d", value);

    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion auf index anwenden
    }
    return(0);
}

/**
 * @brief Gibt ein 2D Int Array aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben? 
 * @param x_index Gefundene X Index
 * @param y_index Gefundene Y Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleInt2DArray(struct variables *anker, FILE *p_output, char *variable,
                     int index_type, int x_index, int y_index,
                     int has_function, char *functionbuffer, char *error_str)
{
    int value, rc;
    wchar_t *str_array;
    struct variables *tmp_array;

    if(index_type == 0 && has_function == 0)
    {
        //Komplettes Array ausgeben
        if((str_array = generateStringValuefromArray(anker, variable)) == NULL)
        {
            strcpy(error_str, "Unkown var");
            return(-1);
        }
        fprintf(p_output, "%S", str_array);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //Funktion aufs ganze Array anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //1D Array aus index generieren und ausgeben
        tmp_array = createTmpArrayOut2DArray(anker, variable, x_index);
        printArrayfromPtrtoFile(tmp_array, p_output);
    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion auf generiertes 1D Array von index anwenden
    }
    else if(index_type == 2 && has_function == 0)
    {
        //Wert aus Array auslesen und ausgeben
        if((rc = getIntValuefrom2DArray(anker, variable, x_index, y_index,
                                        &value)) == -1)
        {
            sprintf(error_str, "Unkown Variable: [%s]", variable);
            return(-1);
        }
        else if(rc == -2)
        {
            strcpy(error_str, "Index Error: Index out of Range");
            return(-2);
        }
        fprintf(p_output, "%d", value);
    }
    else if(index_type == 2 && has_function == 1)
    {
        //Funktion auf bestimmten index anwenden
    }
    return(0);
}

/**
 * @brief Gibt eine String Variable aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben
 * @param x_index Gefundene X Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleStringVar(struct variables *anker, FILE *p_output, char *variable,
                    int index_type, int x_index, int has_function,
                    char *functionbuffer, char *error_str)
{
    char *value;

    if(index_type == 0 && has_function == 0)
    {
        //Kompletten String ausgeben
        value = getStringValue(anker, variable);
        fprintf(p_output, "%s", value);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //Funktion auf komplettenm String anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //Bestimmten Char aus String ausgeben
        value = getStringValue(anker, variable);
        if(x_index > strlen(value)-1)
        {
            strcpy(error_str, "Index Error: Index out of range\n");
            return(-1);
        }
        fprintf(p_output, "%c", value[x_index]);
    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion auf bestimmten Char anwenden
    }
    return(0);
}

/**
 * @brief Gibt ein 1D String Array aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben
 * @param x_index Gefundene X Index
 * @param y_index Gefundene Y Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleString1DArray(struct variables *anker, FILE *p_output, char *variable,
                        int index_type, int x_index, int y_index, int has_function,
                        char *functionbuffer, char *error_str)
{
    char *value;
    wchar_t *str_array;

    if(index_type == 0 && has_function == 0)
    {
        //Komplettes Array ausgeben
        if((str_array = generateStringValuefromArray(anker, variable)) == NULL)
        {
            strcpy(error_str, varhandle_error_str);
            return(-1);
        }
        fprintf(p_output, "%S", str_array);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //Funktion aufs ganze Array anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //Einzelnes Feld ausgeben
        value = getStringValuefromArray(anker, variable, x_index);
        fprintf(p_output, "%s", value);
    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion nur auf ein Feld ausfuehren
    }
    else if(index_type == 2 && has_function == 0)
    {
        //bestimmtes Char aus dem String ausgeben
        value = getStringValuefromArray(anker, variable, x_index);
        if(y_index > strlen(value)-1)
        {
            strcpy(error_str, "Index Error: Index out od range\n");
            return(-2);
        }
        fprintf(p_output, "%c", value[y_index]);
    }
    else if(index_type == 2 && has_function == 1)
    {
        //Funktion nur auf ein bestimmtes Char ausfuehren
    }

    return(0);
}

/**
 * @brief Gibt ein 2D String Array aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben
 * @param x_index Gefundene X Index
 * @param y_index Gefundene Y Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleString2DArray(struct variables *anker, FILE *p_output, char *variable,
                        int index_type, int x_index, int y_index, int has_function,
                        char *functionbuffer, char *error_str)
{
    char *value;
    wchar_t *str_array;
    struct variables *tmp_array;

    if(index_type == 0 && has_function == 0)
    {
        //Komplettes Array ausgeben
        if((str_array = generateStringValuefromArray(anker, variable)) == NULL)
        {
            strcpy(error_str, varhandle_error_str);
            return(-1);
        }
        fprintf(p_output, "%S", str_array);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //Funktion auf kompletets Array anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //Nur Index als Array ausgeben
        tmp_array = createTmpArrayOut2DArray(anker, variable, x_index);
        printArrayfromPtrtoFile(tmp_array, p_output);
    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion nur auf bestimmtes Index Array anwenden
    }
    else if(index_type == 2 && has_function == 0)
    {
        //Nur bestimmtest Feld ausgeben
        value = getStringValuefrom2DArray(anker, variable, x_index, y_index);
        fprintf(p_output, "%s", value);
    }
    else if(index_type == 2 && has_function == 1)
    {
        //Funktion nur auf bestimtest Feld anwenden
    }
    return(0);
}

/**
 * @brief Gibt eine String Variable aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben
 * @param x_index Gefundene X Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleUStringVar(struct variables *anker, FILE *p_output, char *variable,
                    int index_type, int x_index, int has_function,
                    char *functionbuffer, char *error_str)
{
    wchar_t *value;

    if(index_type == 0 && has_function == 0)
    {
        //Kompletten String ausgeben
        value = getUStringVal(anker, variable);
        fprintf(p_output, "%S", value);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //Funktion auf komplettenm String anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //Bestimmten Char aus String ausgeben
        value = getUStringVal(anker, variable);
        if(x_index > wcslen(value)-1)
        {
            strcpy(error_str, "Index Error: Index out of range\n");
            return(-1);
        }
        fprintf(p_output, "%c", value[x_index]);
    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion auf bestimmten Char anwenden
    }
    return(0);
}


/**
 * @brief Gibt ein 1D String Array aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben
 * @param x_index Gefundene X Index
 * @param y_index Gefundene Y Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleUString1DArray(struct variables *anker, FILE *p_output, char *variable,
                        int index_type, int x_index, int y_index, int has_function,
                        char *functionbuffer, char *error_str)
{
    wchar_t *value, *str_array;

    if(index_type == 0 && has_function == 0)
    {
        //Komplettes Array ausgeben
        if((str_array = generateStringValuefromArray(anker, variable)) == NULL)
        {
            strcpy(error_str, varhandle_error_str);
            return(-1);
        }
        fprintf(p_output, "%S", str_array);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //Funktion aufs ganze Array anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //Einzelnes Feld ausgeben
        value = getUStringArrayVal(anker, variable, x_index);
        fprintf(p_output, "%S", value);
    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion nur auf ein Feld ausfuehren
    }
    else if(index_type == 2 && has_function == 0)
    {
        //bestimmtes Char aus dem String ausgeben
        value = getUStringArrayVal(anker, variable, x_index);
        if(y_index > strlen(value)-1)
        {
            strcpy(error_str, "Index Error: Index out od range\n");
            return(-2);
        }
        fprintf(p_output, "%c", value[y_index]);
    }
    else if(index_type == 2 && has_function == 1)
    {
        //Funktion nur auf ein bestimmtes Char ausfuehren
    }

    return(0);
}

/**
 * @brief Gibt ein 2D String Array aus
 *
 * @param anker Anker Punkt fuer Variablen
 * @param p_output File pointer auf das Output file
 * @param variable Buffer welcher die Variable beinhaltet
 * @param index_type Wurde ein Index mit uebergeben
 * @param x_index Gefundene X Index
 * @param y_index Gefundene Y Index
 * @param has_function Ist eine Funktion uebergeben (1 || 0)
 * @param functionbuffer Buffer welche die Funktion enthaehlt
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handleUString2DArray(struct variables *anker, FILE *p_output, char *variable,
                        int index_type, int x_index, int y_index, int has_function,
                        char *functionbuffer, char *error_str)
{
    wchar_t *value;
    wchar_t *str_array;
    struct variables *tmp_array;

    if(index_type == 0 && has_function == 0)
    {
        //Komplettes Array ausgeben
        if((str_array = generateStringValuefromArray(anker, variable)) == NULL)
        {
            strcpy(error_str, varhandle_error_str);
            return(-1);
        }
        fprintf(p_output, "%S", str_array);
    }
    else if(index_type == 0 && has_function == 1)
    {
        //Funktion auf kompletets Array anwenden
    }
    else if(index_type == 1 && has_function == 0)
    {
        //Nur Index als Array ausgeben
        tmp_array = createTmpArrayOut2DArray(anker, variable, x_index);
        printArrayfromPtrtoFile(tmp_array, p_output);
    }
    else if(index_type == 1 && has_function == 1)
    {
        //Funktion nur auf bestimmtes Index Array anwenden
    }
    else if(index_type == 2 && has_function == 0)
    {
        //Nur bestimmtest Feld ausgeben
        value = getUString2DArrayVal(anker, variable, x_index, y_index);
        fprintf(p_output, "%s", value);
    }
    else if(index_type == 2 && has_function == 1)
    {
        //Funktion nur auf bestimtest Feld anwenden
    }
    return(0);
}

/**
 * @brief Parst ein Variablensttring mit Index und Funktion und ruft 
 *        demendsprechend den Handler auf
 *
 * @param anker Anker Punkt fuer Variablen
 * @param variable Buffer welcher die Variable beinhaltet
 * @param p_output File pointer auf das Output file
 * @param error_str Buffer in den Error Nachrichten geschrieben werden
 * 
 * @return Erfolg: 0; Error < 0
 */
int handle_variable(struct variables *anker, char *variable, FILE *p_output,
                    char *error_str)
{
    int var_type, index_type, x_index, y_index;
    char function_buffer[100];
    int has_function = 0;

    variable = StripTrailingSpaces(variable);
    TrimSpaces(variable);

    if(strcmp(variable, "loop.i") != 0 && strstr(variable, "loop.i") == NULL)
    {
        has_function = getFunction(anker, variable, function_buffer, error_str);
    }
    else
    {
        has_function = 0;
    }
    if((index_type = getIndex(anker, variable, &x_index, &y_index,
                              error_str)) < 0)
    {
        return(-1);
    }

    if((var_type = getVarType(anker, variable)) < 0)
    {
        strcpy(error_str, varhandle_error_str);
        return(-2);
    }

    //Call Handles for current var type
    if(var_type == INT && index_type != 0)
    {
        strcpy(error_str, "Type Error: Int is not indexable");
        return(-3);
    }
    else if(var_type == INT)
    {
        if(handleIntVar(anker, p_output, variable, has_function, function_buffer,
                     error_str) < 0)
        {
            return(-4);
        }
        return(0);
    }

    if(var_type == INTARRAY && index_type == 2)
    {
        strcpy(error_str, "Type Error: IntegerArray has no second index");
        return(-5);
    }
    else if(var_type == INTARRAY)
    {
        if(handleInt1DArray(anker, p_output, variable, index_type, x_index,
                            has_function, function_buffer, error_str) < 0)   
        {
            return(-6);
        }
        return(0);
    }
    
    if(var_type == TWO_DINTARRAY)
    {
        if(handleInt2DArray(anker, p_output, variable, index_type, x_index,
                            y_index, has_function, function_buffer,
                            error_str) < 0)
        {
            return(-7);
        }
        return(0);
    }

    if(var_type == STRING && index_type == 2)
    {
        strcpy(error_str, "Type Error: String has no second index");
        return(-8);
    }
    else if(var_type == STRING)
    {
        if(handleStringVar(anker, p_output, variable,
                    index_type, x_index, has_function,
                    function_buffer, error_str) < 0)
        {
            return(-9);
        }
        return(0);

    }

    if(var_type == STRINGARRAY)
    {

        if(handleString1DArray(anker, p_output, variable,
                               index_type, x_index, y_index, has_function,
                               function_buffer, error_str) < 0)
        {
            return(-10);
        }
        return(0);
    }

    if(var_type == TWO_DSTRINGARRAY)
    {
        if(handleString2DArray(anker, p_output, variable,
                               index_type, x_index, y_index, has_function,
                               function_buffer, error_str) < 0)
        {
            return(-11);
        }
        return(0);
    }

    if(var_type == U_STRING)
    {
        if(handleUStringVar(anker, p_output, variable,
                    index_type, x_index, has_function,
                    function_buffer, error_str) < 0)
        {
            return(-12);
        }
        return(0);

    }

    if(var_type == U_STRINGARRAY)
    {
        if(handleUString1DArray(anker, p_output, variable,
                               index_type, x_index, y_index, has_function,
                               function_buffer, error_str) < 0)
        {
            return(-10);
        }
        return(0);

    }

    if(var_type == U_TWO_DSTRINGARRAY)
    {
        if(handleUString2DArray(anker, p_output, variable,
                               index_type, x_index, y_index, has_function,
                               function_buffer, error_str) < 0)
        {
            return(-11);
        }
        return(0);

    }
}
