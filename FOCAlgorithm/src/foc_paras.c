/**
*   @file foc_paras.c
*   @brief 控制相关参数
*   @author Wenxin HU
*   @date 2026/6/3
*   @version 1.0
*   @note
*/

#include "foc_paras.h"

FOC_paras_t tFocParas = {
    .fPULnd = (float)(MOTOR_LS / PU_LB),
    .fPURs = (float)(MOTOR_RS / PU_ZB),
    .fTs = (float)(MOTOR_TS),
    .fCurUppLimit = 0.5f,
    .fCurLowLimit = -0.5f,
    .fIdqCutoffFreq = 200,
};
