#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "lan.h"
#include "counter.h"
#include "web_if.h"
#include "LCD_HD44780.h"
#include "TWI.h"

void udp_packet(eth_frame_t *frame, uint16_t len) {}

uint8_t tcp_listen(uint8_t id, eth_frame_t *frame)
{
	ip_packet_t *ip = (void*)(frame->data);
	tcp_packet_t *tcp = (void*)(ip->data);

	if( (tcp->to_port == htons(80)) ||
		(tcp->to_port == htons(44444)) )
	{
		return 1;
	}
	return 0;
}

void tcp_opened(uint8_t id, eth_frame_t *frame)
{
}

void tcp_closed(uint8_t id, uint8_t reset)
{
}

void tcp_data(uint8_t id, eth_frame_t *frame, uint16_t len, uint8_t closing,int16_t A_X, 
int16_t A_Y,int16_t A_Z, int16_t G_X, int16_t G_Y, int16_t G_Z)
{
	webif_data(id, frame, len, A_X, A_Y, A_Z, G_X, G_Y,G_Z);
}


	enum measure_mode//������������� ���������� ��� ���������� ������������ ������� ��������� � ����������
	{
		accel_xh,
		accel_xl,
		accel_yh,
		accel_yl,
		accel_zh,
		accel_zl,
		hyro_xh,
		hyro_xl,
		hyro_yh,
		hyro_yl,
		hyro_zh,
		hyro_zl,
		temperature_h,
		temperatyre_l
	};
	int array_adress[] =//������ ��������� ������ MPU6050
	{ ACCEL_XOUT_H, ACCEL_XOUT_L, ACCEL_YOUT_H, ACCEL_YOUT_L, ACCEL_ZOUT_H, ACCEL_ZOUT_L, 
	GYRO_XOUT_H, GYRO_XOUT_L, GYRO_YOUT_H, GYRO_YOUT_L, GYRO_ZOUT_H, GYRO_ZOUT_L,
			TEMP_OUT_H, TEMP_OUT_L };
	enum measure_mode current = accel_xh;//������ ���� �� ������� ��������
	char data[15];
	uint8_t Flag = 1;
	uint8_t reg_adress;

	int16_t ACCEL_X;
	int16_t ACCEL_Y;
	int16_t ACCEL_Z;
	int16_t GYRO_X;
	int16_t GYRO_Y;
	int16_t GYRO_Z;

ISR(TWI_vect)//������ ���������� ���� TWI
{
	switch(get_status())
	{
		case TW_BUS_FAIL://���������� ������ ����
		/*/	ERROR();
			char array[4] = "Error";
			for(int i = 0; i< countdown; i++)
			{
				transiver_UART(array[i]);
			}*/
		break;
		
		case TW_START://���������� ������� �����(1)

			

			I2C_tranciv_byte(MPU_6050_ADDRESS_W);//	��������� ����� ��������� � ������ ������
			

			break;

		case ReStart://��������� ��������� �����(4)

			I2C_tranciv_byte(MPU_6050_ADDRESS_R);//��������� ����� ��������� � ������ ������
			break;

		case TW_MT_AD_ACK://������� ������ ����� �������� � ����� ��� ������ ������� ���������(2)
			
//			PORTD ^= 1<<PD7;
//			_delay_ms(500);
			reg_adress = array_adress[(int)current];
			
			I2C_tranciv_byte(reg_adress);//��������� ����� �������� �� �������� ����� ������ ������ 

			break;

		case TW_MT_AD_NACK://������� ������ ����� �������� � ����� ��� ������ ������� �� ���������
			
			kursor_adress(FIRST);
			LCD_write_str("ERROR SLAVE_NACK");
			break;
		
		case TW_MT_DATA_ACK://������� ������ ������ � ������ ������������� (3)

			I2C_start();

			break;

		case TW_MT_DATA_NACK://������� ������ ������ � �� ������ ������������� 
			
			break;

		case TW_MR_AD_ACK://������� ������ ����� �������� � ����� ��� ������ ������� ���������(5)

			data[(int)current] = I2C_receiver_last_byte();//��������� ������ �������� � ������	
		
			break;

		case TW_MR_AD_NACK://������� ������ ����� �������� � ����� ��� ������ ������� �� ���������
			
			break;

		case TW_MR_DATA_ACK://������� ������ ������ � ������� �������������
			
			break;

		case TW_MR_DATA_NACK://������� ������ ��������� ���� � ������� NACK(6)
	
			I2C_stop();
			
			if(current == temperatyre_l)//����� ��������� ������ �� ���������� �������� 
			{	
				
//				PORTB ^= 1<<PB3;
				current = accel_xh;//������ ��������� � ������
				Flag = 1;// � ������� �� �������� ������ �� �������
			}
			else//
			{ 
				current++;//�� ��� ������ ���������, ������������� ������� �� ��������� ���
				I2C_start();	
			
			}
			break;

	}
}


