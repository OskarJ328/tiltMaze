#include "inputConfig.h"
#include "input.h"
#include "gpio.h"


void inputConfig_init(input_t *input){
    input_addButton(input, BUTTON_UP, BUTTON_UP_GPIO_Port, BUTTON_UP_Pin, GPIO_PIN_RESET);
    input_addButton(input, BUTTON_DOWN, BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin, GPIO_PIN_RESET);
}