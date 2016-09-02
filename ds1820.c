#include "ds1820.h"

#define owi_low()		DS1820_DDR |= DS1820_BIT
#define owi_rel()		DS1820_DDR &= ~DS1820_BIT
#define owi_state()		(DS1820_PIN & DS1820_BIT)

void owi_write(uint8_t data)
{
	uint8_t n;

	cli();
	for(n = 0; n < 8; ++n)
	{
		if(data & 1)
		{
			owi_low();
			_delay_us(6);
			owi_rel();
			_delay_us(64);

		}
		else
		{
			owi_low();
			_delay_us(60);
			owi_rel();
			_delay_us(10);
		}
		
		data >>= 1;
	}
	sei();
}

uint8_t owi_read()
{
	uint8_t data = 0, n;

	cli();
	for(n = 0; n < 8; ++n)
	{
		data >>= 1;

		owi_low();
		_delay_us(6);
		owi_rel();
		_delay_us(9);
		if(owi_state())
			data |= 0x80;
		_delay_us(55);
	}
	sei();
	return data;
}

uint8_t owi_reset()
{
	uint8_t success = 0;

	cli();
	owi_low();
	_delay_us(480);
	owi_rel();
	_delay_us(70);
	if(!owi_state())
		success = 1;
	_delay_us(410);
	sei();
	return success;
}

uint8_t owi_crc(uint8_t *data, uint8_t len)
{
	uint8_t crc = 0, n, c;

	while(len--)
	{
		c = *(data++);
		for(n = 8; n; --n)
		{
			if( (crc ^ c) & 1 )
			{
				crc ^= 0x18;
				crc >>= 1;
				crc |= 0x80;
			}
			else
			{
				crc >>= 1;
			}
			c >>= 1;
		}
	}
	return crc;
}

uint8_t ds1820_start()
{
	uint8_t success = 0;

	if(owi_reset())
	{
		owi_write(0xcc);
		owi_write(0x44);
		success = 1;
	}
	return success;
}

int ds1820_read()
{
	int result = 0x7fff;
	uint8_t pad[8], crc, n;

	if(owi_reset())
	{
		owi_write(0xcc);
		owi_write(0xbe);
		for(n = 0; n < 8; ++n)
			pad[n] = owi_read();
		crc = owi_read();

		if(crc == owi_crc(pad, sizeof(pad)))
		{
			// >> 4 for 18B20; >> 1 for 1820
			result = ((int)pad[0] | ((int)pad[1] << 8)) >> 4;
		}
	}

	return result;
}
