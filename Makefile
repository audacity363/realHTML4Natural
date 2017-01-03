CC = gcc
INCLUDES = -I./vars/
//CARGS = -Wall -g
CARGS = -g

all:
	make vars_lib
	$(CC) -c $(CARGS) token_handling.c -o token_handling.o $(INCLUDES)
	$(CC) -c $(CARGS) command_parsing.c -o command_parsing.o $(INCLUDES)
	$(CC) -c $(CARGS) variablen_parsing.c -o variablen_parsing.o $(INCLUDES)
	$(CC) -c $(CARGS) if.c -o if.o $(INCLUDES)
	$(CC) -c $(CARGS) exec_if.c -o exec_if.o $(INCLUDES)
	$(CC) -c $(CARGS) for.c -o for.o $(INCLUDES)
	$(CC) -c $(CARGS) json.c -o json.o $(INCLUDES)
	$(CC) -c $(CARGS) macro.c -o macro.o $(INCLUDES)
	$(CC) -c $(CARGS) exec_macro.c -o exec_macro.o $(INCLUDES)
	$(CC) -g main.c -o main ./*.o $(INCLUDES) -L./vars -lvars

vars_lib:
	cd ./vars/ && make

#list all TODO tags in comments
todo:
	grep -rn --exclude="*.swp" TODO: ./ | grep -v Makefile

clean:
	@rm -f ./*.o
	@rm -f main

