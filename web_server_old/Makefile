HOMEDIR = /u/it/a140734/C/sharedlib_jinja2

STANDARD_PATH    	=   $(HOMEDIR)/include

HEXD_PATH   = $(HOMEDIR)/libs/hexdump/
CFG_PATH    = $(HOMEDIR)/libs/CfgReader/
SOC_PATH    = $(HOMEDIR)/libs/Sockets/
WEB_PATH    = $(HOMEDIR)/web_server/
JINJA_PATH  = $(HOMEDIR)/libs/jinja2_parser/
VAR_PATH    = $(HOMEDIR)/libs/varhandle_lib/
LDA_PATH    = $(HOMEDIR)/libs/nat-lda-parser/compiled_lda/
NATVAR_PATH = $(HOMEDIR)/var2names/
UTILS_PATH  = $(HOMEDIR)/libs/utils/

INCDIR   =  -I$(STANDARD_PATH)  \
            -I$(HEXD_PATH)      \
            -I$(CFG_PATH)       \
            -I$(SOC_PATH)       \
            -I$(WEB_PATH)       \
            -I$(JINJA_PATH)     \
            -I$(VAR_PATH)       \
            -I$(LDA_PATH)       \
            -I$(NATVAR_PATH)    \
            -I$(UTILS_PATH)

CC = xlc

all:
	$(CC) -c -o ./bin/lookup.o lookup.c $(INCDIR)
	$(CC) -c -o ./bin/filehandling.o filehandling.c  $(INCDIR)
	$(CC) -c -o ./bin/response.o response.c  $(INCDIR)
	$(CC) -c -o ./bin/header.o header.c  $(INCDIR)
	$(CC) -c -o ./bin/start.o main.c  $(INCDIR)
	ar -cru libwebserver.a ./bin/*.o

testprog:
	xlc -o ./test/main ./test/main.c $(LIBS) -I./ -L./ -lwebserver

clean:
	-rm ./bin/*.o
	-rm *.a
