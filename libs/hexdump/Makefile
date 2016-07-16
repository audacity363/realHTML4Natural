all:
	xlc -c -o ./bin/hexdump.o hexdump.c
	ar -vcru libhexdump.a ./bin/*.o

testprog:
	xlc -o ./test/main ./test/main.c -I./ -L./ -lhexdump

clean:
	-rm -f *.o *.a 
