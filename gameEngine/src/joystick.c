#include "joystick.h"
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal_adc_ex.h"
#include "stm32l4xx_hal_def.h"
#include <stdint.h>

void joystick_init(joystick_t *joystick, ADC_HandleTypeDef *hadc, volatile uint16_t *adc_dataPtr){
    joystick->hadc = hadc;
    joystick->adc_dataPtr = adc_dataPtr;
    joystick->directionSign_x = 1;
    joystick->directionSign_y = 1;

    uint32_t adc_definedResolution = hadc->Init.Resolution;
    uint16_t adc_bits;
    switch (adc_definedResolution){
    case ADC_RESOLUTION_12B:
        adc_bits = 12;
        break;
    case ADC_RESOLUTION_10B:
        adc_bits = 10;
        break;
    case ADC_RESOLUTION_8B:
        adc_bits = 8;
        break;
    case ADC_RESOLUTION_6B:
        adc_bits = 6;
        break;
    default:
        adc_bits = 0;
        break;
    }
    joystick->adc_resolution = (1 << adc_bits) - 1;
    if(HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK){
        joystick->wasInitialized = false;
        return;
    };
    if(HAL_ADC_Start_DMA(hadc,(uint32_t*)adc_dataPtr, 2) != HAL_OK){
        joystick->wasInitialized = false;
        return;
    };
    joystick->wasInitialized = true;
}

static float joystick_calculatePosition(uint16_t adcVal, uint16_t adcResolution){
    float joyPos = (adcVal * 200.0f / adcResolution) - 100.0f;
    return joyPos; 
}

void joystick_update(joystick_t *joystick){
    if(joystick->wasInitialized){
        joystick->x = joystick->directionSign_x * joystick_calculatePosition(joystick->adc_dataPtr[0], joystick->adc_resolution);
        joystick->y = joystick->directionSign_y * joystick_calculatePosition(joystick->adc_dataPtr[1], joystick->adc_resolution);
    }
}

void joystick_invertAxis_x(joystick_t *joystick){
    joystick->directionSign_x *= -1;
}
void joystick_invertAxis_y(joystick_t *joystick){
    joystick->directionSign_y *= -1;
}

float joystick_getPosition_x(joystick_t *joystick){
    if(joystick->wasInitialized){
        return joystick->x;
    }
    return 0;
}

float joystick_getPosition_y(joystick_t *joystick){
    if(joystick->wasInitialized){
        return joystick->y;
    }
    return 0;
}

