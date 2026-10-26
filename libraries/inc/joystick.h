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

    float previous_x;
    float previous_y;

    bool wasInitialized;
    int8_t directionSign_x;
    int8_t directionSign_y;
    float deadZone_x;
    float deadZone_y;
}joystick_t;

void joystick_init(joystick_t *joystick, ADC_HandleTypeDef *hadc, volatile uint16_t *adc_dataPtr);
void joystick_update(joystick_t *joystick);
void joystick_invertAxis_x(joystick_t *joystick);
void joystick_invertAxis_y(joystick_t *joystick);
void joystick_setDeadZone_x(joystick_t *joystick, float deadZone);
void joystick_setDeadZone_y(joystick_t *joystick, float deadZone);
float joystick_getPosition_x(joystick_t *joystick);
float joystick_getPosition_y(joystick_t *joystick);
bool joystick_isBeyondThreshold_x(joystick_t *joystick, float threshold);
bool joystick_isBeyondThreshold_y(joystick_t *joystick, float threshold);
bool joystick_hasCrossedThreshold_x(joystick_t *joystick, float threshold);
bool joystick_hasCrossedThreshold_y(joystick_t *joystick, float threshold);
