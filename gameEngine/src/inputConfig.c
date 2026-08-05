#include "inputConfig.h"
#include "input.h"
#include "gpio.h"


void inputConfig_init(input_t *input){
    input_addButton(input, BUTTON_PAUSE, BUTTON_PAUSE_GPIO_Port, BUTTON_PAUSE_Pin, GPIO_PIN_RESET);
    input_addButton(input, BUTTON_RESUME, BUTTON_RESUME_GPIO_Port, BUTTON_RESUME_Pin, GPIO_PIN_RESET);
}