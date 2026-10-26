#include "input.h"
#include "basicTypes.h"
#include "button.h"
#include "joystick.h"
#include "stddef.h"
#include <stdint.h>


static button_t buttons[6];

static joystick_t joysticks[1];
static uint16_t joysticks_data[2];

static imu_t imu;



void input_init(input_t *input){
    input->buttons = buttons;
    input->buttonsCount = ARRAY_SIZE(buttons);

    input->joysticks = joysticks;
    input->joysticksCount = ARRAY_SIZE(joysticks);

    input->imu = &imu;

}

void input_addButton(input_t *input, ButtonID id, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState activeState){
    if(id >= input->buttonsCount){
        return;
    }
    button_t button;
    button_init(&button, port, pin, activeState);
    input->buttons[id] = button;
}

button_t *input_getButton(input_t *input, ButtonID id){
    if(id >= input->buttonsCount){
        return NULL;
    }
    return &input->buttons[id];
}

void input_addJoystick(input_t *input, JoystickId id, ADC_HandleTypeDef *hadc){
    if(id >= input->joysticksCount){
        return;
    }
    joystick_t joystick;
    joystick_init(&joystick, hadc, &joysticks_data[id * 2]);
    input->joysticks[id] = joystick;
}

joystick_t *input_getJoystick(input_t *input, JoystickId id){
    if(id >= input->joysticksCount){
        return NULL;
    }
    else{
        return &input->joysticks[id];
    }
}


imu_t *input_getImu(input_t *input){
    return input->imu;
}

void input_update(input_t *input, uint32_t deltaTime_ms){
    for(uint8_t buttonIdx = 0; buttonIdx < input->buttonsCount; buttonIdx++){
        button_update(&input->buttons[buttonIdx], deltaTime_ms);
    }
    for(uint8_t joystickIdx = 0; joystickIdx < input->joysticksCount; joystickIdx++){
        joystick_update(&input->joysticks[joystickIdx]);
    }
}