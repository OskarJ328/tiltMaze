#include "button.h"

static bool button_stateChanged(button_t *button){
    return button->currentState != button->activeState;
}

void button_init(button_t *button, GPIO_TypeDef *port, uint16_t pin, GPIO_PinState activeState){
    button->port = port;
    button->pin = pin;
    button->activeState = activeState;
    button->currentState = false;
    button->previousState = false;
    button->holdTime_ms = 0;
}

void button_update(button_t  *button, uint32_t deltaTime_ms){
    button->previousState = button->currentState;
    GPIO_PinState state = HAL_GPIO_ReadPin(button->port, button->pin);
    if(state == button->activeState){
        button->currentState = true;
        button->holdTime_ms += deltaTime_ms;
    }
    else{
        button->activeState = false;
        button->holdTime_ms = 0;
    }
}

bool button_isPressed(button_t *button){
    if(button->currentState == button->activeState){
        return true;
    }
    return false;
}

bool button_isReleased(button_t *button){
    if(button->currentState != button->activeState){
        return true;
    }
    return false;
}
bool button_wasPressed(button_t *button){
    if(button_stateChanged(button) && button->currentState){
        return true;
    }
    return false;
}
bool button_wasReleased(button_t *button){
    if(button_stateChanged(button) && !button->currentState){
        return true;
    }
    return false;
}
bool button_isHeldFor(button_t *button, uint32_t time_ms){
    if(button->holdTime_ms > time_ms){
        return true;
    }
    return false;
}
uint32_t button_getHoldTime_ms(button_t *button){
    return button->holdTime_ms;
}

button_t buttons[4];