int main()
{
	_delay_ms(20);

	DDRD |= (1<<PD7);
	PORTD |= (1<<PD7);
	lan_init();

	counter_init();


	I2C_init();// ������������� ���� TWI	
//	DDRD = 1<<PD5|1<<PD4|1<<PD7;
//	DDRB = 1<<PB3;
//	PORTD = 0<<PD5|0<<PD4|0<<PD7;

	sei();

//	webif_init();
	
	while(1)
	{
		while(Flag)//���� Flag ������������ ���������� ������ � ������� �� �������
		{	
//			cli();
			char buffer[4];
			ACCEL_X = data[(int)accel_xh]<<8|(data[(int)accel_xl]&0xF0);
			ACCEL_Y = data[(int)accel_yh]<<8|(data[(int)accel_yl]&0xF0);
			ACCEL_Z = data[(int)accel_zh]<<8|(data[(int)accel_zl]&0xF0);
			GYRO_X = data[(int)hyro_xh]<<8|(data[(int)hyro_xl]&0xF0);
			GYRO_Y = data[(int)hyro_yh]<<8|(data[(int)hyro_yl]&0xF0);
			GYRO_Z = data[(int)hyro_zh]<<8|(data[(int)hyro_zl]&0xF0);
			int16_t TEMPERATURE = data[(int)temperature_h]<<8|data[(int)temperatyre_l];
			TEMPERATURE = TEMPERATURE/340+36;
				
			static _Bool is_init;

			if(!is_init)//���� ������ ���� � ������� �� �������������� �������
			{
				LCD_init();//� ������ ��� �� �����������
				kursor_adress(FIRST);
				LCD_write_str("A 0000          ");					
				kursor_adress(SEC_LINE);
				LCD_write_str("H 0000          ");	
				is_init = true;		
			}
			LCD_alignment(ACCEL_X, THIRD);//����� ������ �� �������
//			LCD_alignment(ACCEL_Y, EIGHTH);//
//			LCD_alignment(ACCEL_Z, THIRTEENTH);//
			LCD_alignment(GYRO_X, THIRD_S);//
//			LCD_alignment(GYRO_Y, EIGHTH_S);
//			LCD_alignment(GYRO_Z, THIRTEENTH_S);

/*			itoa(ACCEL_X, buffer, 10);
			kursor_adress(THIRD);
			LCD_write_str(buffer);
//			itoa(ACCEL_Y, buffer, 10);
//			kursor_adress(SEVENTH);
//			LCD_write_str(buffer);
//			itoa(ACCEL_Z, buffer, 10);
//			kursor_adress(ELEVENTH);
//			LCD_write_str(buffer);

			itoa(GYRO_X, buffer, 10);
			kursor_adress(THIRD_S);
			LCD_write_str(buffer);
			itoa(GYRO_Y, buffer, 10);
//			kursor_adress(SEVENTH_S);
//			LCD_write_str(buffer);
//			itoa(GYRO_Z, buffer, 10);
//			kursor_adress(ELEVENTH_S);
//			LCD_write_str(buffer);
//			itoa(TEMPERATURE, buffer, 10);
//			kursor_adress(FIFTEENTH_S);
//			LCD_write_str(buffer);
*/
//			_delay_ms(40);
			Flag = 0;
			I2C_start();//��������� ���������� �� ���� TWI � ��������� ������
			TWCR &= ~(1<<TWINT);//����� ���� TWINT ����� sei() ����������!!!!!
//			sei();
		}


		lan_poll(ACCEL_X, ACCEL_Y, ACCEL_Z, GYRO_X, GYRO_Y, GYRO_Z);

		if(lan_up())
			PORTD &= ~(1<<PD7);
		else
			PORTD |= (1<<PD7);
	}

	return 0;
}
