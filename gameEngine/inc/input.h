#pragma once
#include "button.h"
#include "imu.h"
#include "joystick.h"

typedef enum{
    BUTTON_PAUSE
    ,BUTTON_RESUME
    //,BUTTON_CONFIRM
    ,BUTTON_UP
    ,BUTTON_DOWN
    ,BUTTON_LEFT
    ,BUTTON_RIGHT
}ButtonID;


typedef enum {
    LEFT_ANALOG
    //,RIGHT_ANALOG
}JoystickId;

typedef struct{
    button_t    *buttons;
    uint16_t    buttonsCount;

    joystick_t  *joysticks;
    uint16_t    joysticksCount;

    imu_t       *imu;
}input_t;


void input_init(input_t *input);

void input_addButton(input_t *input, ButtonID id, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState activeState);
button_t *input_getButton(input_t *input, ButtonID id);

void input_addJoystick(input_t *input, JoystickId id, ADC_HandleTypeDef *hadc);
joystick_t *input_getJoystick(input_t *input, JoystickId id);

void input_update(input_t *input, uint32_t deltaTime_ms);