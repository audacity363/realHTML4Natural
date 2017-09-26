all:
	xlc -c sockets.c -o ./bin/sockets.o
	ar -vcru libsockets.a ./bin/*.o

testprog:
	xlc -o ./test/main ./test/main.c -I./ -L./ -lsockets

clean:
	rm *.o *.a
