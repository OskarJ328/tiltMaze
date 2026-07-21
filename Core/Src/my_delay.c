#include "my_delay.h"


#ifdef RTOS
#include "cmsis_os.h"
#else
#include "stm32l4xx_hal.h"
#endif

void myDelay(uint32_t ms){
#ifdef RTOS
	osDelay(ms);
#else
	HAL_Delay(ms);
#endif
}