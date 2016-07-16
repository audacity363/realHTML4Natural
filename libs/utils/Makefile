all:
	xlc -c utils.c -o ./bin/utils.o 
	ar -cru libutils.a ./bin/*.o

testprog:
	xlc -o ./test/main ./test/main.c -I./ -L./ -lutils

clean:
	-rm ./bin/*.o
	rm ./*.a
