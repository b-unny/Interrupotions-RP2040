#ifndef LIB_H
#define LIB_H

#define LED_R 13
#define LED_G 12
#define LED_B 11
#define BUTTON_A 5
#define BUTTON_B 6
#define MATRIX_PIN 7
#define NUM_LEDS 25
#define BRIGHTNESS 0.5 

#include"pico/stdlib.h"
#include"hardware/gpio.h"
#include"hardware/pio.h"
#include"hardware/clocks.h"
#include"ws2812.pio.h"

void init_led_rgb();
void blink_red_led();
void init_buttons();
void button_a_isr(uint gpio, uint32_t event);
void button_b_isr(uint gpio, uint32_t events); 
static uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
static void put_pixel(uint32_t pixel_grb);
void init_led_matrix();
void set_led_matrix(int row, int col, uint32_t color);
void update_display(int number);

#endif
