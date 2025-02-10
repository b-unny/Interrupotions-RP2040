#include "lib.h"

volatile int count  = 0;
volatile uint32_t last_interrupt_time_a = 0;
volatile uint32_t last_interrupt_time_b = 0;

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
    sleep_ms(100);
    gpio_put(LED_R, false);
    sleep_ms(100);
}

void init_buttons() 
{
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_RISE, true, button_a_isr);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_RISE, true, button_b_isr);
}    

void button_a_isr(uint gpio, uint32_t event)
{ 
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_interrupt_time_a < 200) return; 
    last_interrupt_time_a = current_time;
    
    count--;
    if (count <  0) count = 9;
    update_display(count);
}

void button_b_isr(uint gpio, uint32_t events) 
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_interrupt_time_b < 200) return;
    last_interrupt_time_b = current_time;

    count++;
    if (count > 9) count = 0;
    update_display(count);
}


//TODO ver expliccao mais detalhada das funçoes abaixo, tendi nao T-T

static uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) 
{
    return (r << 8) | (g << 16) | b;
}
static void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);  
    // Envia para o pio0, estado de máquina 0
}
void init_led_matrix() 
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, MATRIX_PIN, 800000, false);
}

void set_led_matrix(int index, uint32_t color) 
{
    if (index >= 0 && index < NUM_LEDS) 
    {
        put_pixel(color);
    }
}
 
void update_display(int number)
{
   uint32_t pastel_pink = urgb_u32(
    (uint8_t)(193 * BRIGHTNESS), 
    (uint8_t)(255 * BRIGHTNESS), 
    (uint8_t)(182 * BRIGHTNESS)
);

    static const uint8_t numbers[10][NUM_LEDS] ={
        {   0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0}, // 0

        {   0, 0, 1, 0, 0, 
            0, 0, 1, 0, 0, 
            0, 0, 1, 0, 0, 
            0, 1, 1, 0, 0, 
            0, 0, 1, 0, 0}, // 1

        {   0, 1, 1, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0},  // 2

        {   0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0}, //3

        {   0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 0, 1, 0}, //4

        {   0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 1, 1, 1, 0}, //5
        
        {   0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 1, 1, 1, 0}, //6

        {   0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0}, //7

        {   0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0}, //8

        {   0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0} //9   
    };

    
    for (int index = 0; index < NUM_LEDS; index++) 
        set_led_matrix(index, numbers[number][index] ?  pastel_pink : 0);
}



