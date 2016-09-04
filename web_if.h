#pragma once

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "lan.h"


void webif_init();
void webif_data(uint8_t id, eth_frame_t *frame, uint16_t len, int16_t A_X, int16_t A_Y,int16_t A_Z,
int16_t G_X, int16_t G_Y, int16_t G_Z);
