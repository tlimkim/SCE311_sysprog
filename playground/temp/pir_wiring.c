#include <stdio.h>
#include <wiringPi.h>
#define PIR 4       //BCM_GPIO 23

int Pircheck (void)
{
  if(wiringPiSetup() == -1)
    return 1;

  pinMode(PIR,INPUT);
  for(;;)
  {
    int read=digitalRead(PIR);
    if(read)
    {
      printf("Detection\n");
      delay(1000);
    }
    else{
      printf("Not Detected read: %d\n",read);
      delay(1000);
    }

  }
  return 0;
}
