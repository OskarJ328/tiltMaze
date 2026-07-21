#pragma once

#include "gpio.h"
#include "spi.h"
#include <stdbool.h>
#include <stdint.h>
#include "fonts.h"
#include "images.h"

#define	ILI9341_BLACK   0x0000
#define	ILI9341_BLUE    0x001F
#define	ILI9341_RED     0xF800
#define	ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF

#define ILI9341_WIDTH   240
#define ILI9341_HEIGHT  320

typedef enum {
    defaultOrientation,
    ninetyDeg,
    minusNinetyDeg,
    oneHundredEightyDeg,
}ili9341_rotation_t;

typedef enum {
    RGB,
    BGR
}ili9341_colorFormat_t;

typedef enum {
    invertX,
    invertY,
    invertBoth
}axisToInvert_t;

typedef struct {
    uint16_t            resPin;
    GPIO_TypeDef        *resPort;
    uint16_t            csPin;
    GPIO_TypeDef        *csPort;
    uint16_t            dcPin;
    GPIO_TypeDef        *dcPort;
    SPI_HandleTypeDef   *spiHandle;

    uint16_t                width;
    uint16_t                height;
    ili9341_colorFormat_t   colorFormat;
    bool                    x_isAxisInverted;
    bool                    y_isAxisInverted;
    bool                    areAxesSwapped;
}ili9341_t;

void ILI9341_init(ili9341_t *ili9341);
void ILI9341_drawPixel(ili9341_t *ili9341, uint16_t x, uint16_t y, uint16_t color);
void ILI9341_drawRectangle(ili9341_t *ili9341, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ILI9341_drawImage(ili9341_t *ili9341, uint16_t x, uint16_t y, image_t *image);
void ILI9341_fillScreen(ili9341_t *ili9341, uint16_t color);
void ILI9341_setRotation(ili9341_t *ili9341, ili9341_rotation_t rotation);
void ILI9341_invertAxis(ili9341_t *ili9341, axisToInvert_t axis);
void ILI9341_swapAxes(ili9341_t *ili9341);
void ILI9341_writeChar(ili9341_t *ili9341, uint16_t x0, uint16_t y0, font_t *font, char ch, uint16_t color, uint16_t bgColor);
void ILI9341_writeString(ili9341_t *ili9341, uint16_t x0, uint16_t y0, font_t *font, char *string, uint16_t stringSize, uint16_t color, uint16_t bgColor);