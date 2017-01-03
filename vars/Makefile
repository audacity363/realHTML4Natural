CC = gcc
all:
	$(CC) -g -c utils.c -o utils.o
	$(CC) -g -c integer.c -o integer.o
	$(CC) -g -c string.c -o string.o
	$(CC) -g -c bool.c -o bool.o
	$(CC) -g -c float.c -o float.o
	$(CC) -g -c print.c -o print.o
	ar -vcru libvars.a ./*.o

clean:
	@rm -f ./*.o
	@rm -f ./*.a

test:
	gcc -g ./test_src/main.c -o ./main -I./ -L./ -lvars
