CC = xlc

HOMEDIR = /u/it/a140734/C/realHtml4Natural/
#HOMEDIR = /home/tom/Documents/realHtml4Natural/
STANDARD_PATH    	=   $(HOMEDIR)/include
HEXD_PATH   = $(HOMEDIR)/libs/hexdump/

INCDIR = -I$(STANDARD_PATH) \
		 -I$(HEXD_PATH)

all:
	$(CC) -g -c integer.c -o ./bin/integer.o $(INCDIR)
	$(CC) -g -c string.c -o ./bin/string.o $(INCDIR)
	$(CC) -g -c management.c -o ./bin/management.o $(INCDIR)
	$(CC) -g -c print.c -o ./bin/print.o $(INCDIR)
	$(CC) -g -c unicode.c -o ./bin/unicode.o $(INCDIR)
	ar -vcru libvarhandle.a ./bin/*.o

clean:
	-rm -f ./bin/*.o
	-rm -f ./*.a

test:
	-rm ./test_exec
	$(CC) -g ./test_exec.c -o test_exec -I./ -L./ -lvarhandle -I$(HEXD_PATH) -L$(HEXD_PATH) -lhexdump
