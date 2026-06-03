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

/**
 * @brief Clarke Transformation
 * @param tUvw 输入的UVW相电流
 * @param tAlphaBeta 输出的静止两相坐标系电流
 */
void FOC_Clarke(const MATH_3SystF_t* const tUvw, MATH_2SystF_t* tAlphaBeta);

/**
 * @brief Park Transformation
 * @param tAlphaBeta 输入的静止两相电流
 * @param tDq 输出的旋转两相电流
 * @param fAngel 电角度(0~1)
 */
void FOC_Park(const MATH_2SystF_t* const tAlphaBeta, MATH_2SystF_t* tDq, float fAngel);

/**
 * @breif InvPack Transformation
 * @param tDq 输入的旋转两相电流
 * @param tAlphaBeta 输出的静止两相
 * @param fAngel 电角度(0~1)
 */
void FOC_InvPark(const MATH_2SystF_t* const tDq, MATH_2SystF_t* tAlphaBeta, float fAngel);

#endif //FOC_SPIING4_TEST_FOC_MATH_C_H
