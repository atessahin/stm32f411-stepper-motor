#include "functions.h"
int main()
{
	Stepper_Init();
	while(1)
	{
		Stepper(360,RIGHT,800);
		Delay_ms(1000);
		Stepper(360,LEFT,190);
	}

}