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
    printf("In loop1 \n");
    if (sonic_distance() == 1) {
      printf("LED \n");
      ledOn();
    }
    if (sonic_distance() == 1)
      printf("LED \n");
    goto FUNC2;
  }

  while(1) {

FUNC2:
    printf("In loop2 \n");
    read_dht11_dat();
    goto FUNC1;
  }
  //sonic_distance();
  //read_dht11_dat();        


  return 0;
}
