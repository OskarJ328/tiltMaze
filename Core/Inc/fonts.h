/* vim: set ai et ts=4 sw=4: */
#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    const uint8_t   width;
    const uint8_t   height;
    const uint16_t  *data;
    const uint8_t   charSpacing;
    const uint8_t   lineSpacing;
}font_t;

extern font_t font_7x10;
extern font_t font_11x18;
extern font_t font_16x26;

#endif // __FONTS_H__
