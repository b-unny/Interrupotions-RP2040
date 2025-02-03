#include "lib.h"

volatile int count  = 0;

void init_led_rgb() 
{
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(LED_B);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT);
}

void blink_red_led()
{
    gpio_put(LED_R, true);
    slepp_ms(100);
    gpio_put(LED_R, false);
    slepp_ms(100);
}

void init_buttons() 
{
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_edge_rise(BUTTON_A, button_a_isr);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_edge_rise(BUTTON_B, button_b_isr);
}

// handler botao A
void button_a_isr(uint gpio, uint32_t event)
{
    count--;
    if (count <  0) count = 9;
    update_display(count);
}

//handler botao B
void button_b_isr(uint gpio, uint32_t events) 
{
    count++;
    if (count > 9) count = 0;
    update_display(count);
}

//TODO ver expliccao mais detalhada disso aqui, tendi nao
void init_led_matrix() 
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, MATRIX_PIN, 800000, false);
}

// Atualiza a matriz para exibir um número específico
void update_display(int number) 
{
    static const uint8_t numbers[10][5][5] = { // de 0 a 9
        {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}}, 
        {{0,0,1,0,0},{0,1,1,0,0},{0,0,1,0,0},{0,0,1,0,0},{0,1,1,1,0}}, 
        {{0,1,1,1,0},{0,0,0,1,0},{0,0,1,0,0},{0,1,0,0,0},{0,1,1,1,0}},
        {{0,1,1,1,0},{0,0,0,1,0},{0,0,1,1,0},{0,0,0,1,0},{0,1,1,1,0}}, 
        {{0,0,0,1,0},{0,0,1,1,0},{0,1,0,1,0},{1,1,1,1,1},{0,0,0,1,0}}, 
        {{0,1,1,1,0},{0,1,0,0,0},{0,1,1,1,0},{0,0,0,1,0},{0,1,1,1,0}}, 
        {{0,1,1,1,0},{0,1,0,0,0},{0,1,1,1,0},{0,1,0,1,0},{0,1,1,1,0}}, 
        {{0,1,1,1,0},{0,0,0,1,0},{0,0,1,0,0},{0,1,0,0,0},{0,1,0,0,0}}, 
        {{0,1,1,1,0},{0,1,0,1,0},{0,1,1,1,0},{0,1,0,1,0},{0,1,1,1,0}}, 
        {{0,1,1,1,0},{0,1,0,1,0},{0,1,1,1,0},{0,0,0,1,0},{0,1,1,1,0}}  
};
    
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            set_led_matrix(row, col, numbers[number][row][col] ? PASTEL_PINK : BLK);
        }
    }
}

// Define um LED específico na matriz
void set_led_matrix(int row, int col, uint32_t color) {
    int index = row * 5 + col;
    if (index >= 0 && index < 25) {
        put_pixel(urgb_u32((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF));
    }
}


