#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22  // GPIO 22 para controle do servomotor
#define DELAY_MS 10   // Atraso de 10ms entre cada ajuste
#define MIN_PULSE_WIDTH 500  // Largura de pulso para 0 graus (500µs)
#define MAX_PULSE_WIDTH 2400 // Largura de pulso para 180 graus (2400µs)
#define STEP 5        // Incremento de 5µs por passo

void set_servo_pulse_width(uint slice_num, uint channel, uint pulse_width) {
    // Define a largura do pulso do PWM
    pwm_set_chan_level(slice_num, channel, pulse_width);
}

int main() {
    // Inicializa o GPIO para o servomotor
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);

    // Configura o PWM para 50Hz (período de 20ms)
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 256.0f); // Ajusta o divisor de clock para 50Hz
    pwm_config_set_wrap(&config, 19531);    // Período de 20ms
    pwm_init(slice_num, &config, true);

    // Define as posições do servomotor e aguarda 5 segundos em cada posição
    set_servo_pulse_width(slice_num, channel, MAX_PULSE_WIDTH); // 180 graus
    sleep_ms(5000);
    set_servo_pulse_width(slice_num, channel, 1470); // 90 graus
    sleep_ms(5000);
    set_servo_pulse_width(slice_num, channel, MIN_PULSE_WIDTH); // 0 graus
    sleep_ms(5000);

    // Movimento contínuo e suave do servomotor entre 0 e 180 graus
    while (true) {
        // Movimento de 0 a 180 graus (aumenta a largura do pulso)
        for (uint pulse_width = MIN_PULSE_WIDTH; pulse_width <= MAX_PULSE_WIDTH; pulse_width += STEP) {
            set_servo_pulse_width(slice_num, channel, pulse_width);
            sleep_ms(DELAY_MS);
        }

        // Movimento de 180 a 0 graus (diminui a largura do pulso)
        for (uint pulse_width = MAX_PULSE_WIDTH; pulse_width >= MIN_PULSE_WIDTH; pulse_width -= STEP) {
            set_servo_pulse_width(slice_num, channel, pulse_width);
            sleep_ms(DELAY_MS);
        }

    }

    return 0;
}