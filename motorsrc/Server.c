#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <softPwm.h>
#include <pcf8591.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h>

#include <unistd.h> 

#define DEV_PATH "/dev/button_dev"
#define PCF 120
#define SERVO1 1
#define SERVO2 7
#define butPin 4
#define BUFFER_LENGTH 256


int Joystick_Read(void);
void servo_control(int tmp);
void *thread_joystick(void *arg);
void *thread_Server(void *arg);
void *thread_Camera(void *arg);
int camera(void);
void errorHandl(char* msg);
int getData(int sockfd);
void TCP_Configuration(void);
char message[] = "";
int fd;

int sockfd, newsockfd, portno = 8888, clilen; //port 8888
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int n;
int data;

int main(void){
	int tmp;
	wiringPiSetup ();
	pinMode(butPin,INPUT);	
	TCP_Configuration(); //server socket

	 if ((fd = open("/dev/button_dev", O_RDWR | O_NONBLOCK)) < 0) { 
    	perror("open()");
   	 exit(1);
  	}
 	
 	pcf8591Setup (PCF, 0x48) ; // Setup pcf8591 on base pin 120, and address 0x48
 	softPwmCreate(SERVO1,0,200);
	softPwmCreate(SERVO2,0,200);

	int res[3];
  	pthread_t a_thread[3];
  	void *thread_result[3];
	
	//create joystick, server, camera thread 
  	res[0] = pthread_create(&a_thread[0], NULL, thread_joystick, (void *)message);
 	res[1] = pthread_create(&a_thread[1], NULL, thread_Server, (void *)message);
	res[2] = pthread_create(&a_thread[2], NULL, thread_Camera, (void *)message);

 	if (res[0] != 0) {
   	    perror("Thread0 creation Joystick failed");
    	    exit(EXIT_FAILURE);
  	}
	if (res[1] != 0) {
   	    perror("Thread1 creation Server failed");
    	    exit(EXIT_FAILURE);
  	}
	if (res[2] != 0) {
   	    perror("Thread2 creation Server failed");
    	    exit(EXIT_FAILURE);
  	}
  	  printf("Waiting for thread to finish...\n");
 		
	res[0] = pthread_join(a_thread[0], &thread_result[0]);
	res[1] = pthread_join(a_thread[1], &thread_result[1]);
	res[2] = pthread_join(a_thread[2], &thread_result[2]);

 	if (res[0] != 0) {
   	    perror("Thread0 join failed");
    	    exit(EXIT_FAILURE);
   	}
	if (res[1] != 0) {
   	    perror("Thread1 join failed");
    	    exit(EXIT_FAILURE);
   	}
	if (res[2] != 0) {
   	    perror("Thread2 join failed");
    	    exit(EXIT_FAILURE);
   	}
   	printf("Thread joined, it returned %s\n", (char *)thread_result);
   	printf("Message is now %s\n", message);
	close(fd);
    	exit(EXIT_SUCCESS);
 	return 0 ;
}


int Joystick_Read(void)
{
	int tmp, value[3];

	value[0] = analogRead (PCF + 0) ;
 	//printf("x = %d ", value[0]);
 	value[1] = analogRead (PCF + 1) ;
 	//printf("y = %d ", value[1]);
 	value[2] = analogRead (PCF + 2) ;
 	//printf("z = %d \n", value[2]);
 	if(value[0]==value[1] && value[1]==value[2])
    tmp = 0; // printf("%s\n", dir[0]); // left
 	if(value[1] > 250 && value[0] < 225 && value[2] < 225)
    tmp = 2; // printf("%s\n", dir[2]); // up
 	if(value[1] < 200 && value[1] != value[2] && value[1] != value[0]) // down
    tmp = 3; // printf("%s\n", dir[3]);
 	if(value[2]<10 && value[0] > 150 && value[0] < 200 && value[1] >200 && value[1] <250)  // right
    tmp = 1; // printf("%s\n", dir[1]);
 	if(value[0]<10 && value[1] >200 && value[1] < 250 && value[2] > 150 && value[2] <200) // button
    tmp = 4; // printf("%s\n", dir[4]);

    return tmp;
}

/**/
void servo_control(int tmp)
{
		//if(tmp==0)softPwmWrite(SERVO2,19); // 90 degree
		//else if(tmp== 1)softPwmWrite(SERVO2,11);//-90 degree
		if(tmp== 2)softPwmWrite(SERVO1,16);//90 degree
		else if(tmp== 3)softPwmWrite(SERVO1,14);//-90 degree
		else if(tmp== 4){
		softPwmWrite(SERVO1,15); //0  degree
		//softPwmWrite(SERVO2,15);
		}
}

void *thread_joystick(void *arg) {
	while(1)
	{
	int tmp = Joystick_Read();
	servo_control(tmp);
	delay(100);
	}
	return((void *)0);
}

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

/*create server socket and wait client*/
void TCP_Configuration(void)
{
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   bzero((char *)&serv_addr, sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   bind(sockfd, (struct sockaddr *) &serv_addr,
      sizeof(serv_addr));
   listen(sockfd, 5);
   clilen = sizeof(cli_addr);

   
}
void *thread_Camera(void *arg){

		
		
		
		FILE *fp=NULL;
		int ret, receive, cnt=0;
		char fname[256];
		 while(1){
      		int ret = read(fd, &receive, sizeof(receive));
      		time_t t = time(NULL);
		struct tm tm =*localtime(&t);
      
     		 if(receive == 0){ 
	   	 if(cnt > 1){   
     		 char command[30] = "raspistill -o "; 
       		  sprintf(fname,"%d-0%d-0%d-%d:%d:%d",
         		tm.tm_year+1900, tm.tm_mon +1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        		 printf("pressed!\n");
         	strcat(command, fname);
        	fp=popen(command,"r");
        	pclose(fp);
		char cmd[300];
		sprintf(cmd, "sudo mv %s /var/www/html",fname);
		fp=popen(cmd,"r");
		receive=1;
		pclose(fp);
		}
		else cnt++,receive=1;
      		}
      		}
}

		
/*When connected to client, motor operation according to pir value*/
void *thread_Server(void *arg)
{
	int cnt=0;
	//--- infinite wait on a connection ---
	while (1) {
		printf("waiting for new client...\n");
		if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen)) < 0) 

			printf("opened new communication with client\n");
		while (1) {
			//---- wait for a number from client ---
			data = getData(newsockfd); //Get data from client
			if (data == 1) { 
				delay(500);
				softPwmWrite(SERVO2,24);//Rotate the motor 90 degrees to the right
				delay(500);
				softPwmWrite(SERVO2,5); //Rotate the motor 90 degrees to the left
			}
			else if (data == 0) {
				cnt++;
				if (cnt > 5) {
					softPwmWrite(SERVO1, 15); //stop motor operation
				}

			}
			

			printf("got %d\n", data); //Output pir value from client
		}

		close(newsockfd);


	}
}
