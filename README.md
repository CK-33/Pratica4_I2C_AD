# Leitura de Joystick com Exibição de Dados em Display OLED no RP2040

Projeto acadêmico desenvolvido para o Curso de Graduação em Engenharia Elétrica da Universidade Federal do Vale do São Francisco (UNIVASF) — Juazeiro, BA, 2026.

---

## Equipe

| Papel | Nome |
|---|---|
| **Líder** | Ítalo Gustavo Vieira Souza |
| Colaborador | Anderson Luiz Barreto |
| Colaboradora | Maria Clara de Souza Silva |
| Colaborador | Thyago Torres de Castro Gama |

**Orientador:** Prof. Dr. Ricardo Menezes Prates

---

## Descrição

Este projeto tem como objetivo realizar a leitura analógica do eixo X de um Joystick utilizando o Conversor Analógico-Digital (ADC) do microcontrolador Raspberry Pi Pico W (RP2040) e exibir os dados processados em tempo real em um display OLED SSD1306 via barramento I2C. O desenvolvimento foi realizado na linguagem C com o Pico SDK, utilizando o VS Code integrado ao simulador Wokwi para a validação completa do circuito e do firmware.

---

## Objetivos

- Configurar e ler canais analógicos utilizando o periférico ADC do RP2040;
- Implementar a comunicação I2C a 400 kHz para controle do Display OLED SSD1306;
- Converter valores brutos do ADC de 12 bits para escala de tensão elétrica (0V a 3.3V);
- Desenhar elementos gráficos dinâmicos (strings e barras de progresso) no display OLED;
- Utilizar o ambiente VS Code integrado ao simulador Wokwi para desenvolvimento e testes;
- Praticar versionamento de código com Git e GitHub.

---

## Materiais Utilizados

- Placa BitDogLab (Ambiente base)
- Microcontrolador Raspberry Pi Pico W (RP2040)
- Módulo Joystick Analógico (Eixo X conectado ao pino GP26 / ADC0)
- Display OLED SSD1306 (128x64 pixels, interface I2C nos pinos GP14 e GP15)
- VS Code com simulador Wokwi

---

## Desenvolvimento do Firmware

O firmware foi escrito em C utilizando o Pico SDK. As etapas de desenvolvimento foram as seguintes:

**1. Configuração de hardware:** inicialização do periférico de entrada padrão (`stdio`), ativação do barramento `i2c1` mapeado nos pinos GP14 (SDA) e GP15 (SCL) com resistores de *pull-up* internos ligados, e inicialização do bloco ADC com o pino GP26 configurado como entrada analógica.

**2. Inicialização do Display:** configuração dos parâmetros internos do SSD1306 (resolução de 128x64 e endereço I2C `0x3C`) enviando os comandos de inicialização de tela através de funções dedicadas de baixo nível.

**3. Processamento de Dados:** - Leitura do valor digital bruto de 12 bits (intervalo de 0 a 4095);
- Conversão matemática do valor lido para tensão equivalente utilizando a referência de 3.3V: `Tensão = (Valor_ADC * 3.3) / 4095`;
- Formatação dos dados em string para exibição textual.

**4. Elementos Gráficos e Renderização:** - Limpeza do buffer de tela a cada ciclo para evitar sobreposição;
- Escrita da string de tensão na coordenada estipulada;
- Mapeamento proporcional do valor do ADC para o tamanho físico da tela (0 a 128 pixels) para desenhar uma barra de progresso horizontal (contorno vazado e preenchimento dinâmico);
- Transferência do buffer de memória para a tela física.

**5. Loop principal:** estrutura `while(true)` com taxa de amostragem definida por um atraso de 500 ms (`sleep_ms(500)`), garantindo leituras e atualizações de tela estáveis.

---

## Estrutura do Repositório

| Arquivo | Descrição |
|---|---|
| `pratica.c` | Código-fonte principal do firmware. Realiza a inicialização dos periféricos (I2C e ADC), executa o cálculo de conversão de tensão, renderiza os textos/gráficos e gerencia a amostragem de 500 ms. |
| `ssd1306.c` | Implementação dos drivers do display OLED. Contém o mapa da fonte de caracteres 5x7 e as funções de desenho de pixels, strings, retângulos vazados e preenchidos, além do envio de comandos via I2C blocking. |
| `ssd1306.h` | Arquivo de cabeçalho da biblioteca do display. Define a estrutura de dados `ssd1306_t` (que aloca o buffer de tela de 1024 bytes) e declara os protótipos de todas as funções públicas de tela. |
| `ssd1306_conf.h` | Arquivo de configuração privada legado para a biblioteca SSD1306, contendo definições de famílias de microcontroladores, barramentos e fontes inclusas. |
| `CMakeLists.txt` | Arquivo de configuração do CMake. Define os padrões de build C11/CXX17, importa o Pico SDK (versão 2.2.0), associa as fontes `pratica.c` e `ssd1306.c` e vincula as bibliotecas de hardware (`hardware_adc`, `hardware_i2c`). |
| `diagram.json` | Arquivo de configuração do simulador Wokwi. Descreve as conexões físicas exatas entre o Raspberry Pi Pico W, o display OLED SSD1306 (I2C) e o Joystick analógico (GP26). |
| `wokwi.toml` | Arquivo de diretivas do Wokwi que aponta para o binário de execução compilado (`build/pratica.elf`) para iniciar a simulação automatizada. |

---

## Como Testar

### Pré-requisitos

- [VS Code](https://code.visualstudio.com/) instalado;
- Extensão [Wokwi for VS Code](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode) instalada;
- [Pico SDK (v2.2.0)](https://github.com/raspberrypi/pico-sdk) configurado no ambiente;
- CMake e compilador ARM GCC (`arm-none-eabi-gcc`) instalados.

### Compilação

1. Clone o repositório e acesse a pasta do projeto;
2. Crie o diretório de build e compile o projeto:

```bash
mkdir build
cd build
cmake ..
make
