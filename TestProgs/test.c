#include <stdlib.h>
#include <stdio.h>


long main()
{
	/*if (nparm != 1)	{
		return((long)1);
    }*/

    FILE *logfile = fopen("/tmp/sharedlibtest_tengemann", "a");
    if(logfile == NULL)
    {
        return((long)1);
    }

    printf("%p\n", logfile);

    char teststr[300] = "Hello World";

    /*memcpy((char*)teststr, (char*)parmptr[0], 298);
    teststr[299] = '\0';
    fput(teststr, logfile);*/
    fputs(teststr, logfile);

    fclose(logfile);

    return((long)0);
}
