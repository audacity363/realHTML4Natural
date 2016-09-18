CC=gcc
CFLAGS=-O3 -Wunused-result
HTTPOBJ = ./bin/httppil.o ./bin/http.o ./bin/httpxml.o ./bin/httphandler.o \
			./bin/httppost.o ./bin/httpauth.o ./bin/lookup.o ./bin/natural.o
HEADERS = httpint.h httpapi.h httpxml.h

TARGET = miniweb
HTTPOBJ += ./bin/miniweb.o

DEFINES=

OS="Linux"

DEFINES+= -DHTTP_DEBUG
LDFLAGS += -g


HOMEDIR = /home/tom//Documents/realHtml4Natural/

STANDARD_PATH    	=   $(HOMEDIR)/include
CFG_PATH     		= 	$(HOMEDIR)/libs/CfgReader/

INCDIR = -I$(STANDARD_PATH) \
		 -I$(CFG_PATH)


LIBS = -L$(CFG_PATH) -lcfg

%.o: ../%.c $(HEADERS)
	$(CC) $(DEFINES) -g -c -o $@ $(CFLAGS) $(INCDIR) $(filter %.c, $^) 

all: $(HTTPOBJ)
	@echo Building for $(OS)
	$(CC) $(LDFLAGS) $(HTTPOBJ) $(LIBS) -o $(TARGET)

min: $(HTTPOBJ) httpmin.o
	@echo Building for $(OS)
	$(CC) $(LDFLAGS) $(HTTPOBJ) httpmin.o -o httpd $(LIBS)

install: all
	@rm -f /usr/bin/$(TARGET)
	@cp $(TARGET) /usr/bin

clean:
	@rm -f $(TARGET) $(TARGET).exe
	@rm -f ./bin/*.o
	@rm -rf Debug Release
