.PHONY: var2names web_server
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
CFLAGS = -c -g

ARFLAGS = -vcru



cfgreader:
	$(CC) $(CFLAGS) -o $(CFG_PATH)/bin/cfg.o $(CFG_PATH)/cfg.c $(INCDIR)
	ar $(ARFLAGS) $(CFG_PATH)/libcfg.a $(CFG_PATH)/bin/*.o

clean_cfgreader:
	-rm -f $(CFG_PATH)/bin/*.o $(CFG_PATH)/*.a

hexdump:
	$(CC) $(CFLAGS) -o $(HEXD_PATH)/bin/hexdump.o $(HEXD_PATH)/hexdump.c \
			$(INCDIR)
	ar $(ARFLAGS) $(HEXD_PATH)/libhexdump.a $(HEXD_PATH)/bin/*.o

clean_hexdump:
	-rm -f $(HEXD_PATH)/bin/*.o $(HEXD_PATH)/*.a

jinjaparser:
	$(CC) $(CFLAGS) -o $(JINJA_PATH)/bin/utils.o $(JINJA_PATH)/utils.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(JINJA_PATH)/bin/if.o $(JINJA_PATH)/if.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(JINJA_PATH)/bin/for.o $(JINJA_PATH)/for.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(JINJA_PATH)/bin/parser.o $(JINJA_PATH)/parser.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(JINJA_PATH)/bin/main.o $(JINJA_PATH)/main.c $(INCDIR)
	ar $(ARFLAGS) $(JINJA_PATH)/libjinjaparser.a $(JINJA_PATH)/bin/*.o

clean_jinjaparser:
	-rm -f $(JINJA_PATH)/bin/*.o $(JINJA_PATH)/*.a

ldaparser:
	$(CC) $(CFLAGS) -o $(LDA_PATH)/bin/natone.o $(LDA_PATH)/natone.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(LDA_PATH)/bin/unix.o $(LDA_PATH)/unix.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(LDA_PATH)/bin/main.o $(LDA_PATH)/main.c $(INCDIR)
	ar $(ARFLAGS) $(LDA_PATH)/libldaparser.a $(LDA_PATH)/bin/*.o

clean_ldaparser:
	-rm -f $(LDA_PATH)/bin*.o $(LDA_PATH)/*.a

sockets:
	$(CC) $(CFLAGS) -o $(SOC_PATH)/bin/sockets.o $(SOC_PATH)/sockets.c $(INCDIR)
	ar $(ARFLAGS) $(SOC_PATH)/libsockets.a $(SOC_PATH)/bin/*.o

clean_sockets:
	-rm -f $(SOC_PATH)/bin/*.o $(SOC_PATH)/*.a

utils:
	$(CC) $(CFLAGS) -o $(UTILS_PATH)/bin/utils.o $(UTILS_PATH)/utils.c $(INCDIR)
	ar $(ARFLAGS) $(UTILS_PATH)/libutils.a $(UTILS_PATH)/bin/*.o

clean_utils:
	-rm -f $(UTILS_PATH)/bin/*.o $(UTILS_PATH)/*.a
	
varhandle:
	$(CC) $(CFLAGS) $(VAR_PATH)/integer.c -o $(VAR_PATH)/bin/integer.o
	$(CC) $(CFLAGS) $(VAR_PATH)/string.c -o $(VAR_PATH)/bin/string.o
	$(CC) $(CFLAGS) $(VAR_PATH)/management.c -o $(VAR_PATH)/./bin/management.o
	$(CC) $(CFLAGS) $(VAR_PATH)/print.c -o $(VAR_PATH)/bin/print.o
	ar $(ARFLAGS) $(VAR_PATH)/libvarhandle.a $(VAR_PATH)/bin/*.o

clean_varhandle:
	-rm -f $(VAR_PATH)/bin/*.o $(VAR_PATH)/*.a

var2name:
	$(CC) $(CFLAGS) -o $(NAMES_PATH)/bin/var2name.o $(NAMES_PATH)/var2name.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(NAMES_PATH)/bin/genpage.o $(NAMES_PATH)/genpage.c $(INCDIR)
	ar $(ARFLAGS) $(NAMES_PATH)/libvar2name.a $(NAMES_PATH)/bin/*.o

clean_var2name:
	-rm -f $(NAMES_PATH)/bin/*.o $(NAMES_PATH)/*.a

test_var2name:
	$(CC) -g -o $(NAMES_PATH)/testprogs/main $(NAMES_PATH)/testprogs/main.c \
		$(INCDIR) $(LIBS)

webserver:
	$(CC) $(CFLAGS) -o $(WEB_PATH)/bin/lookup.o $(WEB_PATH)/lookup.c $(INCDIR)
	$(CC) $(CFLAGS) -o $(WEB_PATH)/bin/filehandling.o $(WEB_PATH)/filehandling.c  $(INCDIR)
	$(CC) $(CFLAGS) -o $(WEB_PATH)/bin/response.o $(WEB_PATH)/response.c  $(INCDIR)
	$(CC) $(CFLAGS) -o $(WEB_PATH)/bin/header.o $(WEB_PATH)/header.c  $(INCDIR)
	$(CC) $(CFLAGS) -o $(WEB_PATH)/bin/start.o $(WEB_PATH)/main.c  $(INCDIR)
	ar $(ARFLAGS) $(WEB_PATH)/libwebserver.a $(WEB_PATH)/bin/*.o

clean_webserver:
	-rm -f $(WEB_PATH)/bin/*.o $(WEB_PATH)/*.a

test_webserver:
	$(CC) -o $(WEB_PATH)/test/main $(WEB_PATH)/test/main.c \
		$(INCDIR) $(LIBS)

glob_test:
	$(CC) -g -o $(HOMEDIR)/test/main $(HOMEDIR)/test/main.c $(INCDIR) $(LIBS)


clean:
	$(MAKE) clean_cfgreader
	$(MAKE) clean_hexdump
	$(MAKE) clean_jinjaparser
	$(MAKE) clean_ldaparser
	$(MAKE) clean_sockets
	$(MAKE) clean_utils
	$(MAKE) clean_varhandle
	$(MAKE) clean_var2name
	$(MAKE) clean_webserver

all:
	$(MAKE) cfgreader
	$(MAKE) hexdump
	$(MAKE) jinjaparser
	$(MAKE) ldaparser
	$(MAKE) sockets
	$(MAKE) utils
	$(MAKE) varhandle
	$(MAKE) var2name
	$(MAKE) webserver
