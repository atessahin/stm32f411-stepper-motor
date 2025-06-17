#include "functions.h"
void systemClockConfig()
{
   RCC->CR |=(1<<16);// Enable HSE
   while(!(RCC->CR & (1<<17)));//Wait for HSE to be ready
   
   FLASH->ACR|=(3<<1)|(1<<8)|(1<<9)|(1<<10);
   //Flash access wait state
   //Enable prefetch buffer
   //Enable instruction cache
   //Enable data cache
   
   RCC->APB1ENR|=(1<<28);
   PWR->CR |=(3U<<14);
   //To access PWR registers, first enable
   //power interface clock
   RCC->CR &= ~(1 << 24);  //Disable PLL
   while(RCC->CR & (1<<25))
   	;//Wait for PLL to turn off
   
   //PLL calculations
 RCC->PLLCFGR = 		(8 << 0) |      // PLLM = 8 (bits 5:0)
                  (168 << 6) |    // PLLN = 168 (bits 14:6)
                  (0 << 16) |     // PLLP = 2 (bits 17:16) 00=÷2
                  (1 << 22);      // PLLSRC = HSE (bit 22)
   								 
    RCC->CR |= (1 << 24);  // PLLON
  while(!(RCC->CR & (1 << 25)));  // WAIT FOR PLLRDY		
   	RCC->CFGR&=~(15U<<4);//NO AHB DIVISION 84 MHZ
   	RCC->CFGR|=(4U<<10);//APB1 /2 =42 MHZ (MAX ALREADY)
   	RCC->CFGR&=~(4U<<13);//APB2 /1=84MHZ NO DIVISION
   
   //Select system clock
   	RCC->CFGR |= (2 << 0); 
    while((RCC->CFGR & (3 << 2)) != (2 << 2));//WAIT UNTIL SWS=SW
   SystemCoreClock=84000000;//UPDATE SYSTEM CORE CLOCK
}
void timConfig()
{
   	RCC->APB1ENR|=(1<<0);
   TIM2->PSC = 84 - 1;
   TIM2->ARR = 0xFFFFFFFF;
   TIM2->CR1 |= (1 << 0);// Counter enable
}
void gpioConfig()
{
   	RCC->AHB1ENR|=(1<<0);
   GPIOA->MODER &= ~((3 << 0) | (3 << 2) | (3 << 4)|(3<<6));    // PA0=blue, PA1=pink, PA2=yellow, PA3=orange
   GPIOA->MODER |= (1 << 0) | (1 << 2) | (1 << 4)|(1<<6);
   
   GPIOA->OTYPER &= ~((1 << 0) | (1 << 1) | (1 << 2)| (1<<3));  
   
 
   GPIOA->OSPEEDR |= (3 << 0) | (3 << 2) | (3 << 4) | (3<<6);     
}
void Delay_us(uint32_t us)
{
   TIM2->CNT = 0;
   while(TIM2->CNT < us);
}
void Delay_ms(uint32_t ms)
{
   	Delay_us(ms * 1000);  
}
void Stepper(float angle, int direction, int speed)
{
   float perAngle = 360.0 / 2048.0;
   int stepNumber = (int)(angle / perAngle);
   
   uint8_t sequence[4] = {0x01, 0x02, 0x04, 0x08};  
/*
   	Hexadecimal -> Binary -> Which pin is active
   	0x01 = 0000 0001  PA0 active
   	0x02 = 0000 0010 	PA1 active  
   	0x04 = 0000 0100  PA2 active
   	0x08 = 0000 1000  PA3 active
*/
   
   for(int i = 0; i < stepNumber; i++)
   {
       if(direction == RIGHT) {
           GPIOA->ODR = sequence[i % 4];  
       }
       else if(direction == LEFT){
           GPIOA->ODR = sequence[3 - (i % 4)];  
       }
       
       Delay_ms(speed);
   }
   
   GPIOA->ODR = 0;
}
void Stepper_Init(void)
{
   systemClockConfig();
   gpioConfig();
   timConfig();
}
