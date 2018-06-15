/*
 * Developed by Jiwon Jeon.
 *
 * pir_wiring.c uses pir device driver to read sensor read value.
 * device driver code for PIR is in /dev/pir/
 */
#include "main.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <fcntl.h>

#define NODE_NAME "/dev/pir_dev"

int read_pir_dat(void)
{
  int fd,ret,stat;
  fd = open(NODE_NAME, O_RDONLY);

  ret = read(fd,&stat, sizeof(stat));

  printf("[PIR] sensor value: %d\n", stat);
  usleep(500000);
  
  return stat;
}
