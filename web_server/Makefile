#CC=gcc
CC=xlc

CFLAGS=-O3 -Wunused-result

HEADERS = httpint.h httpapi.h httpxml.h

TARGET = miniweb
HTTPOBJ += ./bin/miniweb.o

DEFINES=

OS="Linux"

DEFINES+= -DHTTP_DEBUG
LDFLAGS = -lm -L${SAG}/common/lib -brtl -Wl,-bbigtoc,-blibpath:/usr/lib/threads:/opt/softwareag/common/lib:/opt/softwareag/nat/v63140/lib:$(NATDIR)/$(NATVERS)/lib:/usr/lib:/lib -bE:$(NATDIR)/$(NATVERS)/bin/build/samples.exp -lpthreads -lc -lC -ldl -g


#HOMEDIR = /home/tom//Documents/realHtml4Natural/
HOMEDIR = /u/it/a140734/C/realHtml4Natural/

STANDARD_PATH    	=   $(HOMEDIR)/include
CFG_PATH     		= 	$(HOMEDIR)/libs/CfgReader/
HEXD_PATH           =   $(HOMEDIR)/libs/hexdump/

INCDIR = -I$(STANDARD_PATH) \
		 -I$(CFG_PATH) \
		 -I$(HEXD_PATH) \
		 -I./


LIBS = -L$(CFG_PATH) -lcfg \
	   -L$(HEXD_PATH) -lhexdump

all: 
	@echo Building for $(OS)
	$(CC) $(DEFINES) -g -c -o ./bin/httppil.o httppil.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/http.o http.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/httpxml.o httpxml.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/httphandler.o httphandler.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/httppost.o httppost.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/httpauth.o httpauth.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/lookup.o lookup.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/natural.o natural.c $(INCDIR)
	$(CC) $(DEFINES) -g -c -o ./bin/miniweb.o miniweb.c $(INCDIR)
	$(CC) $(LDFLAGS) -o ./miniweb ./bin/*.o $(LIBS)

clean:
	@rm -f ./bin/*.o
	@rm -rf Debug Release
