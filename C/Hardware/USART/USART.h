#ifndef USTART_H
#define USTART_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <stdio.h>

void Usart1_Init(uint32_t bound);
void put_s(uint8_t *s);
#endif
