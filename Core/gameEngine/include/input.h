#pragma once
#include "button.h"
#include "imu.h"

typedef struct{
    button_t *buttons;
    uint16_t buttonsCount;

    imu_t *imu;
}input_t;


void input_init(input_t *input);
button_t *input_getButton(input_t *input, ButtonID id);