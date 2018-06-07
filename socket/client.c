#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>


void error_handle(char* msg)
{
  puts(msg);
  exit(1);
}

void sendData( int sockfd, int x ) {

  int n;
  char buffer[32];
  
  sprintf( buffer, "%d\n", x );
  if ((n = write(sockfd, buffer, strlen(buffer))) < 0) {
    perror("write error: ");
    exit(0);
  }
  buffer[n] = '\0';
}

int main (void)
{
  int sock = 0;
  struct sockaddr_in serv_addr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
   printf("error on sock \n"); 
  }

  printf("sock: %d \n");

  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("192.168.30.253");
  serv_addr.sin_port = htons(51717);

  if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
    perror("connect error: ");
    exit(0);
  }
  
  sendData(sock, 1);

  sleep(5);

  close(sock);

  return 0;
}

