all:
	gcc -c utils.c -o ./bin/utils.o 
	ar -vcru libutils.a ./bin/*.o

testprog:
	gcc -o ./test/main ./test/main.c -I./ -L./ -lutils

clean:
	-rm ./bin/*.o
	rm ./*.a
