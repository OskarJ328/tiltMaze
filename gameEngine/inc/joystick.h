#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "dma.h"
#include "adc.h"


typedef struct{
    ADC_HandleTypeDef *hadc;
    volatile uint16_t *adc_dataPtr;
    uint16_t adc_resolution;

    float x;
    float y;
    bool wasInitialized;
    int8_t directionSign_x;
    int8_t directionSign_y;
}joystick_t;

void joystick_init(joystick_t *joystick, ADC_HandleTypeDef *hadc, volatile uint16_t *adc_dataPtr);
void joystick_update(joystick_t *joystick);
void joystick_invertAxis_x(joystick_t *joystick);
void joystick_invertAxis_y(joystick_t *joystick);
float joystick_getPosition_x(joystick_t *joystick);
float joystick_getPosition_y(joystick_t *joystick);