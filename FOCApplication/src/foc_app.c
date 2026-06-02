/**
*   @file foc_app.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#include "foc_app.h"

float fAmp = 0.1f; // 旋转矢量的幅值
float fAngle = 0; // 旋转角度(0~1)
MATH_3SystF_t tSwTime;
MATH_3SystF_t fSwDuty;
MATH_2SystF_t tAlphaBeta;
float fIncAngel = 0.01f; //递增角度 (0~1)

int16_t i16Delay = 100;
int16_t i16DelayCnt = 0;

int16_t gi16ADCRaw[8] = {};
void APP_CurrentRestruct(void) {
    gi16ADCRaw[0] = hadc1.Instance->JDR1; // u phase current
    gi16ADCRaw[1] = hadc2.Instance->JDR1; // v phase current
    gi16ADCRaw[2] = hadc1.Instance->JDR3; // w phase current
    gi16ADCRaw[3] = hadc1.Instance->JDR2; // v bus voltage
    // 实际测量得到的更应该成为端电压
    gi16ADCRaw[4] = hadc1.Instance->JDR4; // u phase voltage
    gi16ADCRaw[5] = hadc2.Instance->JDR4; // v phase voltage
    gi16ADCRaw[6] = hadc2.Instance->JDR2; // w phase voltage
    gi16ADCRaw[7] = hadc2.Instance->JDR3; // ntc voltage
}

// 在ADC中断中调用调度器函数
void APP_FocScheduler(void) {
    APP_CurrentRestruct();

    FOC_DecompAlphaBeta(fAmp, fAngle, &tAlphaBeta);
    FOC_Svpwm(&tAlphaBeta, &tSwTime, &fSwDuty);
    TIM1->CCR1 = (uint32_t)(tSwTime.fArg1 * (PWM_PERIOD_CYCLES));
    TIM1->CCR2 = (uint32_t)(tSwTime.fArg2 * (PWM_PERIOD_CYCLES));
    TIM1->CCR3 = (uint32_t)(tSwTime.fArg3 * (PWM_PERIOD_CYCLES));

    // 在ADC中断中延时角度增加
    if (i16DelayCnt >= i16Delay) {
        fAngle += fIncAngel;
        fAngle = fAngle > 1.f ? 0.f : fAngle;

        i16DelayCnt = 0;
    }
    i16DelayCnt ++;

    //pStateFuncTable[tAppState.tEvent][tAppState.tStatus]();

}

void StateFault(void){}

void StateInit(void){}

void StateReady(void){}

void StateCalib(void){}

void StateAlign(void){}

void StateRun(void){}

void StateReset(void){}
