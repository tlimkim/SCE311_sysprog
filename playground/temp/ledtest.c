#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kdev_t.h>

#define DEV_PATH ""

void ledOn ()
{ 
  int fd = 0;

  if ((fd = open(DEV_PATH, O_RDWR | O_NONBLOCK)) < 0) {
    perror("open()");
    exit(1);
  }

  printf("LED Success \n");

  close(fd);
}
