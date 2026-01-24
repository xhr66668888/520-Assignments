test: main.o unit_test.o complex.o
	gcc -o test main.o unit_test.o complex.o -lm

main.o: main.c complex.h unit_test.h
	gcc -c -g -Wall main.c

unit_test.o: unit_test.c complex.h unit_test.h
	gcc -c -g -Wall unit_test.c

complex.o: complex.c complex.h
	gcc -c -g -Wall complex.c

clean:
	rm -f *.o test
