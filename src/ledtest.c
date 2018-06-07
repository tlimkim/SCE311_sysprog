/*
 * Developed by Taeklim Kim
 * 
 * ledtest.c works for two function: ledOn() ledOff()
 * They just turn on the LED or turn off the LED
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>

#define DEV_PATH "/dev/led_dev"

int fd = 0;

// Turn on the LED if it called
void ledOn ()
{ 
  if ((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
    perror("open()");
    exit(1);
  }
}

// Turn off the LED if it called
void ledOff()
{
  close(fd);
}
