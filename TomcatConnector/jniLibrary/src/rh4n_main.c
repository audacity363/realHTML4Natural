#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rh4n.h"
#include "rh4n_logging.h"
#include "rh4n_utils.h"
#include "rh4n_jni_environ.h"
#include "natural/rh4n_natcall.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <Inputfile>\n", argv[0]);
		exit(1);
	}

	int ret = 0;
	RH4nProperties props;

	if((ret = rh4nUtilsLoadProperties(argv[1], &props)) != RH4N_RET_OK) {
		exit(ret);
	}

	props.logging = rh4nLoggingCreateStreamingRule(props.natlibrary, props.natprogram, props.i_loglevel, props.logpath);

	rh4nCallNatural(&props, NULL);
}
