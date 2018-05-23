#include <wiringPi.h>
#include <unistd.h>
#include <stdlib.h>


#define butPin 23

int main(void){
	if(wiringPiSetup()==-1)
		return 1;


	pinMode(butPin,INPUT);
	
	while(1){
		int ret=digitalRead(butPin);
		if(ret)
		{
			system("raspistill -o image.jpg");
			//continue;
		}
		else
			continue;
	}
	return 0;

}
