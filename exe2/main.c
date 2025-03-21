#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

volatile int g1_timer = 0;
volatile int g2_timer = 0;



void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == BTN_PIN_R)
            flag_r = 1;
        else if (gpio == BTN_PIN_G)
            flag_g = 1;
    }
}

bool timer_0_callback(repeating_timer_t *rt) {
    g1_timer = 1;
    return true;
}

bool timer_1_callback(repeating_timer_t *rt) {
    g2_timer = 1;
    return true;
}

int main() {
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    repeating_timer_t timer_0;
    repeating_timer_t timer_1;

    int c1 = 0;
    int c2 = 0;
    
    while (true) {

        if (flag_r) {
            flag_r = 0;
            if(c1 == 0){
                
                if(add_repeating_timer_us(500000, timer_0_callback,NULL, &timer_0)){
                    c1 = 1;
                }
            }else{
                cancel_repeating_timer(&timer_0);
                c1 = 0;
                gpio_put(LED_PIN_R, 0);
            }
        }

        if (flag_g) {
            flag_g = 0;
            if(c2 == 0){
                if(add_repeating_timer_us(250000, timer_1_callback,NULL, &timer_1)){
                    c2 = 1;
                }
            }else{
                cancel_repeating_timer(&timer_1);
                c2 = 0;
                gpio_put(LED_PIN_G, 0);
            }
        }

        if(g1_timer){
            g1_timer = 0;
            gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
        }

        if(g2_timer){
            g2_timer = 0;
            gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G));
        }
    }
}
