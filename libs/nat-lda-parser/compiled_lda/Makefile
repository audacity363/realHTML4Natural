HOMEDIR = /home/tom/Documents/sharedlib_jinja2

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

CC = gcc

all:
	$(CC) -g -c natone.c -o ./bin/natone.o $(INCDIR)
	$(CC) -g -c unix.c -o ./bin/unix.o $(INCDIR) 
	$(CC) -g -c main.c -o ./bin/main.o $(INCDIR)
	ar -vcru libldaparser.a ./bin/*.o 

clean:
	-rm ./bin/*.o
	-rm ./*.a

