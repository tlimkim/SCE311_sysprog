#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define butPin 23
#define ledPin 17

int camera(void){
	if(wiringPiSetupGpio()==-1)
		return 1;


	pinMode(butPin,INPUT);
	pinMode(ledPin,OUTPUT);
	//pullUpDnControl(butPin,PUD_UP);
	
	
		int ret=digitalRead(butPin);
		const char *command="raspistill -o image.jpg";
		FILE *fp=NULL;

		
		while(1){
		if(ret==0)
		{		
			//digitalWrite(ledPin,HIGH);
			printf("pressed!");
			//system("raspistill -o image.jpg");
			fp=popen(command,"r");
			pclose(fp);
			printf("close\n");
		
		}
		else if(ret==1){
			delay(5);
			printf("w");
		}
		
		}


}
int main(void){
camera();
return 0;
}
