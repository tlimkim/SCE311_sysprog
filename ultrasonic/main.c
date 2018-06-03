/*
 * Developed by TaekLim Kim
 *
 * In this main.c, it calls several sensors like ultrasonic, Humid, 
 * Sound and also LCD & LED outputs.
 * This function works as control part of the system 
 */

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main(void) 
{
  printf("Module Started \n");

  while(1) {

FUNC1:
    // Call for checking Distance
    // sonic_distance() == 1 means that Baby is close
    if (sonic_distance() == 1) {
      printf("LED \n");
      ledOn();

    } else { // Off LED when baby is leaving
      ledOff();
    }
    goto FUNC2;
  }

  while(1) {

FUNC2:
    // Call for checking Humidity
    read_dht11_dat();
    
    goto FUNC1;
  }
  return 0;
}
