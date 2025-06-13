#include "functions.h"


void systemClockConfig()
{
	RCC->CR |=(1<<16);// hse yi aç
	while(!(RCC->CR & (1<<17)));//hse nin açilmasini bekle
	
	FLASH->ACR|=(3<<1)|(1<<8)|(1<<9)|(1<<10);
	//Flash erisim bekleme süresi
	//Prefetch buffer etkinlestirme
	//Instruction cache etkinlestirme
	//Data cache etkinlestirme
	
	RCC->APB1ENR|=(1<<28);
	PWR->CR |=(3U<<14);
	//PWR register'larina erismek için önce power interface
	//clock'unu etkinlestirmek gerekir

	RCC->CR &= ~(1 << 24);  //PLL'i kapat
	while(RCC->CR & (1<<25))
		;//kapanmasini bekle
	
	//pll hesaplari
  RCC->PLLCFGR = 		(8 << 0) |      // PLLM = 8 (bits 5:0)
                   (168 << 6) |    // PLLN = 168 (bits 14:6)
                   (0 << 16) |     // PLLP = 2 (bits 17:16) 00=÷2
                   (1 << 22);      // PLLSRC = HSE (bit 22)
									 
	 RCC->CR |= (1 << 24);  // PLLON
   while(!(RCC->CR & (1 << 25)));  // BEKLE PLLRDY		

		RCC->CFGR&=~(15U<<0);//AHB BÖLME ISLEMI YOK 84 MHZ
		RCC->CFGR|=(4U<<10);//APB1 /2 =42 MHZ (MAX ZATEN)
		RCC->CFGR&=~(4U<<13);//APB2 /1=84MHZ BÖLME YOK
	
	//sistem saati seç
		RCC->CFGR |= (2 << 0); 
	 while((RCC->CFGR & (3 << 2)) != (2 << 2));//SWS=SW OLANA KADAR BEKLE
	SystemCoreClock=84000000;//SISTEM CORE CLOCK GÜNCELLE
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
		Hexadecimal -> Binary -> Hangi pin aktif
		0x01 = 0000 0001  PA0 aktif
		0x02 = 0000 0010 	PA1 aktif  
		0x04 = 0000 0100  PA2 aktif
		0x08 = 0000 1000  PA3 aktif
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