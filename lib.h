#ifndef LIB_H
#define LIB_H

#define PASTEL_PINK 0xFFD1DC  // RGB(255, 209, 220)
#define BLK 0x000000

#include"pico/stdlib.h"
#include"hardware/gpio.h"
#include"hardware/pio.h"
#include"ws2812.pio.h"

void init_led_rgb();
void blink_red_led();
void init_buttons();
void button_a_isr(uint gpio, uint32_t event);
void button_b_isr(uint gpio, uint32_t events); 
void init_led_matrix();
void update_display(int number); 
void set_led_matrix(int row, int col, uint32_t color);

#endif
