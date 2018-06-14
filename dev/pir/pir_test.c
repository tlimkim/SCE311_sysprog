/*
 * Developed by Jiwon Jeon
 *
 * pir_test.c
 * device driver test code
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>

#define NODE_NAME "/dev/pir_dev"

int main(int argc, char * argv[]) {
  
  int fd,ret,stat;
  if ((fd = open(NODE_NAME, O_RDONLY | O_NONBLOCK)) < 0) {
    perror("open: ");
  }

  while(1){
    ret=read(fd,&stat, sizeof(stat));
    printf("%d byte read, stat=%d\n", ret,stat);
    usleep(500000); 
  }
}

