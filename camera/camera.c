#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define butPin 23

int camera(void){
	if(wiringPiSetupGpio()==-1)
		return 1;


	pinMode(butPin,INPUT);
	//pullUpDnControl(butPin,PUD_UP);
	

		int ret=digitalRead(butPin);
		while(1){
		if(ret==0)
		{	
			printf("pressed!");
			system("raspistill -o image.jpg");
			
		
		}
		else if(ret==1){
			delay(5);
			printf("no");
		}
		
		}


}
int main(void){
camera();
}

