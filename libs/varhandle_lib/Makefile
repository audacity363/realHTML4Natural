CC = gcc
all:
	$(CC) -g -c integer.c -o ./bin/integer.o
	$(CC) -g -c string.c -o ./bin/string.o
	$(CC) -g -c management.c -o ./bin/management.o
	$(CC) -g -c print.c -o ./bin/print.o
	ar -vcru libvarhandle.a ./bin/*.o
clean:
	-rm -f ./bin/*.o
	-rm -f ./*.a
