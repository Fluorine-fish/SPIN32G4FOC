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

uint16_t duty[3] = {};
float fAmp = 0.1f; // 旋转矢量的幅值
float fAngle = 0; // 旋转角度（0～1）
MATH_3SystF_t tSwTime;
MATH_3SystF_t fSwDuty;
MATH_2SystF_t tAlphaBeta;
int16_t i16Delay = 1;
float fIncAngel = 0.06f; //递增角度

static void FocInit() {
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start(&htim1);

    // PWM Init
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

    // ADC Init
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_ADCEx_InjectedStart_IT(&hadc1);
    HAL_ADCEx_InjectedStart_IT(&hadc2);

    // OPAMP Init
    HAL_OPAMP_Start(&hopamp1);
    HAL_OPAMP_Start(&hopamp2);
    HAL_OPAMP_Start(&hopamp3);
}

void App_DebugTask(void const * argument) {

    FocInit();

    while (1) {
        FOC_DecompAlphaBeta(fAmp, fAngle, &tAlphaBeta);
        FOC_Svpwm(&tAlphaBeta, &tSwTime, &fSwDuty);
        TIM1->CCR1 = (uint32_t)(tSwTime.fArg1 * (PWM_PERIOD_CYCLES));
        TIM1->CCR2 = (uint32_t)(tSwTime.fArg2 * (PWM_PERIOD_CYCLES));
        TIM1->CCR3 = (uint32_t)(tSwTime.fArg3 * (PWM_PERIOD_CYCLES));

        fAngle += fIncAngel;
        fAngle = fAngle > 1.f ? 0.f : fAngle;

        osDelay(i16Delay);
    }
}
