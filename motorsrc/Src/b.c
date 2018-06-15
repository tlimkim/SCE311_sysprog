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

int sockfd, newsockfd, portno = 8888, clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int n;
int data;

/*
 * The main function basically sets
 * GPIO, pinmode, TCP / IP, pcf - ADC function, and Pwm.
 * Open the button device file in the kernel by file I / O.
 * We then create three threads to create the joystick, server, and camera threads, respectively. 
 * Then join the thread and exit.
 */
int main(void){
	int tmp;
	wiringPiSetup ();	// wiringPi set
	TCP_Configuration(); // TCP/IP socket set

	 if ((fd = open("/dev/button_dev", O_RDWR | O_NONBLOCK)) < 0) {	// open Button Device Driver
    	perror("open()");
   	 	exit(1);
  	}
 	
 	pcf8591Setup (PCF, 0x48) ; // Setup pcf8591 on base pin 120, and address 0x48
 	softPwmCreate(SERVO1,0,200); // Setup Servo1 Pwm 
	softPwmCreate(SERVO2,0,200); // Setup Servo2 Pwm 

	int res[3];
  	pthread_t a_thread[3];
  	void *thread_result[3];

  	res[0] = pthread_create(&a_thread[0], NULL, thread_joystick, (void *)message); // Create Therad function for Joystick
 	res[1] = pthread_create(&a_thread[1], NULL, thread_Server, (void *)message); // Create Therad function for Server
	res[2] = pthread_create(&a_thread[2], NULL, thread_Camera, (void *)message); // Create Therad function for Camera

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
 		
	res[0] = pthread_join(a_thread[0], &thread_result[0]);	// close thread function for Joystick
	res[1] = pthread_join(a_thread[1], &thread_result[1]);  // close thread function for Server
	res[2] = pthread_join(a_thread[2], &thread_result[2]);  // close thread function for Camera

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

/*
 * The joystick read function reads the analog value using the ADC.
 * If, the value is classified according to each x, y, z value, 
 * and the up, down, right, left, and key values ​​are classified.
 */

int Joystick_Read(void)
{
	int tmp, value[3];

	value[0] = analogRead (PCF + 0) ;	// read analog data for moved joystick 
 	value[1] = analogRead (PCF + 1) ;
 	value[2] = analogRead (PCF + 2) ;

 	if(value[0]==value[1] && value[1]==value[2]) tmp = 0; // left
 	else if(value[1] > 250 && value[0] < 225 && value[2] < 225) tmp = 2; // up
 	else if(value[1] < 200 && value[1] != value[2] && value[1] != value[0]) tmp = 3; // down
 	else if(value[2]<10 && value[0] > 150 && value[0] < 200 && value[1] >200 && value[1] <250) tmp = 1; // right
 	else if(value[0]<10 && value[1] >200 && value[1] < 250 && value[2] > 150 && value[2] <200) tmp = 4; // button

    return tmp;
}

/* 
 * It is a function to control servo motor.
 * Adjust servo motor pwm according to up, down, key value.
 */
void servo_control(int tmp)
{
		if(tmp== 2)softPwmWrite(SERVO1,16); //90 degree
		else if(tmp== 3)softPwmWrite(SERVO1,14); //-90 degree
		else if(tmp== 4)softPwmWrite(SERVO1,15); //0  degree
}

/*
 * It is a joystick thread function.
 * Continue reading the value of the joystick and control
 * the servo motor based on the read value.
 */
void *thread_joystick(void *arg) {
	while(1)
	{
	int tmp = Joystick_Read();	// read direction data from joystick
	servo_control(tmp);	// control the servo motor 
	delay(100);
	}
	return((void *)0);
}

/* 
 * If an error occurs in the server function, an error is output.
 */
void error(char *msg) {
	perror(msg);
	exit(1);
}

/*
 * The server function reads the data value from the client.
 */
int getData(int sockfd) {
	char buffer[32];
	int n;

	(n = read(sockfd, buffer, 31));
	buffer[n] = '\0';
	return atoi(buffer);
}

/*
 * It is a function that sets TCP / ip.
 * I created a socket, bound the socket via the TCP / IP address,
 * and set the socket to read data every 5 seconds.
 */
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

/*
 * It is a camera thread function.
 * Button Continue to operate the camera
 * in the while thread in the camera thread according to the operation of the interface.
 * I created a file with the current time using time.h.
 */

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

		
/*
 * It is a server thread function.
 * According to the data value read through the socket, 
 * if the data is 1, the pwm of the servo motor is controlled and 
 * the code is configured so that the motor rotates 90 degrees to 90 degrees.
 */
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
			data = getData(newsockfd);
			if (data == 1) { // when PIR detect baby
				delay(500);
				softPwmWrite(SERVO2,24); // motor control to foward direction
				delay(500);
				softPwmWrite(SERVO2,5);	// motor control to rverse direction
			}
			else if (data == 0) { // when PIR don't detect baby
				cnt++;
				if (cnt > 5) {
					softPwmWrite(SERVO1, 15); // motor don't operation
				}
			}
			printf("got %d\n", data);
		}
		close(newsockfd);
	}
}
