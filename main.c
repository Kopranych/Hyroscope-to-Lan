#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "lan.h"
#include "counter.h"
#include "web_if.h"

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

void tcp_data(uint8_t id, eth_frame_t *frame, uint16_t len, uint8_t closing)
{
	webif_data(id, frame, len);
}

int main()
{
	_delay_ms(20);

	DDRD |= (1<<PD7);
	PORTD |= (1<<PD7);
	lan_init();

	counter_init();
	sei();

//	webif_init();
	
	while(1)
	{
		lan_poll();

		if(lan_up())
			PORTD &= ~(1<<PD7);
		else
			PORTD |= (1<<PD7);
	}

	return 0;
}
