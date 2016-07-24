#HOMEDIR = /u/it/a140734/C/sharedlib_jinja2
HOMEDIR = /home/tom/Documents/realHtml4Natural

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

LIBS     =  -L$(VAR_PATH) -lvarhandle \
			-L$(UTILS_PATH) -lutils

CC = gcc

all:
	$(CC) -g -c -o ./bin/utils.o utils.c $(INCDIR)
	$(CC) -g -c -o ./bin/if.o if.c $(INCDIR)
	$(CC) -g -c -o ./bin/for.o for.c $(INCDIR)
	$(CC) -g -c -o ./bin/parser.o parser.c $(INCDIR)
	$(CC) -g -c -o ./bin/main.o main.c $(INCDIR)
	ar -vcru libjinjaparser.a ./bin/*.o

testprog:
	gcc  -o ./test/main ./test/main.c $(INCDIR) -I./ -L./ -ljinjaparser $(LIBS)

clean:
	-rm -f ./bin/*.o
	-rm -f ./*.a
