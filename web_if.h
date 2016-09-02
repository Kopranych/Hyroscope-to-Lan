#pragma once

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "lan.h"

void webif_init();
void webif_data(uint8_t id, eth_frame_t *frame, uint16_t len);
