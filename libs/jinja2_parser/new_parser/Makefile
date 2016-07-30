#HOMEDIR = /u/it/a140734/C/realHtml4Natural/
HOMEDIR = /home/tom//Documents/realHtml4Natural/

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

NAMES_PATH = $(HOMEDIR)/var2names
WEB_PATH   = $(HOMEDIR)/web_server

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

LIBS    = -L$(CFG_PATH) -lcfg \
          -L$(HEXD_PATH) -lhexdump \
          -L$(SOC_PATH) -lsockets \
          -L$(WEB_PATH) -lwebserver \
          -L$(NATVAR_PATH) -lvar2name \
          -L$(LDA_PATH) -lldaparser \
          -L$(JINJA_PATH) -ljinjaparser \
          -L$(VAR_PATH) -lvarhandle \
          -L$(UTILS_PATH) -lutils \

#CC = xlc
CC = gcc

all:
	$(CC) -g -c -o ./bin/file_handling.o file_handling.c $(INCDIR)
	$(CC) -g -c -o ./bin/start_jinjaparser.o start_jinjaparser.c $(INCDIR)
	$(CC) -g -c -o ./bin/variable_handle.o variable_handle.c $(INCDIR)
	$(CC) -g -c -o ./bin/command_handling.o command_handling.c $(INCDIR)
	$(CC) -g -c -o ./bin/for_handling.o for_handling.c $(INCDIR)
	$(CC) -g -c -o ./bin/if_handling.o if_handling.c $(INCDIR)
	$(CC) -g -o ./main main.c $(INCDIR) ./bin/*.o $(LIBS)
	
