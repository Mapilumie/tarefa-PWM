#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define LED_PIN 12

void set_pwm_duty(uint slice_num, uint chan, uint16_t level) {
    pwm_set_chan_level(slice_num, chan, level);
}

int main() {
    // Inicialização da GPIO
    
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);

    // Configuração do PWM
    uint slice_num_led = pwm_gpio_to_slice_num(LED_PIN);
    uint chan_led = pwm_gpio_to_channel(LED_PIN);

    // Frequência de 50Hz (20ms)
    pwm_set_wrap(slice_num_led, 19999);
    pwm_set_enabled(slice_num_led, true);

    while (true) {
        for (uint16_t level = 0; level <= 20000; level += 100) {
            set_pwm_duty(slice_num_led, chan_led, level);
            sleep_ms(10);
        }
        for (uint16_t level = 20000; level > 0; level -= 100) {
            set_pwm_duty(slice_num_led, chan_led, level);
            sleep_ms(10);
        }
    }

    return 0;
}