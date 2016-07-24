/*	+-----------------------------------------------------------------------+
	|	COPYRIGHT 1997									VAW GREVENBROICH	|
	|																		|
	|	Modul			:	cfg.c											|
	|	Projekt			:													|
	|																		|
	|	Proceduren des Moduls												|
	|		-GLOBAL			:	OpenCfg()									|
	|							GetCfgToken()								|
	|							CloseCfg()									|
	|		-MODUL LOKAL	:	()											|
	|																		|
	|	Beschreibung	:	Programm, um aus einem Konfigurationsfile 		|
	|						Eintraege zu suchen/lesen.						|
	|	Ein Eintrag im Konfigurationsfile hat den folgenden Aufbau:			|
	|			IDENT-NAME:													|
	|				Etherminal 	= xxxx										|
	|				TCP-Port	= yyyy										|
	|                                                                       |
	+-----------+-----------------------------------------------+-----------+
	| Datum     | Taetigkeit									| Bearbeiter|
	+-----------+-----------------------------------------------+-----------+
	| 25.02.97	| Fertigstellung								| Engemann	|
	+-----------+-----------------------------------------------+-----------+
	| 29.10.97	| Differenzierte Fehlermeldung					| Engemann	|
	+-----------+-----------------------------------------------+-----------+
	| 31.07.00	| Stringvergleich genauer						| Engemann	|
	+-----------+-----------------------------------------------+-----------+
*/        
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define	CFGPATH				"CFGPATH"		
#define	TOKEN_PRO_ZEILE		3
#define FALSE 0
#define TRUE  1

char		zeile[200];
fpos_t		position;
FILE		*p_config_file;


static char	info[200];

