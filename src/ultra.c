/*
 * Developed by Taeklim Kim
 *
 * ultra.c check the distance between dangerous things and Baby.
 * There are saftey range between them, and the sensor checks it 
 * every seconds.
 * This function also calls the LCD, so that prints some message
 * while the baby is in danger.
 */

#include "main.h"

#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#define trigPin 4
#define echoPin 5

int lcdflag = 0;

int sonic_distance(void)
{
  int distance = 0;
  int count = 0;

  int start_time, travel_time = 0;
  int timeout = 0;
  int duration = 0;

  wiringPiSetup();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(trigPin, LOW);
  sleep(1);
  digitalWrite(trigPin, HIGH);
  usleep(20);
  digitalWrite(trigPin, LOW);

  // Send the Sonic and start the time
  start_time = micros();
  while(digitalRead(echoPin) == LOW) {
    timeout = micros();
    if ((timeout - start_time) >= 10000) {
      return 0;
    }
  }
  // Receive the sonic that sent before
  while(digitalRead(echoPin) == HIGH) { 
    travel_time = micros();
  }

  // Find out the time that sensor checks
  duration = travel_time - start_time - 650;
  
  distance = duration / 58;

  printf("[Dist]: %dcm\n", distance);

  // If a baby is close enough at dangerous thing, 
  // it calls LCD and send the message to LCD.
  if (distance <= 5) {
    lcd_print("Dangerous Baby \n");
    lcdflag = 1;
    return 1;

  // Release the message if a baby is gone.
  } else if (distance > 5 && lcdflag == 1 ) {
    //lcd_clear();
    return 0;
  }

  return 0;
}	
