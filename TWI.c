#include "TWI.h"


void I2C_init()
{
	TWBR = 0x20;// скорость передачи при 8мГц получается 100кГц
	MPU_I2C_W(MPU_6050_ADDRESS_W,PWR_MGMT_1, 0x00);//зануляем регистр управления питанием, без этого не читаются данные
}


void I2C_start()
{
	TWCR = (1<<TWSTA)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE);
	while(!(TWCR&(1<<TWINT))); //ждем установку бита TWIN
}


void I2C_stop()
{
	TWCR = (1<<TWSTO)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE);
}


int I2C_tranciv_byte(unsigned char byte)
{
	TWDR = byte; //записали данные в регистр передачи данных
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);//запустили передачу данных
	while(!(TWCR&(1<<TWINT))); //ждем установку бита TWIN
	if((TWSR & 0xF8)!= TW_MT_DATA_ACK)//проверяем данные регистра статуса
	{
		return 0;//если данные неверны возвращаем 0
	}
	return 1;//если верны 1
}


char I2C_receiver_byte()
{
	int err = 0;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA)|(1<<TWIE);// включили прием данных
	while(!(TWCR&(1<<TWINT))); //ждем установку бита TWIN
	if((TWSR & 0xF8)!= TW_MR_DATA_ACK) err = 1;
	else err = 0;
	return TWDR;
}


char I2C_receiver_last_byte()
{

	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);// включили прием данных
	while(!(TWCR&(1<<TWINT))); //ждем установку бита TWIN

	return TWDR;
}

unsigned char get_status()
{
	unsigned char status;
	status = TWSR & 0xF8;// накладываем маску для чтения значения в статусном регистре
	return status;
}

char MPU_I2C_R(unsigned char addrw,unsigned char ra, unsigned char addrr)
{
	char data;
	I2C_start();//отправляем условие СТАРТ
	I2C_tranciv_byte(addrw);//отправляем адрес модуля на запись
	I2C_tranciv_byte(ra);//отправляем адрес регистра откуда будем читать данные
	I2C_start();//отправляем повторный СТАРТ
	I2C_tranciv_byte(addrr);//отправляем адрес модуля на чтение
	data = I2C_receiver_last_byte();//читаем данные из регистра
	I2C_stop();//отправляем условие СТОП
	return data;
}

char MPU_I2C_W(unsigned char addrw,unsigned char ra, unsigned char data)
{
	I2C_start();//отправляем условие СТАРТ
	I2C_tranciv_byte(addrw);//отправляем адрес модуля на запись
	I2C_tranciv_byte(ra);//отправляем адрес регистра куда будем записывать данные
	I2C_tranciv_byte(data);//записываем данные
	I2C_stop();//отправляем условие СТОП
	if((TWSR&0xF8)!=TW_MT_DATA_ACK)
	{
		 return 0;
	}
	else
	return 1;
}
