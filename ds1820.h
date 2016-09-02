#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DS1820_DDR		DDRA
#define DS1820_PIN		PINA
#define DS1820_BIT		(1<<PA1)

uint8_t ds1820_start();
int ds1820_read();
