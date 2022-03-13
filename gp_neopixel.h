#ifndef GP_NEOPIXEL_H
#define GP_NEOPIXEL_H

#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_COUNT 12

extern Adafruit_NeoPixel strip;

extern uint32_t BLACK;
extern uint32_t RED;
extern uint32_t GREEN;
extern uint32_t BLUE;
extern uint32_t CYAN;
extern uint32_t YELLOW;
extern uint32_t PURPLE;
extern uint32_t GRAY;
extern uint32_t WHITE;

void gp_neopixel_init();
void gp_neopixel_set_half(uint32_t color);
void gp_neopixel_set(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void gp_neopixel_show();

#endif
