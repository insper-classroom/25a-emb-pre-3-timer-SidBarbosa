#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;  


void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { 
        if (gpio == BTN_PIN_R)
            flag_f_r = 1;

    } else if (events == 0x8) { 
        if (gpio == BTN_PIN_R)
            flag_f_r = 0;
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);
    
    uint64_t t0 = 0;

    while (true) {
        if (flag_f_r) {
            if (t0 == 0) {
                t0 = time_us_64(); 
            }
        } else {
            if (t0 != 0) {
                uint64_t tf = time_us_64() - t0;
                t0 = 0;
                if (tf >= 500000) {  
                    gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
                }
            }
        }
    }
}
