#pragma once
#include "button.h"
#include "imu.h"
#include <stdint.h>

typedef struct{
    button_t *buttons;
    uint16_t buttonsCount;

    imu_t *imu;
}input_t;


void input_init(input_t *input);
void input_addButton(input_t *input, ButtonID id, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState activeState);
button_t *input_getButton(input_t *input, ButtonID id);
void input_update(input_t *input, uint32_t deltaTime_ms);