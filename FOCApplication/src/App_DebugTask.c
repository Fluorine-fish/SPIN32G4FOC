/**
*   @file App_DebugTask.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#include "adc.h"
#include "cmsis_os.h"
#include "opamp.h"
#include "tim.h"

#include "App_DebugTask.h"
#include "foc_math.h"


void App_DebugTask(void const * argument) {

    while (1) {
        osDelay(1);
    }
}
