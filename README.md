# Leitura de Joystick com Exibição em Display OLED — RP2040

Projeto acadêmico desenvolvido para o **Curso de Graduação em Engenharia Elétrica** da Universidade Federal do Vale do São Francisco (UNIVASF) — Juazeiro, BA, 2026.

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

Este projeto realiza a **leitura analógica do eixo X de um Joystick** utilizando o Conversor Analógico-Digital (ADC) do microcontrolador **Raspberry Pi Pico W (RP2040)** e exibe os dados processados em tempo real em um **display OLED SSD1306** via barramento I2C.

O desenvolvimento foi realizado em **linguagem C** com o Pico SDK, utilizando o VS Code integrado ao simulador **Wokwi** para validação completa do circuito e do firmware.

---

## Objetivos

- Configurar e ler canais analógicos utilizando o periférico ADC do RP2040;
- Implementar a comunicação I2C a 400 kHz para controle do Display OLED SSD1306;
- Converter valores brutos do ADC de 12 bits para escala de tensão elétrica (0 V a 3,3 V);
- Desenhar elementos gráficos dinâmicos (strings e barras de progresso) no display OLED;
- Utilizar o ambiente VS Code integrado ao simulador Wokwi para desenvolvimento e testes;
- Praticar versionamento de código com Git e GitHub.

---

## Materiais Utilizados

| Componente | Descrição |
|---|---|
| Placa BitDogLab | Ambiente base |
| Raspberry Pi Pico W (RP2040) | Microcontrolador principal |
| Módulo Joystick Analógico | Eixo X conectado ao pino GP26 / ADC0 |
| Display OLED SSD1306 | 128×64 pixels, interface I2C nos pinos GP14 e GP15 |
| VS Code + Wokwi | Ambiente de desenvolvimento e simulação |

---

## Desenvolvimento do Firmware

O firmware foi escrito em C utilizando o Pico SDK. As etapas de desenvolvimento foram:

**1. Configuração de hardware**
Inicialização do periférico de entrada padrão (`stdio`), ativação do barramento `i2c1` mapeado nos pinos GP14 (SDA) e GP15 (SCL) com resistores de pull-up internos, e inicialização do bloco ADC com o pino GP26 configurado como entrada analógica.

**2. Inicialização do Display**
Configuração dos parâmetros internos do SSD1306 (resolução 128×64 e endereço I2C `0x3C`), enviando os comandos de inicialização de tela através de funções dedicadas de baixo nível.

**3. Processamento de Dados**
- Leitura do valor digital bruto de 12 bits (intervalo de 0 a 4095);
- Conversão matemática para tensão equivalente com referência de 3,3 V:

$$\text{Tensão} = \frac{\text{Valor\_ADC} \times 3{,}3}{4095}$$

- Formatação dos dados em string para exibição textual.

**4. Elementos Gráficos e Renderização**
- Limpeza do buffer de tela a cada ciclo para evitar sobreposição;
- Escrita da string de tensão na coordenada estipulada;
- Mapeamento proporcional do valor do ADC para o tamanho físico da tela (0 a 128 pixels) para desenhar uma barra de progresso horizontal (contorno vazado e preenchimento dinâmico);
- Transferência do buffer de memória para a tela física.

**5. Loop principal**
Estrutura `while(true)` com taxa de amostragem definida por um atraso de 500 ms (`sleep_ms(500)`), garantindo leituras e atualizações de tela estáveis.

---

## Estrutura do Repositório

```
.
├── pratica.c          # Código-fonte principal do firmware
├── ssd1306.c          # Implementação dos drivers do display OLED
├── ssd1306.h          # Cabeçalho da biblioteca do display
├── ssd1306_conf.h     # Configuração privada legada da biblioteca SSD1306
├── CMakeLists.txt     # Configuração do CMake
├── diagram.json       # Configuração do simulador Wokwi
└── wokwi.toml         # Diretivas do Wokwi para o binário de execução
```

| Arquivo | Descrição |
|---|---|
| `pratica.c` | Inicialização dos periféricos (I2C e ADC), cálculo de conversão de tensão, renderização de textos/gráficos e amostragem de 500 ms. |
| `ssd1306.c` | Mapa da fonte de caracteres 5×7 e funções de desenho de pixels, strings, retângulos vazados e preenchidos, além do envio de comandos via I2C. |
| `ssd1306.h` | Define a estrutura `ssd1306_t` (buffer de tela de 1024 bytes) e declara os protótipos de todas as funções públicas de tela. |
| `ssd1306_conf.h` | Definições de famílias de microcontroladores, barramentos e fontes inclusas. |
| `CMakeLists.txt` | Define padrões de build C11/CXX17, importa o Pico SDK (v2.2.0), associa as fontes e vincula as bibliotecas `hardware_adc` e `hardware_i2c`. |
| `diagram.json` | Descreve as conexões físicas entre o Raspberry Pi Pico W, o display OLED SSD1306 e o Joystick analógico (GP26). |
| `wokwi.toml` | Aponta para o binário compilado (`build/pratica.elf`) para iniciar a simulação automatizada. |

---

## Como Testar

### Pré-requisitos

- VS Code instalado;
- Extensão **Wokwi for VS Code** instalada;
- **Pico SDK (v2.2.0)** configurado no ambiente;
- **CMake** e compilador **ARM GCC** (`arm-none-eabi-gcc`) instalados.

### Compilação

Clone o repositório, acesse a pasta do projeto e execute:

```bash
mkdir build
cd build
cmake ..
make
```

O arquivo `.uf2` gerado estará disponível na pasta `build/`.

### Simulação no Wokwi

1. Abra o projeto no VS Code;
2. Certifique-se de que o arquivo `diagram.json` está presente na raiz;
3. Inicie a simulação pela extensão Wokwi;
4. Com a simulação ativa:
   - Clique e arraste o ponto central do Joystick para as laterais (alterando o eixo horizontal);
   - Observe a mudança instantânea no Display OLED: o texto exibirá o valor em Volts (0,00 V a 3,30 V) e a barra gráfica inferior aumentará ou diminuirá proporcionalmente ao movimento.

### Execução em Hardware Real

1. Conecte o Raspberry Pi Pico W ao computador via cabo USB mantendo o botão **BOOTSEL** pressionado para entrar no modo de gravação;
2. Copie o arquivo `pratica.uf2` (pasta `build/`) para o diretório de armazenamento em massa que aparecer no sistema;
3. O microcontrolador reiniciará automaticamente e executará o firmware;
4. Mova o joystick físico na placa BitDogLab e acompanhe a resposta gráfica diretamente no display OLED.

### Observações

- A taxa de atualização da tela ocorre estritamente a cada **500 ms**;
- O cálculo considera a resolução nativa do ADC do RP2040: **12 bits (4096 níveis)**.

---

## Vídeo Demonstrativo

[Assistir no Google Drive](https://drive.google.com/file/d/1rpi9E6v15zoS-oQ90uiiP5ECENOgUwqe/view?usp=drive_link)

---

## Conclusão

A integração entre o conversor analógico-digital (ADC) e o display OLED via protocolo I2C foi realizada com pleno sucesso. O firmware respondeu de maneira linear e precisa às variações analógicas aplicadas ao Joystick, processando as grandezas de tensão elétrica sem latência perceptível. A biblioteca gráfica demonstrou robustez na renderização dinâmica de strings e formas geométricas no buffer local. O ambiente simulado no Wokwi reproduziu fielmente o comportamento esperado para o hardware, consolidando os conceitos práticos de comunicação serial, aquisição de sinais e interfaces de saída em sistemas embarcados.
