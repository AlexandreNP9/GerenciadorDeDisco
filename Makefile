play:
	gcc -c Gerenciador.c
	gcc main.c Gerenciador.o -o execute
	./execute

remove:
	rm *.o
	rm execute