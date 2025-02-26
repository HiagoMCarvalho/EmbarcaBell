#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "math.h"

const uint led_pin_red = 13;   // LED vermelho na GPIO13 (PWM)
const uint led_pin_blue = 12;  // LED azul na GPIO12 (PWM)
const uint microfone = 28;     // GPIO28 para ADC2 (Microfone)
const float Vref = 3.3;        // Tensão de referência do ADC
const uint amostras_por_ciclo = 200;  // Número de amostras por ciclo de medição
const float sensibilidade = 20.0; // Ajuste para calibrar resposta
const float fator_suavizacao = 0.1;  // Para suavizar as mudanças de brilho
const float histerese = 0.02;  // Mínima variação antes de mudar brilho

float brilho_azul_atual = 0.0;
float brilho_vermelho_atual = 0.0;

void set_led_brightness(uint led_pin, float brightness) 
{
    uint slice = pwm_gpio_to_slice_num(led_pin);
    pwm_set_gpio_level(led_pin, (uint16_t)(brightness * 65535)); // PWM de 16 bits
    pwm_set_enabled(slice, true);
}

float suavizar(float novo_valor, float valor_anterior) 
{
    return valor_anterior + fator_suavizacao * (novo_valor - valor_anterior);
}

int main() 
{
    //inicialização do STDIO
    stdio_init_all();  

    //inicialização do ADC
    adc_init();
    adc_gpio_init(microfone);

    //inicialização do PWM
    gpio_set_function(led_pin_red, GPIO_FUNC_PWM);
    gpio_set_function(led_pin_blue, GPIO_FUNC_PWM);
    uint slice_red = pwm_gpio_to_slice_num(led_pin_red);
    uint slice_blue = pwm_gpio_to_slice_num(led_pin_blue);
    pwm_set_clkdiv(slice_red, 64.0);
    pwm_set_clkdiv(slice_blue, 64.0);
    pwm_set_wrap(slice_red, 65535);
    pwm_set_wrap(slice_blue, 65535);

    while (true) 
    {
        float Vmax = 0.0;
        float Vmin = Vref;

        //cálculo dos picos maximos e minimos das amostras
        for (uint i = 0; i < amostras_por_ciclo; i++) 
        {
            adc_select_input(2);
            uint16_t mic_value = adc_read();
            float Ventrada = (mic_value * Vref) / 4095.0;

            if (Ventrada > Vmax) Vmax = Ventrada;
            if (Ventrada < Vmin) Vmin = Ventrada;

            sleep_us(100);
        }

        //cálculo da amplitude
        float amplitude = Vmax - Vmin;
        //cálculo do nível de ruído em decibéis
        float dB = 20 * log10((amplitude * sensibilidade) / Vref) + 94;

        //interpolação linear de 86 a 109 dB (Valores definidos para fins didáticos)
        float novo_brilho_azul = 1.0 - ((dB - 86) / 23);
        float novo_brilho_vermelho = (dB - 86) / 23;

        //padronização dos valores para ficarem entre 0 e 1
        novo_brilho_azul = novo_brilho_azul < 0 ? 0 : (novo_brilho_azul > 1 ? 1 : novo_brilho_azul);
        novo_brilho_vermelho = novo_brilho_vermelho < 0 ? 0 : (novo_brilho_vermelho > 1 ? 1 : novo_brilho_vermelho);

        // Suavização para evitar piscadas bruscas
        if (fabs(novo_brilho_azul - brilho_azul_atual) > histerese) 
        {
            brilho_azul_atual = suavizar(novo_brilho_azul, brilho_azul_atual);
        }
        if (fabs(novo_brilho_vermelho - brilho_vermelho_atual) > histerese) 
        {
            brilho_vermelho_atual = suavizar(novo_brilho_vermelho, brilho_vermelho_atual);
        }

        //definição da intensidade dos leds via pwm
        set_led_brightness(led_pin_blue, brilho_azul_atual);
        set_led_brightness(led_pin_red, brilho_vermelho_atual);

        //simulação de envio de notificação de acordo com qual ala hospitalar está com nível de ruído acima do permitido
        if(dB > 109)
        {
            printf("ruido acima do permitido detectado no centro cirurgico!\n");
        }

        sleep_us(100);
    }
}
