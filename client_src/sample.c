#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int TRIG = 23, ECHO = 24;

static int ping()
{
    long ping      = 0;
    long pong      = 0;
    float distance = 0;
    long timeout   = 500000; // 0.5 sec ~ 171 m

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // Ensure trigger is low.
    digitalWrite(TRIG, LOW);
    delay(50);

    // Trigger the ping.
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10); 
    digitalWrite(TRIG, LOW);

    // Wait for ping response, or timeout.
    while (digitalRead(ECHO) == LOW) {
    }
    

    // Cancel on timeout.
    if (micros() > timeout) {
	printf("Out of range. in first\n");
	return 0;
    }

    ping = micros();

    // Wait for pong response, or timeout.
    while (digitalRead(ECHO) == HIGH && micros() < timeout) {
    }

    // Cancel on timeout.
    if (micros() > timeout) {
	printf("Out of range. in second\n");
	return 0;
    }

    pong = micros();

    // Convert ping duration to distance.
    distance = (float) (pong - ping) * 0.017150;

    printf("Distance: %.2f cm.\n", distance);

    return 1;
}

int main (int argc, char *argv[])
{
    printf ("Raspberry Pi wiringPi HC-SR04 Sonar test program.\n");

    if (wiringPiSetup () == -1) {
	exit(EXIT_FAILURE);
    }

    if (setuid(getuid()) < 0) {
	perror("Dropping privileges failed.\n");
	exit(EXIT_FAILURE);
    }

    ping();

    return 0;
}
