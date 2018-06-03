#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>


#define butPin 23
#define ledPin 17


void *camera(void *arg){
	if(wiringPiSetupGpio()==-1)
		return 1;


	pinMode(butPin,INPUT);
	pinMode(ledPin,OUTPUT);
	//pullUpDnControl(butPin,PUD_UP);
	
		while(1){	
		int ret=digitalRead(butPin);
		//digitalWrite(ledPin,LOW);
		
		if(ret==0)
		{	
			
			//digitalWrite(ledPin,HIGH);
			printf("pressed!");
			system("raspistill -o image.jpg");

			//signal(SIGINT,(void *)sig_handler);
			

		
		}
		else if(ret==1){
			delay(5);
			printf("w");
		}

		/*char input[10];
		scanf("%s",&input);
		if(input=="exit"){
			break;
		}*/


		}


}

int main(void){
	char mess[]="";
	int res;
	pthread_t t;
	void *thread_result;
	res=pthread_create(&t, NULL, camera,(void *)mess);
	res=pthread_join(t,&thread_result);
	//camera();
	exit(EXIT_SUCCESS);
	return 0;
}


