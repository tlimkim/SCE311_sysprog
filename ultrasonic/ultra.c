#include "main.h"

#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define trigPin 4
#define echoPin 5

int lcdflag = 0;

void sonic_distance(void)
{
    int distance = 0;
    int count = 0;

    int start_time, travel_time = 0;
    int duration = 0;

    wiringPiSetup();

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    while (1)
    {
	digitalWrite(trigPin, LOW);
	sleep(1);
	digitalWrite(trigPin, HIGH);
	usleep(20);
	digitalWrite(trigPin, LOW);

	while(digitalRead(echoPin) == LOW)
	    start_time = micros();

	while(digitalRead(echoPin) == HIGH) 
	    travel_time = micros();
    
	duration = travel_time - start_time;

	int distance = duration / 58;

	printf("Distance: %dcm\n", distance);

	if (distance <= 5) {
	    lcdprint("Dangerous Baby");
	    lcdflag = 1;
    	    continue;
	} else if (distance > 5 && lcdflag ==1 ) {
	    lcdclear();
	}
    }
}	
