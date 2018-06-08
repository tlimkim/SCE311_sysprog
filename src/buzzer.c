#include <wiringPi.h>
#include <stdio.h>

#define BeepPin 0

int buzzer(void)
{
  if (wiringPiSetup() == -1){ 
    printf("setup wiringPi failed !");
    return 1; 
  }

  pinMode(BeepPin, OUTPUT);   //set GPIO0 output

  while(1){
    digitalWrite(BeepPin, LOW);  //beep on
    delay(100);                  //delay
    digitalWrite(BeepPin, HIGH); //beep off
    delay(100);                  //delay
  }

  return 0;
}
