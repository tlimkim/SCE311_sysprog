#include <stdio.h>
#include <wiringPi.h>
#define PIR 29       //BCM_GPIO 23

int Pircheck (void)
{
  int ret;

  if(wiringPiSetup() == -1)
    return 1;

  pinMode(PIR,INPUT);
  for(;;)
  {
    int read=digitalRead(PIR);
    if(read)
    {
      printf("Detection\n");
      return 1;    
      delay(1000);
    }
    else{
      printf("Not Detected read: %d\n",read);
      return 0;
      delay(1000);
    }

  }
  return ret;
}
