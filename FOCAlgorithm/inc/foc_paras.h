/**
*   @file foc_paras.h
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/3
*   @version 1.0
*   @note
*/
#ifndef FOC_SPIING4_TEST_FOC_PARAS_H
#define FOC_SPIING4_TEST_FOC_PARAS_H

#include "base_math.h"

/* 电机相关参数 */
#define MOTOR_LS (0.00107367f) // 电机相电感
#define MOTOR_RS (0.7588f)     // 电机相电阻
#define MOTOR_TS (0.000050f)   // PWM控制周期

/* 标幺化参考值 */
#define PU_UB (36.333333f / F_SQRT_3) // 供电电压标幺化
#define PU_IB (36.666667f)
#define PU_FB (200.0f)
#define PU_OMEGA (PU_FB * 2.f * PI)
#define PU_ZB (PU_UB / PU_IB)
#define PU_LB (PU_ZB / PU_OMEGA)

typedef struct {
    float fPULnd;
    float fPURs;
    float fTs;
    float fCurUppLimit;
    float fCurLowLimit;
    float fIdqCutoffFreq; // 电流环控制截止频率
    float fAlignVoltage; // 对齐电压
    uint32_t u32AlignTime; // 对齐时间
}FOC_paras_t;

extern FOC_paras_t tFocParas;

#endif //FOC_SPIING4_TEST_FOC_PARAS_H
