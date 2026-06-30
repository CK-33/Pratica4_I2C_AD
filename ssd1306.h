#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/types.h"

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t address;
    i2c_inst_t *i2c_port;
    uint8_t buffer[1024];
} ssd1306_t;

void ssd1306_init(ssd1306_t *p, uint8_t width, uint8_t height, uint8_t address, i2c_inst_t *i2c_port);
void ssd1306_clear(ssd1306_t *p);
void ssd1306_show(ssd1306_t *p);
void ssd1306_draw_string(ssd1306_t *p, uint8_t x, uint8_t y, uint8_t scale, const char *str);
void ssd1306_draw_empty_square(ssd1306_t *p, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void ssd1306_draw_square(ssd1306_t *p, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

#endif
