// Библиотека для работы с LCD HD44780 диспоеем
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdint.h>

////////////////////////переопределения////////////////////////////////////////
//#define F_CPU 8000000L
#define PORT_SIG 	  PORTB//порт управления
#define DDR_SIG	      DDRB//
#define PIN_SIG   	  PINB//
#define E      	      PB0//строб
#define RW	   	   	  PB1//направление:1 чтение дисплея, 0 запись в дисплей
#define RS  	      PB2//1 передаем данные, 0 передаем команду
#define PORT_DATA	  PORTA//порт данных
#define DDR_DATA	  DDRA//
#define PIN_DATA	  PINA//
#define BF       	  PA7//флаг занятости если 1 то занят дисплей если 0 то свободен
#define RS_COM   	  0//передаем команду
#define RS_DATA  	  1//передаем данные
#define ON			  1//включение 
#define OFF			  0//выключение
#define BUS_LINE	  0x28//2 линии данных, 4 бита шина
#define SEC_LINE	  0xA8//Вторая линия первый символ
#define FIRS_LINE 	  0x80//Первая линия первый символ
#define FIRST		  0x80//Адрес первой ячейки 
#define THIRD		  0x82//
#define FOURTH		  0x83//Адрес четвертой ячейки	  
#define SEVENTH		  0x86//Адрес седьмой ячейки
#define EIGHTH		  0x87//
#define ELEVENTH	  0x8B//
#define THIRTEENTH 	  0x8C//
#define THIRD_S	      0xC2//
#define SEVENTH_S	  0xC6//
#define EIGHTH_S	  0xC8//
#define ELEVENTH_S 	  0xB2//
#define THIRTEENTH_S  0xCD//
#define FIFTEENTH_S   0xB6//
//////////////////////////////////////функции///////////////////////////////
void LCD_init();//функция инициализации дисплея 
void LCD_write(uint8_t data, uint8_t BIT_RS);//функция записи в дисплей
void LCD_read_BF();//функция чтения флага BF готовности из дисплея на данном железе НЕ РАБОТАЕТ!!!делаем через delay)))
void LCD_write_adress(uint8_t data, uint8_t adress);//функция записи в дисплей по конкретному адресу
void LCD_write_str(int8_t *string);//вывод строки на дисплей
void line_wrapping(int8_t number);//переключение на нужную строку
void clean_display();//очистка дисплея
void kursor_adress(uint8_t adress);//установка курсора на нужный адрес
void LCD_alignment(int16_t str, uint8_t adress);// функция выравнивания на дисплее значений
