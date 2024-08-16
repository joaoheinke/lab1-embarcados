#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int LED_PIN_R = 18;
const int BTN_PIN = 13;

const int MOTOR_1 = 6;
const int MOTOR_2 = 7;
const int MOTOR_3 = 8;
const int MOTOR_4 = 9;

// Sequência para o motor de passo
const uint8_t step_sequence[4] = {
    0b0001,
    0b0010,
    0b0100,
    0b1000
};

void set_motor_pins(uint8_t step) {
    gpio_put(MOTOR_1, step & 0b0001);
    gpio_put(MOTOR_2, step & 0b0010);
    gpio_put(MOTOR_3, step & 0b0100);
    gpio_put(MOTOR_4, step & 0b1000);
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    // Inicializa os pinos do motor como saída
    gpio_init(MOTOR_1);
    gpio_init(MOTOR_2);
    gpio_init(MOTOR_3);
    gpio_init(MOTOR_4);
    gpio_set_dir(MOTOR_1, GPIO_OUT);
    gpio_set_dir(MOTOR_2, GPIO_OUT);
    gpio_set_dir(MOTOR_3, GPIO_OUT);
    gpio_set_dir(MOTOR_4, GPIO_OUT);

    bool led_r_state = false;
    int step_index = 0;

    while (true) {
        if (!gpio_get(BTN_PIN)) {
            led_r_state = !led_r_state;  
            gpio_put(LED_PIN_R, led_r_state);  

            for(int i = 0; i < 2048; i++){
                set_motor_pins(step_sequence[step_index]);
                step_index = (step_index + 1) % 4;  // Avança para o próximo passo
                sleep_ms(10);  // Controle da velocidade do motor
            }

            // Desliga todos os pinos do motor quando o botão é liberado
            set_motor_pins(0);
            sleep_ms(200);  
        }
    }

    return 0;
}
