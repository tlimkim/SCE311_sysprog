#include <wiringPi.h>
#include <stdio.h>

#define BeepPin 24

int buzzer(void)
{
  int i = 0;

  if (wiringPiSetup() == -1){ 
    printf("setup wiringPi failed !");
    return 1; 
  }

  pinMode(BeepPin, OUTPUT);   //set GPIO0 output
 
  digitalWrite(BeepPin, HIGH);
  /*
  while (i == 1000) {
    digitalWrite(BeepPin, HIGH);
    delay(10000);
    digitalWrite(BeepPin, LOW);
    delay(10000);
    i++;
  }
  */
  return 0;
}
