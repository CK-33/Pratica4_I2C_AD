
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
// Inclua o cabeçalho da biblioteca do OLED fornecida em sua disciplina. 
// O nome pode variar, como ssd1306.h, inc_ssd1306.h, etc.
#include "ssd1306.h"

// Definições de pinos baseadas no padrão comum do BitDogLab. 
// Verifique o esquemático da sua placa para confirmar os pinos I2C exatos.
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// O eixo X do Joystick normalmente é ligado ao ADC0 (GPIO 26) ou ADC1 (GPIO 27)
#define JOYSTICK_X_PIN 26 

int main() {
    stdio_init_all();

    // 1. Inicialização do barramento I2C para o Display OLED a 400kHz
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização da estrutura do Display OLED (adapte para as funções da sua biblioteca)
    ssd1306_t display;
    ssd1306_init(&display, 128, 64, 0x3C, I2C_PORT);

    // 2. Inicialização do ADC para ler o Joystick
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);

    while (true) {
        // Seleciona a entrada do ADC correspondente ao pino do eixo X (Canal 0 para o GPIO 26)
        adc_select_input(0);
        
        // Lê o valor bruto (0 a 4095, devido aos 12-bits do RP2040)
        uint16_t adc_x_raw = adc_read();

        // Converte o valor lido para tensão. A referência padrão do RP2040 é 3.3V.
        float tensao_x = (adc_x_raw * 3.3f) / 4095.0f;

        // Prepara a string com o texto que será plotado
        char str_tensao[32];
        sprintf(str_tensao, "Eixo X: %.2f V", tensao_x);

        // 3. Imprime os dados no Display OLED
        ssd1306_clear(&display); // Limpa o buffer
        ssd1306_draw_string(&display, 10, 20, 1, str_tensao); // Desenha a string

        // ETAPA OPCIONAL: Apresentar a curva/comportamento (Barra de progresso)
        // Mapeia o valor bruto do ADC para o comprimento do display em pixels (0 a 128)
        int barra_largura = (adc_x_raw * 128) / 4095;
        ssd1306_draw_empty_square(&display, 0, 40, 128, 10); // Contorno da barra
        ssd1306_draw_square(&display, 0, 40, barra_largura, 10); // Preenchimento dinâmico

        // Atualiza a tela física
        ssd1306_show(&display);

        // 4. Tempo de amostragem exigido: 500 ms
        sleep_ms(500);
    }
    return 0;
}
