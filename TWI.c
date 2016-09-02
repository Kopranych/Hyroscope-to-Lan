#include "TWI.h"


void I2C_init()
{
	TWBR = 0x20;// �������� �������� ��� 8��� ���������� 100���
	MPU_I2C_W(MPU_6050_ADDRESS_W,PWR_MGMT_1, 0x00);//�������� ������� ���������� ��������, ��� ����� �� �������� ������
}


void I2C_start()
{
	TWCR = (1<<TWSTA)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE);
	while(!(TWCR&(1<<TWINT))); //���� ��������� ���� TWIN
}


void I2C_stop()
{
	TWCR = (1<<TWSTO)|(1<<TWINT)|(1<<TWEN)|(1<<TWIE);
}


int I2C_tranciv_byte(unsigned char byte)
{
	TWDR = byte; //�������� ������ � ������� �������� ������
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);//��������� �������� ������
	while(!(TWCR&(1<<TWINT))); //���� ��������� ���� TWIN
	if((TWSR & 0xF8)!= TW_MT_DATA_ACK)//��������� ������ �������� �������
	{
		return 0;//���� ������ ������� ���������� 0
	}
	return 1;//���� ����� 1
}


char I2C_receiver_byte()
{
	int err = 0;
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA)|(1<<TWIE);// �������� ����� ������
	while(!(TWCR&(1<<TWINT))); //���� ��������� ���� TWIN
	if((TWSR & 0xF8)!= TW_MR_DATA_ACK) err = 1;
	else err = 0;
	return TWDR;
}


char I2C_receiver_last_byte()
{

	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);// �������� ����� ������
	while(!(TWCR&(1<<TWINT))); //���� ��������� ���� TWIN

	return TWDR;
}

unsigned char get_status()
{
	unsigned char status;
	status = TWSR & 0xF8;// ����������� ����� ��� ������ �������� � ��������� ��������
	return status;
}

char MPU_I2C_R(unsigned char addrw,unsigned char ra, unsigned char addrr)
{
	char data;
	I2C_start();//���������� ������� �����
	I2C_tranciv_byte(addrw);//���������� ����� ������ �� ������
	I2C_tranciv_byte(ra);//���������� ����� �������� ������ ����� ������ ������
	I2C_start();//���������� ��������� �����
	I2C_tranciv_byte(addrr);//���������� ����� ������ �� ������
	data = I2C_receiver_last_byte();//������ ������ �� ��������
	I2C_stop();//���������� ������� ����
	return data;
}

char MPU_I2C_W(unsigned char addrw,unsigned char ra, unsigned char data)
{
	I2C_start();//���������� ������� �����
	I2C_tranciv_byte(addrw);//���������� ����� ������ �� ������
	I2C_tranciv_byte(ra);//���������� ����� �������� ���� ����� ���������� ������
	I2C_tranciv_byte(data);//���������� ������
	I2C_stop();//���������� ������� ����
	if((TWSR&0xF8)!=TW_MT_DATA_ACK)
	{
		 return 0;
	}
	else
	return 1;
}
