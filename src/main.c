#include "us2066.h"
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

int main()
{
    int i = 0;
    us2066_init();
    for (i = 0; i < 80; i++)
    {
        _disp_data(0, 31);
        _disp_data(1, 31);
        _disp_data(2, 31);
    }
    while (1)
    {
        _delay_us(10);
    }
}