/*	+-----------------------------------------------------------------------+
	|	Procedure	:		open_cfg()										|
	|																		|
	|	Beschreibung	:	MODUL GLOBAL									|
	|																		|
	|	Aus einem Konfigurationsfile wird anhand eines Idend-Namens			|
	|	der Terminal-Server-Name und das Port des Terminal-Servers 			|
	|	der angeschlossenen Maschine ermittelt.								|
	|	Ein Eintrag im Konfigurationsfile hat den folgenden Aufbau:			|
	|			IDENT-NAME:													|
	|				Etherminal 	= xxxx										|
	|				TCP-Port	= yyyy										|
	|                                                                       |
	|	p_cfg					->	Name des Konfigurationsfiles			|
	|	p_ident					->	IDENT-NAME								|
	|	p_error					->	Rueckgabe Error-String (bei -1)			|
	|                                                                       |
	|	return()				0/-1 (ok / nicht ok)						|
	+-----------+-----------------------------------------------+-----------+
	| Datum     | Taetigkeit									| Bearbeiter|
	+-----------+-----------------------------------------------+-----------+
	| 05.02.97  | Fertigstellung								|D. Engemann|
	+-----------+-----------------------------------------------+-----------+
*/
int	OpenCfg(char *p_cfg, char *p_ident, char **p_error)
{

char		config_file_name[100],
			ident_string[30],
			*p_env;

static char ErrorBuffer[100];

int			ident_flag = FALSE;

size_t		len;

	/*
		+---------------------------------------------------------------+
		| Aus einer Environment-Variable wird das Verzeichnis des		|
		| Konfigurationsfiles gelesen.									|
		+---------------------------------------------------------------+
	*/
	if (p_cfg == NULL)
	{
		*p_error = 	"Konfigurationsfile nicht angegeben"; 
		return(-1);
	}
	
    strcpy(config_file_name, p_cfg);

	/*
		+---------------------------------------------------------------+
		| Die entsprechende Konfigurationsdatei kann jetzt ausgewertet 	|
		| werden.                                                       |
		+---------------------------------------------------------------+
	*/
	if ((p_config_file = fopen(config_file_name, "r")) == NULL)
	{
		sprintf(ErrorBuffer, "Konfigurationsdatei [%s] nicht vorhanden",
								config_file_name);
		*p_error = ErrorBuffer;
		return(-1);
	}

	/*
		Bei der Suche des Ident-Namens im Konfigurationsfile wird grundsaetzlich
		mit Grossbuchstaben gearbeitet. 
	*/
    strcpy(ident_string, p_ident);
    strcat(ident_string, ":");
	sprintf(info, "[%s] Ident-Name [%s]\n", config_file_name, ident_string);

	/*
		+---------------------------------------------------------------+
		| Suche nach dem IDENT-NAMEN im Konfigurationsfile.				|
		|                                   	                        |
		| Aufbau:                       	                            |
		|	IDENT-NAME:													|
		|		eintrag_1  = xxxx	 									|
		|		eintrag_2  = yyyy	                                    |
		|		eintrag_3  = zzzz	                                    |
		+---------------------------------------------------------------+
	*/
	while (fgets(zeile, sizeof(zeile), p_config_file) != NULL)
	{
		len = strlen (zeile);

		if (len > 0 && (zeile[len-1] == '\n'))
		{
			zeile[len-1] = '\0';
		}

		/* Kommentar; naechste Zeile einlesen							*/
		if (zeile[0] == ';' || zeile[0] == '\0' || zeile[0] == ' ' || zeile[0] == '#')
		{
			continue;
		}
		/* Strings mussen gleich sein									*/
		if (strncmp(zeile, ident_string, strlen(ident_string)) == 0)
		/* In der Zeile wird nach dem Ident - String gesucht.			*/
/*       if ((strstr(str_toupper(zeile), ident_string)) != NULL) */
		{
			ident_flag = TRUE;
			break;
		}
	} /* while */

	if (!ident_flag)
	{
		sprintf(ErrorBuffer, "Eintrag [%s] im Konfigurations-File nicht gefunden",
							  ident_string);	
		*p_error = ErrorBuffer;
		fclose (p_config_file);
		return(-1);
	}

	/*
		Position des Ident-Namens merken	*/	 

	fgetpos(p_config_file, &position);

	return(0);

}
/*	+-----------------------------------------------------------------------+
	|	Procedure	:		GetCfgToken()									|
	|																		|
	|	Beschreibung	:	MODUL GLOBAL									|
	|																		|
	|	Der naechste Eintrag im Konfigurationfile wird gesucht.				|
	|	Ein Eintrag im Konfigurationsfile hat den folgenden Aufbau:			|
	|			IDENT-NAME:													|
	|				Etherminal 	= xxxx										|
	|				TCP-Port	= yyyy										|
	|                                                                       |
	|	p_token					->	zu suchendes Token						|
	|	p_error					->	String mit Fehlermeldung				|
	|                                                                       |
	|	return()				0/-1 (ok / nicht ok)						|
	+-----------+-----------------------------------------------+-----------+
	| Datum     | Taetigkeit									| Bearbeiter|
	+-----------+-----------------------------------------------+-----------+
	| 05.02.97  | Fertigstellung								|D. Engemann|
	+-----------+-----------------------------------------------+-----------+
*/
char *GetCfgToken(char *p_token, char **p_error, char PrintInfo)
{

static	char	eintrag[100];
static	char	error[100];
		char	*p_zeile,
    			*token[TOKEN_PRO_ZEILE];
		int		j;


	/* Immer ab nach Ident-Namen weitersuchen		*/
	fsetpos(p_config_file, &position);

	while (TRUE)
	{
 		if (fgets(zeile, sizeof(zeile), p_config_file) == NULL)
		{
			sprintf(error, "Eintrag [%s] im Konfigurations-File nicht gefunden", p_token);
			*p_error = error;
			fclose (p_config_file);
			return(NULL);
		}
		/* Zeilen-Ende \n durch String-Ende \0 ersetzen				*/
		(char)(*(strchr(zeile, '\n')) = '\0');

		/* Kommentar; naechste Zeile einlesen							*/
/**		if (zeile[0] == ';' || zeile[0] == '\0' || zeile[0] == ' ' || zeile[0] == '#') **/
		if (zeile[0] == ';' || zeile[0] == '\0' || zeile[0] == '#')	{
			continue;
		}
        if (strstr(zeile, ":") != NULL)
		{
			sprintf(error, "Eintrag [%s] im Konfigurations-File nicht gefunden", p_token);
			*p_error = error;
			return(NULL);
		}
		/* Token in der aktuellen Zeile suchen	*/
		if (strstr(zeile, p_token) != NULL)
		{
			break;
		}
	}

	/* Ausplittung der Zeile in Token							*/
	for (j = 0; j < TOKEN_PRO_ZEILE; j++)
	{
		p_zeile = (j == 0) ? zeile : NULL;
		if ((token[j] = (char *)strtok (p_zeile, " ,\t:")) == NULL)
		{
			*p_error = "Syntax-Fehler im Konfigurationsfile (fehlender Eintrag)";
			fclose (p_config_file);
			return(NULL);
		}
	}

	/* Zeile ausgeben											*/
	if (PrintInfo) {
		printf("CFG: [%s %s %s]\n", token[0], token[1], token[2]);
	}

	/*  Nur das jeweils 3. Token wird zurueckgegeben			*/
	strcpy(eintrag, token[2]);
	return eintrag;	

}
/*	+-----------------------------------------------------------------------+
	|	Procedure	:		CloseCfg()										|
	|																		|
	|	Beschreibung	:	MODUL LOKAL										|
	|																		|
	|	Aus einem Konfigurationsfile wird anhand eines Idend-Namens			|
	|	der Terminal-Server-Name und das Port des Terminal-Servers 			|
	|	der angeschlossenen Maschine ermittelt.								|
	|	Ein Eintrag im Konfigurationsfile hat den folgenden Aufbau:			|
	|			IDENT-NAME:													|
	|				Etherminal 	= xxxx										|
	|				TCP-Port	= yyyy										|
	|                                                                       |
	|	return()				0/-1 (ok / nicht ok)						|
	+-----------+-----------------------------------------------+-----------+
	| Datum     | Taetigkeit									| Bearbeiter|
	+-----------+-----------------------------------------------+-----------+
	| 05.02.97  | Fertigstellung								|D. Engemann|
	+-----------+-----------------------------------------------+-----------+
*/
int	CloseCfg(void)
{
	return(fclose(p_config_file));

}

char *CfgInfo(void)
{
	return(info);

}

