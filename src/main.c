/*
 * Developed by TaekLim Kim
 *
 * In this main.c, it calls several sensors like ultrasonic, Humid, 
 * Sound and also LCD & LED outputs.
 * This function works as control part of the system 
 */

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

void sendData(int sockfd, int x)
{
  int n;

  char buf[32];
  sprintf(buf, "%d\n", x);

  if ((n = write(sockfd, buf, strlen(buf))) < 0) {
    perror("write error: ");
  }
  
  buf[n] = '\0';
}

int main(void) 
{
  printf("Module Started \n");
  
  int sonic = 0;
  int temp = 0;

  int sockfd, portno = 8888, n;
  char serverIp[] = "192.168.23.42";
  struct sockaddr_in serv_addr;
  struct hostent *server;
  
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    perror("Open Error: ");

  if ((server = gethostbyname(serverIp)) == NULL)
    perror("no such host \n");

  bzero((char*)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))< 0)
    perror("Connect Error: ");

  while(1) {

FUNC1:
    if (sonic != 1)
      lcd_print("babycare!", temp);
    // Call for checking Distance
    sonic = sonic_distance();
    if (sonic == 1) { // sonic_distance() == 1 means that Baby is close
      printf("[LED]\n");
      ledOn();
      buzzer();
      // add buzzer

    } else { // Off LED when baby is leaving
      ledOff();
    }
    goto FUNC2;
  }

  while(1) {

FUNC2:
    // Call for checking Humidity
    temp = read_dht11_dat();
    if (read_pir_dat() == 1) {
      sendData(sockfd, 1);
    } else {
      sendData(sockfd, 0);
    }

    goto FUNC1;
  }

  close(sockfd);
  return 0;
}
