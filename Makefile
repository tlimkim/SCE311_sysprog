exec : lcdtest.o ultra.o main.o dht11.o
	gcc -o exec main.o ultra.o lcdtest.o dht11.o -lwiringPi -lwiringPiDev

ultra.o : ultra.c
	gcc -c -o ultra.o ultra.c -lwiringPi

lcdtest.o : lcdtest.c
	gcc -c -o lcdtest.o lcdtest.c -lwiringPi -lwiringPiDev

dht11.o : dht11.c
	gcc -c -o dht11.o dht11.c -lwiringPi -lwiringPiDev

main.o : main.c
	gcc -c -o main.o main.c

clean :
	rm *.o exec
