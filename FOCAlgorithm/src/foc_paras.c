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
    .fCurUppLimit = 0.9f,
    .fCurLowLimit = -0.9f,
    .fIdqCutoffFreq = 200,

    .fAlignVoltage = 0.05f, // 0.05的相电压
    .u32AlignTime = 10000,  // 对齐2s

    .u16SpeedLoopPeriod = 4,
    .fSpeedUp = 0.0005f,
    .fSpeedDown = 0.0005f,
    .fSpeedKp = 1.4f,
    .fSpeedKi = 0.008f,         // 控制器内部不会乘Ts，输入Ki的时候需要自己乘
    .fSpeedUpperLimit = 0.8f,    // 闭环限幅
    .fSpeedLowerLimit = -0.8f,   // 闭环限幅
    .fSpeedOLUpperLimit = 0.3f,  // 开环限幅
    .fSpeedOLLowerLimit = -0.3f, // 开环限幅
};
