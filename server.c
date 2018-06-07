#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <softPwm.h>

#define SERVO 1



void error(char *msg) {
	perror(msg);
	exit(1);
}

int getData(int sockfd) {
	char buffer[32];
	int n;

	(n = read(sockfd, buffer, 31));
	buffer[n] = '\0';
	return atoi(buffer);
}


int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno = 8000, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	int data;
	int cnt = 0;
	char str;

	if (wiringPiSetup() == -1)
		return 1;

	softPwmCreate(SERVO, 0, 200);

	printf("using port #%d\n", portno);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *)&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr));
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	//--- infinite wait on a connection ---
	while (1) {
		printf("waiting for new client...\n");
		if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen)) < 0)

			printf("opened new communication with client\n");
		while (1) {
			//---- wait for a number from client ---
			data = getData(newsockfd);
			if (data == 1) {
				delay(1000);
				softPwmWrite(SERVO, 24);
			}
			else if (data == 0) {
				delay(1000);
				cnt++;
				if (cnt > 5) {
					softPwmWrite(SERVO, 15);
					//delay(1000);
					//digitalWrite(SERVO, LOW);
				}

			}
			

			printf("got %d\n", data);
		}

		close(newsockfd);


	}

	return 0;
}
