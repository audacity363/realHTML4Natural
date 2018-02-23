CC = xlc
INCLUDES = -I./include

OBJS = rh4n_vars.o \
	   rh4n_vars_group.o \
	   rh4n_vars_print.o \
	   rh4n_vars_string.o \
	   rh4n_vars_array.o \
	   rh4n_vars_bool.o \
	   rh4n_vars_integer.c \
	   rh4n_vars_float.c \
	   rh4n_vars_ustring.c 

lib: clean $(OBJS)
	@printf "Creating librh4nvars.a\n"
	@ar -vcru librh4nvars.a
	@printf "Done..\n"

$(OBJS):
	@printf "CC ./src/$*.c => ./bin/$*.o\n"
	@$(CC) -c -g $(INCLUDES) -o ./bin/$*.o ./src/$*.c

test: lib
	@$(CC) -g $(INCLUDES) -o ./test/main ./test/main.c ./bin/*.o

clean:
	@printf "Cleaning libvar objects\n"
	@rm -rf ./bin/*.o
	@printf "Cleaning library object\n"
	@rm -rf ./librh4nvars.a
