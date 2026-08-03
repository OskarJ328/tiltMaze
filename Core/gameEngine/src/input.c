#include "input.h"
#include "button.h"
#include "stddef.h"


void input_init(input_t *input){
    input->buttons = buttons;
    input->buttonsCount = ARRAY_SIZE(buttons);

    input->imu = &imu;

}

void input_addButton(input_t *input, button_t *button, ButtonID id){
    if(id >= input->buttonsCount){
        return;
    }
    input->buttons[id] = button;
}

button_t *input_getButton(input_t *input, ButtonID id){
    if(id >= input->buttonsCount){
        return NULL;
    }
    return input->buttons[id];
}

imu_t *input_getInput(input_t *input){
    return input->imu;
}

void input_update(input_t *input, uint32_t deltaTime_ms){
    for(uint8_t buttonIdx = 0; buttonIdx < input->buttonsCount; buttonIdx++){
        if(input->buttons[buttonIdx] != NULL){
            button_update(input->buttons[buttonIdx], deltaTime_ms);
        }
    }
}