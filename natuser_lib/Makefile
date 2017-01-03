OBJ = ncuxinit.o flipBuffer.o send_values_to_page.o

INCLUDE = -I./src/ \
		  -I../include/

LIBS = 

lib: $(OBJ)
	@echo "Done..."
	
%.o: ./src/%.c
	gcc -g -fpic -c $< $(INCLUDE) -o ./bin/$@

clean:
	rm -rf libldaparser.a
	rm -rf ./bin/*.o

test:
	make lib
	gcc -g ./test_src/main.c $(INCLUDE) -o ./test -L./ -lldaparser -I./src/ $(LIBS)
