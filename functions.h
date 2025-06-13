#include "stm32f4xx.h"                  // Device header
#define RIGHT 1
#define LEFT  0

void systemClockConfig();
void Stepper(float angle, int direction, int speed);
void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);
void gpioConfig();
void timConfig();

void Stepper_Init(void);