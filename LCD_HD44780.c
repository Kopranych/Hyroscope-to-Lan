// Библиотека для работы с LCD HD44780 диспоеем


#include "LCD_HD44780.h"



/////////////////////////////////////////

void LCD_init()
{	
	_delay_ms(40);//
	DDR_DATA |= 0xF0;//порты на 
	PORT_DATA |= 0xF0;//выход
	DDR_SIG |= (ON<<E)|(ON<<RW)|(ON<<RS);//управляющие порты 
	PORT_SIG |= (ON<<E)|(ON<<RS);//на выход
	PORT_SIG &= ~(ON<<RW);//порт чтения/записи на на запись
	_delay_ms(40);
	LCD_write(BUS_LINE,RS_COM);//2 линии данных, 4 бита шина
	LCD_write(BUS_LINE,RS_COM);
	LCD_write(BUS_LINE,RS_COM);
	LCD_write(BUS_LINE,RS_COM);
	LCD_write(0x01,RS_COM);//очистка экрана
	LCD_write(0x06,RS_COM);//инкремент адреса экран не движется
	
	LCD_write(0x0C,RS_COM);//включить дисплей

}

void LCD_write(uint8_t data, uint8_t BIT_RS)
{
	_delay_ms(40);
//	LCD_read_BF();
	DDR_DATA |= 0xF0;//порт данных 
	PORT_DATA |= 0xF0;//на выход
	PORT_SIG &= ~(ON<<RW);
	if(BIT_RS == RS_COM)// если 0  
	{
		PORT_SIG &= ~(ON<<RS);//то Команда
	} 
	else PORT_SIG|= (ON<<RS);// иначе 1 то данные 
	uint8_t tmp = data;//сохронили данные во временном
	uint8_t temp = PORT_DATA & 0x0F;//
	temp |= (data &0xF0);// обрезали младщую тетраду данных
	PORT_DATA = temp;//выствали тетраду в порт
	PORT_SIG|= (ON<<E);//строб вверх
	_delay_ms(2);
	PORT_SIG &= ~(ON<<E);//строб вниз старшая тетрада на дисплей 
	temp = PORT_DATA & 0x0F;
	temp|= (tmp<<4);// обрезали старшую тетраду
	PORT_DATA = temp;//выставили младшую тетраду в порт
	PORT_SIG|= (ON<<E);//строб вверх
	_delay_ms(2);
	PORT_SIG &= ~(ON<<E);//строб вниз младшая тетрада ушла на дисплей
//	_delay_ms(40);	

}

void LCD_write_adress(uint8_t data, uint8_t adress)
{
	_delay_ms(40);
//	LCD_read_BF();
	LCD_write(adress, RS_COM);
	DDR_DATA |= 0xF0;
	PORT_DATA |= 0xF0;
	PORT_SIG &= ~(ON<<RW);
	PORT_SIG|= (ON<<RS);// иначе 1 то данные 
	uint8_t tmp = data;
	uint8_t temp = PORT_DATA & 0x0F;
	temp |= (data &0xF0);
	PORT_DATA = temp;
	PORT_SIG|= (ON<<E);
	_delay_ms(2);
	PORT_SIG &= ~(ON<<E);
	temp = PORT_DATA & 0x0F;
	temp|= (tmp<<4);
	PORT_DATA = temp;
	PORT_SIG|= (ON<<E);
	_delay_ms(2);
	PORT_SIG &= ~(ON<<E);
	_delay_ms(40);	

}

void LCD_write_str(int8_t *string)
{
	while(*string)
	{
		LCD_write(*string,RS_DATA);
		string++;
	}
}

void line_wrapping(int8_t number)
{
	if(number == 1)
	{
	 LCD_write(FIRS_LINE,RS_COM); 
	}
	else
	 LCD_write(SEC_LINE,RS_COM);	
}

void clean_display()
{
	LCD_write(0x01,RS_COM);
}

void kursor_adress(uint8_t adress)
{
		LCD_write(adress, RS_COM);
}

inline void LCD_alignment(int16_t str, uint8_t adress)
{	
	uint8_t buffer[] = {' ',' ',' ',' '};
	itoa(str, buffer, 10);

	if (str < 0)
		{
			if (str > (-10))
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("     ");
			}
			else if (str > (-100))
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("    ");
			}
			else if (str  >(-1000))
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("   ");
			}
			else if (str  <=(-1000))
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("   ");
			}
		}

		else
		{
			if (str < 10)
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("     ");
			}
			else if (str < 100)
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("    ");
			}
			else if (str < 1000)
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("   ");
			}
			else if (str >= 1000)
			{
				kursor_adress(adress);
				LCD_write_str(buffer);
				LCD_write_str("   ");
			}
		}
}

/*
void LCD_read_BF()
{
	uint8_t tmp = 0;
	uint8_t data = 0;

	DDR_DATA &= 0x0F;
	PORT_DATA |= 0xF0;
	PORT_SIG |= (ON<<RW);
	PORT_SIG &= ~(ON<<RS);
	
	do{
	PORT_SIG|= (ON<<E);
	_delay_ms(10);
	data = PIN_DATA & 0xF0;
	PORT_SIG &= ~(ON<<E);
	
	tmp = data;
	PORT_SIG|= (ON<<E);
	_delay_ms(10);
	data = PIN_DATA & 0xF0;
	PORT_SIG &= ~(ON<<E);
	
	data = data>>4;
	tmp|=data;
	}while((tmp &(1<<BF))!=0);		
}
*/




