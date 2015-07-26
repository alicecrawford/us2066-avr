/*
US2066 AVR driver
*/

#ifndef _us2066_h
#define _us2066_h

#include <stdint.h>

// Set up the micro for SPI and initalize the display
void us2066_init();

// Basic low-level functions
void _disp_command(uint8_t disp_num, uint8_t command);
void _disp_data(uint8_t disp_num, uint8_t data);
uint8_t _disp_read_status(uint8_t disp_num);
uint8_t _disp_read_data(uint8_t disp_num);
void _disp_wait_busy(uint8_t disp_num);

#endif
