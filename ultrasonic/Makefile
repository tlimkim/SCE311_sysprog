exec : lcdtest.o ultra.o main.o
	gcc -o exec main.o ultra.o lcdtest.o -lwiringPi -lwiringPiDev

ultra.o : ultra.c
	gcc -c -o ultra.o ultra.c -lwiringPi

lcdtest.o : lcdtest.c
	gcc -c -o lcdtest.o lcdtest.c -lwiringPi -lwiringPiDev

main.o : main.c
	gcc -c -o main.o main.c


clean :
	rm *.o exec
