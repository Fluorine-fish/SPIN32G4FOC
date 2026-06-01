/**
*   @file foc_math.c.h
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#ifndef FOC_SPIING4_TEST_FOC_MATH_C_H
#define FOC_SPIING4_TEST_FOC_MATH_C_H

#include "base_math.h"

/**
 * @brief Foc为输出SVPWM计算占空比
 * @param tAlphaBeta 输入的alpha beta相电压
 * @param tSwTim 输出的三相开关时间
 * @param t3PhDuty 输出的三相开关占空比（可以用于观测端电压）
 * @return
 */
uint16_t FOC_Svpwm(
    MATH_2SystF_t* tAlphaBeta,
    MATH_3SystF_t* tSwTim,
    MATH_3SystF_t* t3PhDuty);

/**
 * @brief 将向量解算到两项坐标系
 * @param fVecAmp 向量幅值
 * @param fAngel 0～1的角度
 * @param tAlphaBeta 解算结果
 * @return
 */
int16_t FOC_DecompAlphaBeta(float fVecAmp, float fAngel, MATH_2SystF_t* tAlphaBeta);

#endif //FOC_SPIING4_TEST_FOC_MATH_C_H
