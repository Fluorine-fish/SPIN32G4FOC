/**
*   @file App_DebugTask.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#include "cmsis_os.h"
#include "mt6701_driver.h"
#include "App_DebugTask.h"
uint16_t u16SensorRaw = 0;

void App_DebugTask(void const * argument) {

    while (1) {

        osDelay(1);
    }
}
