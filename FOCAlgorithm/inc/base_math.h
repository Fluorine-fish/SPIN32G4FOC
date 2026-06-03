/**
*   @file base_math.h
*   @brief 
*   @author Wenxin HU
*   @date 2026/6/1
*   @version 1.0
*   @note
*/
#ifndef FOC_SPIING4_TEST_BASE_MATH_H
#define FOC_SPIING4_TEST_BASE_MATH_H

#include "stdint.h"

#define F_SQRT_3 (1.7320508075f)
#define PI (3.1415926535f)

// 两变量体系结构体
typedef struct {
    float fArg1; // alpha d
    float fArg2; // beta q
}MATH_2SystF_t;

// 三变量体系结构体
typedef struct {
    float fArg1; // A U
    float fArg2; // B V
    float fArg3; // C W
}MATH_3SystF_t;

float BM_FastSin(float fAngle);
float BM_FastCos(float fAngle);

#endif //FOC_SPIING4_TEST_BASE_MATH_H
