#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "us2066.h"

#define SPI_CS PORTB
#define SPI_DDR DDRB
#define NUM_DISP 3
#define RST_PORT PORTD
#define RST_PIN 0
#define RST_DDR DDRD

void us2066_init()
{
    uint8_t i = 0;

    DDRB |= (1<<DDB5)|(1<<DDB3);
    RST_DDR |= (1<<RST_PIN);
    RST_PORT &= ~(1<<RST_PIN);
    _delay_ms(2);
    RST_PORT |= (1<<RST_PIN);

    for (i = 0; i < NUM_DISP; i++)
    {
        SPI_DDR |= (1<<i);
        SPI_CS |= (1<<i);
    }

    SPCR |= (1<<SPE)|(1<<DORD)|(1<<MSTR);
    SPCR |= (1<<CPOL)|(1<<CPHA)|(1<<SPR1);

    _delay_us(100);

    for (i = 0; i < NUM_DISP; i++)
    {
        _disp_command(i, 0x2A);  //function set (extended command set)
        _disp_command(i, 0x71);  //function selection A, disable internal Vdd regualtor
        _disp_data(i, 0x00);
        _disp_command(i, 0x28);  //function set (fundamental command set)
        _disp_command(i, 0x08);  //display off, cursor off, blink off
        _disp_command(i, 0x2A);  //function set (extended command set)
        _disp_command(i, 0x79);  //OLED command set enabled
        _disp_command(i, 0xD5);  //set display clock divide ratio/oscillator frequency
        _disp_command(i, 0x70);  //set display clock divide ratio/oscillator frequency
        _disp_command(i, 0x78);  //OLED command set disabled
        _disp_command(i, 0x08);  //extended function set (2-lines)
        _disp_command(i, 0x06);  //COM SEG direction
        _disp_command(i, 0x72);  //function selection B, disable internal Vdd regualtor
        _disp_data(i, 0x00);     //ROM CGRAM selection
        _disp_command(i, 0x2A);  //function set (extended command set)
        _disp_command(i, 0x79);  //OLED command set enabled
        _disp_command(i, 0xDA);  //set SEG pins hardware configuration
        _disp_command(i, 0x10);  //set SEG pins ... NOTE: When using NHD-0216AW-XB3 or NHD_0216MW_XB3 change to (0x00)
        _disp_command(i, 0xDC);  //function selection C
        _disp_command(i, 0x00);  //function selection C
        _disp_command(i, 0x81);  //set contrast control
        _disp_command(i, 0x7F);  //set contrast control
        _disp_command(i, 0xD9);  //set phase length
        _disp_command(i, 0xF1);  //set phase length
        _disp_command(i, 0xDB);  //set VCOMH deselect level
        _disp_command(i, 0x40);  //set VCOMH deselect level
        _disp_command(i, 0x78);  //OLED command set disabled
        _disp_command(i, 0x28);  //function set (fundamental command set)
        _disp_command(i, 0x01);  //clear display
        _disp_command(i, 0x80);  //set DDRAM address to 0x00
        _disp_command(i, 0x0C);  //display ON
    }

}

void _disp_command(uint8_t disp_num, uint8_t command)
{
    SPI_CS &= ~(1<<disp_num);
    SPDR = 0x1f;
    while(!(SPSR & (1<<SPIF)));
    SPDR = command & 0x0f;
    while(!(SPSR & (1<<SPIF)));
    SPDR = (command & 0xf0) >> 4;
    while(!(SPSR & (1<<SPIF)));
    _delay_us(1);
    SPI_CS |= (1<<disp_num);
    //_disp_wait_busy(disp_num);

}

void _disp_data(uint8_t disp_num, uint8_t data)
{

    SPI_CS &= ~(1<<disp_num);
    SPDR = 0x5f;
    while(!(SPSR & (1<<SPIF)));
    SPDR = data & 0x0f;
    while(!(SPSR & (1<<SPIF)));
    SPDR = (data & 0xf0) >> 4;
    while(!(SPSR & (1<<SPIF)));
    _delay_us(1);
    SPI_CS |= (1<<disp_num);
    //_disp_wait_busy(disp_num);
}

void _disp_wait_busy(uint8_t disp_num)
{
    uint8_t result = 0xff;
    SPI_CS &= ~(1<<disp_num);
    SPDR = 0x3f;
    while(!(SPSR & (1<<SPIF)));
    while ((result & 0x80) != 0)
    {
        SPDR = 0;
        while(!(SPSR & (1<<SPIF)));
        result = SPDR;
    }
    _delay_us(1);
    SPI_CS |= (1<<disp_num);
}

uint8_t _disp_read_status(uint8_t disp_num)
{
    uint8_t result;
    SPI_CS &= ~(1<<disp_num);
    SPDR = 0x3f;
    while(!(SPSR & (1<<SPIF)));
    SPDR = 0;
    while(!(SPSR & (1<<SPIF)));
    result = SPDR;
    _delay_us(1);
    SPI_CS |= (1<<disp_num);

    return result;
}

uint8_t _disp_read_data(uint8_t disp_num)
{
    uint8_t result;
    SPI_CS &= ~(1<<disp_num);
    SPDR = 0x7f;
    while(!(SPSR & (1<<SPIF)));
    SPDR = 0;
    while(!(SPSR & (1<<SPIF)));
    result = SPDR;
    _delay_us(1);
    SPI_CS |= (1<<disp_num);

    return result;
}
