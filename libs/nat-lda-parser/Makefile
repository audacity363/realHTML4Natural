OBJ = unix.o start.o

INCLUDE = -I/home/tom/Documents/realHTMLparser/vars/

LIBS = -L/home/tom/Documents/realHTMLparser/vars/ -lvars

lib: $(OBJ)
	ar -vcru libldaparser.a ./bin/*.o
	
%.o: ./src/%.c
	gcc -g -c $< $(INCLUDE) -o ./bin/$@

clean:
	rm -rf libldaparser.a
	rm -rf ./bin/*.o

test:
	make lib
	gcc -g ./main.c $(INCLUDE) -o ./main -L./ -lldaparser -I./src/ $(LIBS)
