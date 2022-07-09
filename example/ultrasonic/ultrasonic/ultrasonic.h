#ifndef ULTRASONIC_ULTRASONIC_H
#define ULTRASONIC_ULTRASONIC_H

#include "main.h"

void delay_us(__IO uint32_t delay);
void ULTRASONIC_init();
void ULTRASONIC_start();
double ULTRASONIC_read();

#endif //ULTRASONIC_ULTRASONIC_H
