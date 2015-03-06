Quash: main.o
	gcc main.o -o Quash -l readline

main.o: main.c
	gcc -c -g main.c -l readline

clean:
	rm -f *.o Quash
