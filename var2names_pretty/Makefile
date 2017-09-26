INCLUDE = -I./src/ \
		  -I../libs/varhandle_lib/src/ \
		  -I../libs/nat-lda-parser/src/ \
		  -I../natuser_lib/src/ \
		  -I../libs/utils/src/

LIBS = -L../libs/varhandle_lib/ -lldaparser\
	   -L../libs/nat-lda-parser/ -lvars \
	   -L../libs/utils/ -lutils

all:
	gcc -g -c p_print.c $(INCLUDE) -o ./bin/p_print.o
	gcc -g main.c $(INCLUDE) $(LIBS) -o ./main ./bin/*.o
