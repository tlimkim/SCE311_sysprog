#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <fcntl.h>
#include <pthread.h>

pthread_t thread;  /* User Thread */

int thread_start_flag = 1;
int thr_id;

#define NODE_NAME "/dev/pir_dev"

int main(int argc, char * argv[]) {
  
  int fd,ret,stat;
  fd = open(NODE_NAME, O_RDONLY);

  while(1){
    ret=read(fd,&stat, sizeof(stat));
    printf("%d byte read, stat=%d\n", ret,stat);
    usleep(500000); 
  }
}

