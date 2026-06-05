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

/*  采用并行PI环节后项差分离散的PI控制器 */
typedef struct {
    float fKpGain;         // Kp
    float fKiGain;         // Ki
    float fKaGain;         // ka
    float fUpperLimit;     // 上限幅
    float fLowerLimit;     // 下限幅
    float fInErrK_1;       // 上次误差
    float fIntePartK_1; // 累计误差
} FOC_CtrlPIpBR_t;

typedef struct {
    float fInte;
    float fTs;
}FOC_Integral_t;

typedef struct {
    float fState; // 当前状态值
    float fRampUp; // Ramp +Acc
    float fRampDown; // Ramp -Acc
} FOC_Ramp_t;

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

/**
 * @brief 带抗饱和算法的PI控制器
 * @param fInErr 输入误差
 * @param ptParam 控制器结构体
 * @return 输出结果
 */
float FOC_CtrlPIpBR(float fInErr, FOC_CtrlPIpBR_t* const ptParam);

/**
 * @brief 母线电压补偿
 * @param fDcbusActual 母线电压实际值(0~1)
 * @param tUAlphaBeta 输入InvPark后的电压
 * @param tUAlphaBetaComp 输出补偿后
 */
void FOC_DcbusComp(float fDcbusActual, MATH_2SystF_t* tUAlphaBeta, MATH_2SystF_t* tUAlphaBetaComp);

/**
 * @brief 斜坡规划函数
 * @param fIn 目标值
 * @param ptRamp 规划结构体
 * @return 规划后的最终速度
 */
float FOC_Ramp(float fIn, FOC_Ramp_t* const ptRamp);

/**
 * @brief 求解离散积分
 * @param fIn 当前值
 * @param ptInte 积分结构体
 * @return 从0时刻到现在的积分值
 */
float FOC_Integrate(float fIn, FOC_Integral_t* const ptInte);

#endif //FOC_SPIING4_TEST_FOC_MATH_C_H
