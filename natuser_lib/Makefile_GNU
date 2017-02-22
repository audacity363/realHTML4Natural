OBJ = ncuxinit.o flipBuffer.o gen_page.o

INCLUDE = -I./src/ \
		  -I../include/ \
		  -I../libs/varhandle_lib/src/

LIBS = 

lib: $(OBJ)
	gcc -shared -o librealHTML4Natural.so ./bin/*.o
	@echo "Done..."
	
%.o: ./src/%.c
	gcc -g -fpic -c $< $(INCLUDE) -o ./bin/$@

clean:
	rm -rf ./librealHTML4Natural.so
	rm -rf ./bin/*.o

test:
	make lib
	gcc -g ./test_src/main.c $(INCLUDE) -o ./test -L./ -lldaparser -I./src/ $(LIBS)
