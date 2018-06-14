/*
 * Developed by Jiwon Jeon
 *
 * dht11.c 
 * read_dht11_dat() is for reading dht11 (temperature - humidity sensor) read value.
 * implented by wiringPi.
 */
#include "main.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS    83
#define DHTPIN        7 //wPi pin. physical num 7

int dht11_dat[5] = {0, 0, 0, 0, 0};
int lcdflag_ = 0;

int read_dht11_dat()
{
    if (wiringPiSetup() == -1) {
	    exit(1);
    }
    delay(1000);

  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  float f; 
  int loop=0;

  dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(18);
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(35);
  pinMode(DHTPIN, INPUT);
 
  for (i=0;i<MAXTIMINGS;i++)
  {
    loop=i;
    counter = 0;
    while (digitalRead(DHTPIN)==laststate)
    {
      counter++;
      delayMicroseconds(1);
      if (counter==255)
        break;
    }
    laststate = digitalRead(DHTPIN);

    if (counter==255) 
      break;
    
    if ((i>=4)&&(i%2==0))
    {
      dht11_dat[j/8]<<=1;
      if (counter>25)
        dht11_dat[j/8] |= 1;
        j++;
    }
  }

  if ((j>=40)&&(dht11_dat[4]==((dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3])&0xFF))) {

    f = dht11_dat[2] * 9. / 5. + 32;
    printf("[Hum] %d.%d %% \n[Temp] %d.%d C (%.1f F)\n",      
    dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
    
    if (dht11_dat[0] >= 78) {
      lcd_print("Your Baby Pee");
      lcdflag_ = 1;
    } else if (dht11_dat[0] <= 78 && lcdflag_ == 1) {
      lcd_clear();
    }
  } else {
    printf("Data not good, skip\n");
    return 0;
  }
  return dht11_dat[2];
}

