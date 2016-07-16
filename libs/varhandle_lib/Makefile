all:
	xlc -g -c integer.c -o ./bin/integer.o
	xlc -g -c string.c -o ./bin/string.o
	xlc -g -c management.c -o ./bin/management.o
	xlc -g -c print.c -o ./bin/print.o
	ar -vcru libvarhandle.a ./bin/*.o
clean:
	-rm -f ./bin/*.o
	-rm -f ./*.a
