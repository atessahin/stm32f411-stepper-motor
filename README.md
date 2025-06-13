# stm32f411-stepper-motor
STM32F411E-DISCO + ULN2003A stepper motor driver register programming no HAL.
Controls a 28BYJ-48 stepper motor through direct register manipulation.
It allows precise angle-based rotation in both directions with adjustable speed.
The code initializes the STM32F411 to run at 84MHz and uses Timer2 for accurate timing.

# Features
Direct register (no HAL)
Angle control
Bidirectional rotation
Adjustable speed
Timer-based delays with microsecond precision

# Hardware Requirements        
STM32F411E-DISCO board
28BYJ-48 stepper motor
ULN2003A driver board

# USAGE
include "functions.h"
int main()
{
    Stepper_Init();
    while(1)
    {
        Stepper(90, RIGHT, 10);   // 90° clockwise, fast
        Delay_ms(1000);
        Stepper(180, LEFT, 190);  // 180° counter-clockwise, slow
        Delay_ms(1000);
    }
}

# Connections
Connect the STM32F411E-DISCO board to the ULN2003A driver board:
PA0 (Blue wire) → IN1
PA1 (Pink wire) → IN2
PA2 (Yellow wire) → IN3
PA3 (Orange wire) → IN4
GND → GND
5V → VCC


# Technical Details
System Clock: 84MHz (HSE + PLL)
Motor: 28BYJ-48 (2048 steps/revolution)
Driver: 4-phase unipolar stepper sequence
Timer: TIM2 for precise delays
