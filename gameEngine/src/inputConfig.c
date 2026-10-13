#include "inputConfig.h"
#include "input.h"

void inputConfig_init(input_t *input){
    input_addButton(input, BUTTON_RESUME, BUTTON_UP_GPIO_Port, BUTTON_UP_Pin, GPIO_PIN_RESET);
    input_addButton(input, BUTTON_PAUSE, BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin, GPIO_PIN_RESET);
    input_addJoystick(input, LEFT_ANALOG, &hadc1);
}