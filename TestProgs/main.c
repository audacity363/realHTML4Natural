#include <stdio.h>

#include "hexdump.h"
#include "natuser.h"

#define RC_OK			0


long natnetposts (WORD nparm, BYTE **parmptr, FINFO *parmdec);
/*
	Es muessen drei Paramter in parmptr angegeben sein:
    parmptr[0]  Rechner-ID/Kettenname
    parmptr[1]  Return-Code (RC_OK/RC_ERROR)
    parmptr[2]  Return-String
*/

int main() 
{
	short int	rc_init = 0;
	int			n_parm;
	long    	rc;

	register	short int i;

	char		return_string[80];
	char		gewicht[80];

	struct	{
				char 		*p_rechner;
				short int   *p_return_code;
				char		*p_return_string;
			} nat_uebergabe;

	nat_uebergabe.p_rechner = "BALIABFO";		/* Waagen-Nummer	*/
    nat_uebergabe.p_return_code = &rc_init;

	memset(return_string, '0', sizeof(return_string));
	nat_uebergabe.p_return_string = (char*)&return_string;

	n_parm = 3;

	rc = natnetposts((WORD)n_parm, (BYTE**)&nat_uebergabe, NULL);

	if (rc == RC_OK)
	{
		printf("Alles klar \n");
	}
	else
	{
		printf("NAT_WAAGE: Return-Code %d\n",(short int) *nat_uebergabe.p_return_code);
		printf("NAT_WAAGE: %78.78s\n", nat_uebergabe.p_return_string);
	}


	/*nat_waage_protokoll();*/

}
