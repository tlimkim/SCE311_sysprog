#include <stdio.h>
#include <wiringPi.h>
#include <pcf8591.h>
#include <softPwn.h>

#define PCF 120
#define SERVO 1
char dir[6][10]={"left", "right", "up", "down", "button", "home"};

int Joystick_Read(void);
void servo_control(int tmp);

int main (void)
{
 int tmp;
 wiringPiSetup () ;
 // Setup pcf8591 on base pin 120, and address 0x48
 pcf8591Setup (PCF, 0x48) ;
 softPwmCreate(SERVO,0,200);

 while(1) // loop forever
 {
 	tmp = Joystick_Read();
 	servo_control(tmp);
 	delay (100) ;
 }
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

void servo_control(int tmp)
{
		if(tmp== 1)softPwmWrite(SERVO,15);	//0  degree
		else if(tmp== 2)softPwmWrite(SERVO,24);//90 degree
		else if(tmp== 3)softPwmWrite(SERVO,5);//-90 degree
		else if(tmp== 4)return;
}
