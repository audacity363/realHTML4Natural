/*	+-----------------------------------------------------------------------+
	|	COPYRIGHT 1997			VAW GREVENBROICH							|
	|																		|
	|	Include-Datei	:		cfg.h										|
	|																		|
	|	Beschreibung	:		Beschreibung der
	|																		|
	+-----------+-----------------------------------------------+-----------+
	| Datum     | Taetigkeit									| Bearbeiter|
	+-----------+-----------------------------------------------+-----------+
	| 07.11.97	| Fertigstellung								|D. Engemann|
	+-----------+-----------------------------------------------+-----------+
*/

/*
	+-----------------------------------------------------------------------+
	|	globale Defines														|
	+-----------------------------------------------------------------------+
*/
#ifndef _CFG_H_
#define _CFG_H_

#define	CFG_FILE	"kopplung.cfg"

extern int	OpenCfg(char *, char *, char **);
extern char *GetCfgToken(char *, char **, char);
extern int	CloseCfg(void);
extern char	*CfgInfo(void);

#endif /* of _CFG_H_ */
