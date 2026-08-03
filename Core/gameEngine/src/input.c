#include "input.h"
#include "button.h"
#include "stddef.h"


void input_init(input_t *input){
    input->buttons = buttons;
    input->buttonsCount = ARRAY_SIZE(buttons);

    input->imu = &imu;

}

button_t *input_getButton(input_t *input, ButtonID id){
    if(id >= input->buttonsCount){
        return NULL;
    }
    return &input->buttons[id];
}

imu_t *input_getInput(input_t *input){
    return input->imu;
}