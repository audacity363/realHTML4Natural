#HOMEDIR = /home/tom/Documents/sharedlib_jinja2
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
STND_PATH   = $(HOMEDIR)/include/

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
	$(CC) -c var2name.c -o ./bin/var2name.o $(INCDIR)
	$(CC) -c genpage.c -o ./bin/genpage.o $(INCDIR)
	ar -vcru libvar2name.a ./bin/*.o

clean:
	-rm ./bin/*.o
	-rm ./*.a
