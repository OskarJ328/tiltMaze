#pragma once

#include "gpio.h"
#include "stdbool.h"

typedef struct{
    GPIO_TypeDef        *port;
    uint16_t            pin;

    GPIO_PinState       activeState;

    bool                previousState;
    bool                currentState;

    uint32_t            holdTime_ms;
}button_t;

void button_init(button_t *button, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState activeState);
void button_update(button_t  *button, uint32_t deltaTime_ms);
bool button_isPressed(button_t *button);
bool button_isReleased(button_t *button);
bool button_wasPressed(button_t *button);
bool button_wasReleased(button_t *button);
bool button_isHeldFor(button_t *button, uint32_t time_ms);
uint32_t button_getHoldTime_ms(button_t *button);