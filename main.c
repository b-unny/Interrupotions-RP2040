#include "lib.h"
#include "pico/stdlib.h"

int main() 
{
    stdio_init_all();
    init_led_rgb();
    init_led_matrix();
    init_buttons();
    
    while (true)  blink_red_led(); 

    return 0;
}
